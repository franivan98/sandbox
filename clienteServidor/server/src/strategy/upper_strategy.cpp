#include "strategy/upper_strategy.hpp"
#include <algorithm>

/**
 * Implementación del método processMessage de UpperStrategy.
 * Este método convierte el mensaje recibido a mayúsculas y lo envía de vuelta al cliente.
 */
json UpperStrategy::processMessage(int client_fd, const json& request) {
   if(!request.contains("payload") || !request["payload"].is_string()) {
        return{
            {"status", "error"},
            {"message", "Invalid request: 'payload' field is required and must be a string."}
        };
    }

    std::string payload = request["payload"];
    std::transform(payload.begin(), payload.end(), payload.begin(), ::toupper);
    return {
        {"status", "success"},
        {"payload", payload}
    };
}