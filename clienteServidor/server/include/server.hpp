#ifndef SERVER_HPP
#define SERVER_HPP

// server.hpp
// Declaración de la clase Server

class Server {
public:
    explicit Server(int port);
    void start();
private:
    int port;
};

#endif // SERVER_HPP