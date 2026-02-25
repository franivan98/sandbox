#include "server.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

#define BUFFER_SIZE 1024

// Implementación de la clase Server
Server::Server(int port) : port(port) {}

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
    if(listen(server_fd, 10) < 0){
        perror("listen");
        close(server_fd);
        return;
    }
    std::cout << "Servidor multicliente en puerto " << port << std::endl;
    
    while(true){
        // Aceptar una conexión entrante
        socklen_t addrlen = sizeof(address);
        int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);
        if(client_fd < 0){
            perror("accept");
            continue;   
        }
        // Manejar la conexión del cliente en un hilo separado
        std::thread(&Server::handleClient, this, client_fd).detach();
    }

    // Cerrar el socket del servidor (aunque en este caso no se llegará aquí)
    close(server_fd);

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
    const char* response = "Mensaje recibido por el servidor";
    ssize_t bytes_sent = send(client_fd, response, strlen(response), 0);
    if(bytes_sent < 0){
        perror("send");
        close(client_fd);
        return;   
    }

    // Cerrar la conexión con el cliente
    close(client_fd);
}