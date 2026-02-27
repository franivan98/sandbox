#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <cjson/cJSON.h>

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

    cJSON* root = cJSON_CreateObject();

    // Solicitar al usuario que ingrese un mensaje para enviar al servidor
    char input[BUFFER_SIZE];
    int op;
    printf("----QUE QUIERES HACER?----\n");
    printf("1. Enviar un mensaje al servidor\n");
    printf("2. Solicitar la hora al servidor\n");
    printf("3. convertir un mensaje a mayusculas\n");
    printf("Ingresar opcion: ");
    scanf("%d", &op);
    getchar(); // Consumir el salto de línea después de la opción
    switch(op){
        case 1:
            cJSON_AddStringToObject(root, "type", "echo");
            printf("Igresa el mensaje a enviar: \n");
            fgets(input, BUFFER_SIZE, stdin);
            input[strcspn(input, "\n")] = '\0'; // Eliminar el salto de línea al final del mensaje
            break;
        case 2:
            cJSON_AddStringToObject(root, "type", "time");
            stpcpy(input, ".");
            break;
        case 3:
            cJSON_AddStringToObject(root, "type", "upper");
            printf("Igresa el mensaje a convertir a mayusculas: \n");
            fgets(input, BUFFER_SIZE, stdin);
            input[strcspn(input, "\n")] = '\0'; // Eliminar el salto de línea al final del mensaje
            break;
        default:
            printf("Opcion no valida\n");
            return 1;

    }
    cJSON_AddStringToObject(root, "payload", input);
    //printf("Ingresa el mensaje para enviar: \n");
    //fgets(input, BUFFER_SIZE, stdin);
    //input[strcspn(input, "\n")] = '\0'; // Eliminar el salto de línea al final del mensaje
    char *json_str = cJSON_PrintUnformatted(root);

    // Enviar mensaje al servidor
    ssize_t bytes_sent = send(sock, json_str, strlen(json_str), 0);
    if(bytes_sent < 0){
        perror("send");
        close(sock);
        return 1;
    }
    printf("Mensaje enviado: %s\n", json_str);

    cJSON_Delete(root); // Liberar memoria del objeto cJSON
    free(json_str); // Liberar memoria de la cadena JSON

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
    printf("Respuesta cruda: %s\n", buffer);

    cJSON* response = cJSON_Parse(buffer);
    if(response == NULL){
        printf("Error al parsear la respuesta JSON\n");
        close(sock);
        return 1;
    }
    cJSON* status = cJSON_GetObjectItem(response, "status");
    if(cJSON_IsString(status)){
        printf("Status: %s\n", status->valuestring);
    } else {
        printf("Campo 'status' no encontrado o no es una cadena\n");
    }
    cJSON* payload = cJSON_GetObjectItem(response, "payload");
    if(cJSON_IsString(payload)){
        printf("Payload: %s\n", payload->valuestring);
    } else {
        printf("Campo 'payload' no encontrado o no es una cadena\n");
    }

    cJSON_Delete(response); // Liberar memoria del objeto cJSON
    close(sock); // Cerrar el socket después de recibir la respuesta
    return 0;


}