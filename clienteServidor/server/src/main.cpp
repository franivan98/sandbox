#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    // Crear socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Verificar si el socket se creó correctamente
    if(server_fd < 0){
        perror("socket");
        return 1;
    }

    // Configurar la dirección del servidor
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Enlazar el socket a la dirección y puerto
    if( bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0 ){
        perror("bind");
        return 1;
    }

    // Escuchar por conexiones entrantes
    if(listen(server_fd, 1) < 0){
        perror("listen");
        return 1;
    }

    std::cout<<"Servidor escuchando en puerto "<<PORT<<std::endl;

    // Aceptar una conexión entrante
    int addrlen = sizeof(address);
    int client_fd = accept(server_fd, (sockaddr*)&address, (socklen_t*)&addrlen);

    if(client_fd < 0){
        perror("accept");
        return 1;
    }

    // Recibir mensaje del cliente
    char buffer[BUFFER_SIZE] = {0};
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    std::cout<<"Mensaje recibido: "<<buffer<<std::endl;

    // Cerrar los sockets
    close(client_fd);
    close(server_fd);

    return 0;

}