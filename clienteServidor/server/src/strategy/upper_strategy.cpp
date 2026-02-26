#include "strategy/upper_strategy.hpp"
#include <sys/socket.h>

/**
 * Implementación del método processMessage de UpperStrategy.
 * Este método convierte el mensaje recibido a mayúsculas y lo envía de vuelta al cliente.
 */
void UpperStrategy::processMessage(int client_fd, const std::string& message) {
    std::string upperMessage;
    for(char c : message){
        upperMessage += std::toupper(c);
    }
    send(client_fd, upperMessage.c_str(), upperMessage.size(), 0);
}