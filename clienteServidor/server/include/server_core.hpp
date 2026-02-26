#ifndef SERVER_CORE_HPP
#define SERVER_CORE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

// Declaracion de la clase ServerCore
class ServerCore {
public:
// Constructor y metodos publicos
    ServerCore(int port,int poolSize);
    void run();
    void stop();

private:
// Atributos privados
    int port;
    int poolSize;
    bool running;

    int server_fd;

    std::queue<int> clientQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::vector<std::thread> workers;

// Metodos privados
    void workerLoop();
    void handleClient(int client_fd);
};
#endif // SERVER_CORE_HPP