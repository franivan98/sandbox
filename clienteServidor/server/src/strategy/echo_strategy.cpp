#include "strategy/echo_strategy.hpp"
#include <sys/socket.h>
#include <unistd.h>

/**
 * Implementación del método processMessage de EchoStrategy.
 * Este método recibe un mensaje del cliente y lo envía de vuelta, actuando como un "echo" del mensaje recibido.
 */
void EchoStrategy::processMessage(int client_fd, const std::string& message) {
    send(client_fd, message.c_str(), message.size(), 0);
}