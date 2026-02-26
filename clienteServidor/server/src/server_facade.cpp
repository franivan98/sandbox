#include "server_facade.hpp"
#include "server_core.hpp"

ServerFacade::ServerFacade(int port) {
    core = new ServerCore(port, 4); // Pool de 4 hilos
}

void ServerFacade::start() {
    core->run();
}

void ServerFacade::stop() {
    core->stop();
    delete core;
}