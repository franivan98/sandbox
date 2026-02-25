#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){

    // Crear socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    // Verificar si el socket se creó correctamente
    if(sock < 0){
        perror("socket");
        return 1;
    }

    // Configurar la dirección del servidor
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convertir la dirección IP de texto a binaria
    // En este caso, se conecta al localhost
    if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
        perror("inet_pton");
        return 1;
    }

    // Conectar al servidor
    if(connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("connect");
        return 1;
    }

    // Solicitar al usuario que ingrese un mensaje para enviar al servidor
    char input[BUFFER_SIZE];
    //printf("Ingresa el mensaje para enviar: \n");
    //fgets(input, BUFFER_SIZE, stdin);
    //input[strcspn(input, "\n")] = '\0'; // Eliminar el salto de línea al final del mensaje
    strcpy(input, "Hola desde el cliente");
    // Enviar mensaje al servidor
    ssize_t bytes_sent = send(sock, input, strlen(input), 0);
    if(bytes_sent < 0){
        perror("send");
        close(sock);
        return 1;
    }

    printf("Mensaje enviado: %s\n", input);

    ssize_t bytes_received;
    char buffer[BUFFER_SIZE] = {0};
    // Recibir respuesta del servidor
    bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
    if(bytes_received < 0){
        perror("recv");
        close(sock);
        return 1;   
    }
    buffer[bytes_received] = '\0'; // Asegurar que el buffer esté null-terminated
    printf("Respuesta del servidor: %s\n", buffer);

    close(sock); // Cerrar el socket después de recibir la respuesta
    return 0;


}