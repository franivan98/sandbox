#ifndef SERVER_FACADE_HPP
#define SERVER_FACADE_HPP

class ServerFacade {
public:
    explicit ServerFacade(int port);
    void start();
    void stop();

private:
    class ServerCore* core;
};
#endif // SERVER_FACADE_HPP