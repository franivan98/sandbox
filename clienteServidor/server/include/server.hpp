#ifndef SERVER_HPP
#define SERVER_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

// server.hpp
// Declaración de la clase Server

class Server {
public:
    explicit Server(int port, int poolSize = 4);
    void start();

private:
    int port;
    int poolSize;

    int server_fd; // Socket del servidor

    std::queue<int> clientQueue; // Cola de clientes pendientes
    std::mutex queueMutex; // Mutex para proteger la cola
    std::condition_variable queueCV; // Variable de condición para notificar a los hilos trabajadores
    std::vector<std::thread> workers; // Hilos trabajadores
    bool running; // Flag para controlar la ejecución de los hilos trabajadores

    void workerLoop(); // Función que ejecutan los hilos trabajadores
    void handleClient(int client_fd);
};

#endif // SERVER_HPP