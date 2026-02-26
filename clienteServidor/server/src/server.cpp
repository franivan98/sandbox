#include "server.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

// Implementación de la clase Server
// Constructor para inicializar el servidor con el puerto y el tamaño del pool de hilos
Server::Server(int port, int poolSize) 
: port(port), poolSize(poolSize), running(true) {}

// Método para iniciar el servidor
void Server::start(){

    // Crear socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket");
        return;
    }

    // Configurar la dirección del servidor
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

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
    std::cout << "Servidor con thread pool en puerto " << port << std::endl;
    
    // Iniciar los hilos trabajadores
    for(int i = 0; i < poolSize; i++){
        workers.emplace_back(&Server::workerLoop, this);
    }
    
    while(running){
        // Aceptar una conexión entrante
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
        if(client_fd < 0){
            perror("accept");
            continue;   
        }
        // Agregar el cliente a la cola de clientes pendientes
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            clientQueue.push(client_fd);
        }
        // Notificar a los hilos trabajadores que hay un cliente pendiente
        queueCV.notify_one();
    }

    // Cerrar el socket del servidor (aunque en este caso no se llegará aquí)
    close(server_fd);

}

void Server::workerLoop(){
    while(running){
        int client_fd;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            // Esperar hasta que haya un cliente pendiente
            queueCV.wait(lock, [&]{ return !clientQueue.empty() || !running; });
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
void Server::handleClient(int client_fd){

    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received;

    // Recibir mensaje del cliente
    bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if(bytes_received < 0){
        perror("recv");
        close(client_fd);
        return;   
    }
    std::thread::id tid = std::this_thread::get_id();
    
    // Enviar respuesta al cliente 
    const char* response = "Hola desde el servidor con thread pool!";
    // Imprimir el mensaje recibido y el ID del hilo que lo maneja
    std::cout << "Hilo " << tid << " recibió: " << buffer << std::endl;
    ssize_t bytes_sent = send(client_fd, response, strlen(response), 0);
    if(bytes_sent < 0){
        perror("send");
        close(client_fd);
        return;   
    }

    // Cerrar la conexión con el cliente
    close(client_fd);
}