#include "strategy/time_strategy.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <ctime>
#include <string>

/**
 * Implementación del método processMessage de TimeStrategy.
 * Este método obtiene la hora actual del sistema, la convierte a una cadena legible y la envía al cliente.
 */
void TimeStrategy::processMessage(int client_fd, const std::string& message) {
    time_t now = time(nullptr);
    std::string timeStr = ctime(&now);
    send(client_fd, timeStr.c_str(), timeStr.size(), 0);    
}