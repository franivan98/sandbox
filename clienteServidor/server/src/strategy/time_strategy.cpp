#include "strategy/time_strategy.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>
#include <string>

/**
 * Implementación del método processMessage de TimeStrategy.
 * Este método obtiene la hora actual del sistema, la convierte a una cadena legible y la envía al cliente.
 */
json TimeStrategy::processMessage(int client_fd, const json& message) {
    time_t now = time(nullptr);
    std::string timeStr = ctime(&now);
    return {
        {"status", "success"},
        {"payload", timeStr}
    };
}