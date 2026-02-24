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
    socklen_t addrlen = sizeof(address);
    int client_fd = accept(server_fd, (sockaddr*)&address, &addrlen);

    if(client_fd < 0){
        perror("accept");
        return 1;
    }

    // Recibir mensaje del cliente
    char buffer[BUFFER_SIZE] = {0};
    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    
    if(bytes_received < 0){
        perror("recv");
        close(client_fd);
        close(server_fd);
        return 1;
    }
    buffer[bytes_received] = '\0'; // Asegurar que el buffer esté null-terminated

    std::cout<<"Mensaje recibido: "<<buffer<<std::endl;

    //enviamos respuesta al cliente
    const char* response = "ACK del servidor\n";
    send(client_fd, response, strlen(response), 0);

    // Cerrar los sockets
    close(client_fd);
    close(server_fd);

    return 0;

}