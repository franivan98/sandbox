#include "server_core.hpp"
#include "strategy/echo_strategy.hpp"
#include "strategy/time_strategy.hpp"
#include "strategy/upper_strategy.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// Constructor para inicializar el servidor con el puerto y el tamaño del pool de hilos
ServerCore::ServerCore(int port, int poolSize)
: port(port), poolSize(poolSize), running(false) {}

// Método para iniciar el servidor
void ServerCore::run(){
    running = true;

    // Crear socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket");
        return;
    }

    // Configurar la dirección del servidor
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Configurar opciones del socket para reutilizar la dirección y el puerto
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0){
        perror("setsockopt");
        close(server_fd);
        return;
    }

    // Enlazar el socket a la dirección
    if(bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0){
        perror("bind");
        close(server_fd);
        return;
    }

    // Escuchar conexiones entrantes
    // El número 128 es el tamaño de la cola de conexiones pendientes
    if(listen(server_fd, 128) < 0){
        perror("listen");
        close(server_fd);
        return;
    }
    std::cout << "Servidor Facade con thread pool en puerto " << port << std::endl;
    
    // Iniciar los hilos trabajadores
    for(int i = 0; i < poolSize; i++){
        workers.emplace_back(&ServerCore::workerLoop, this);
    }
    
    while(running){
        // Aceptar una conexión entrante
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
        if(client_fd < 0){
            if(!running){
                break; // Salir si el servidor se está deteniendo
            }
            perror("accept");
            continue;   
        }
        // Agregar el cliente a la cola de clientes pendientes
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            clientQueue.push(client_fd);
        }
        queueCV.notify_one();
    }
}

// Método para detener el servidor
void ServerCore::stop(){
    // Detener el servidor y cerrar el socket
    running = false;
    close(server_fd);
    // Notificar a los hilos trabajadores para que terminen
    queueCV.notify_all();
    // Esperar a que los hilos trabajadores terminen
    for(auto& worker : workers){
        if(worker.joinable()){
            worker.join();
        }
    }
}

// Método para el bucle de los hilos trabajadores
void ServerCore::workerLoop(){
    while(running){
        int client_fd;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this](){ return !clientQueue.empty() || !running; });
            if(!running && clientQueue.empty()){
                return; // Salir si el servidor se está deteniendo y no hay clientes pendientes
            }
            client_fd = clientQueue.front();
            clientQueue.pop();
        }
        // Manejar la conexión del cliente
        handleClient(client_fd);
    }
}

// Método para manejar la conexión de un cliente
void ServerCore::handleClient(int client_fd){
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received; 
    // Recibir mensaje del cliente
    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if(bytes_received < 0){
        perror("recv");
        close(client_fd);
        return;   
    }

    buffer[bytes_received] = '\0'; // Asegurar que el buffer esté null-terminated
    std::thread::id tid = std::this_thread::get_id();
    std::cout << "Hilo " << tid << " recibió: " << buffer << std::endl;

    // Enviar respuesta al cliente con strategy
    buffer[bytes_received] = '\0';
    std::string message(buffer);
    MessageProcessor* processor = nullptr;

    // Seleccionar la estrategia de procesamiento de mensajes según el contenido del mensaje recibido
    // Si el mensaje es "TIME", se utiliza TimeStrategy para enviar la hora actual al cliente.
    // Si el mensaje es "upper", se utiliza UpperStrategy para convertir el mensaje a mayúsculas y enviarlo de vuelta al cliente.
    // Para cualquier otro mensaje, se utiliza EchoStrategy para devolver el mensaje tal cual al cliente.

    if(message == "TIME"){
        processor = new TimeStrategy();
    } else {
        if(message == "upper"){
            processor = new UpperStrategy();
        } else {
        processor = new EchoStrategy();
        }
    }

    processor->processMessage(client_fd, message);
    shutdown (client_fd, SHUT_RDWR); // Cerrar la conexión para evitar problemas de bloqueo
    delete processor; // Liberar memoria del procesador
    close(client_fd); // Cerrar la conexión con el cliente
}

