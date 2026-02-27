#include "strategy/echo_strategy.hpp"
#include <sys/socket.h>
#include <unistd.h>

/**
 * Implementación del método processMessage de EchoStrategy.
 * Este método recibe un mensaje del cliente y lo envía de vuelta, actuando como un "echo" del mensaje recibido.
 */
json EchoStrategy::processMessage(int client_fd, const json& message) {
    if(!message.contains("payload") || !message["payload"].is_string()) {
        return{
            {"status", "error"},
            {"message", "Invalid request: 'payload' field is required and must be a string."}
        };
    }
    return {
        {"status", "success"},
        {"payload", message["payload"]}
    };
}