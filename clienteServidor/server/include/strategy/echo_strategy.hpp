#ifndef ECHO_STRATEGY_HPP
#define ECHO_STRATEGY_HPP

#include "message_processor.hpp"

/*
    * EchoStrategy es una implementación concreta de MessageProcessor que simplemente devuelve el mensaje recibido.
    * Este es un ejemplo básico de una estrategia de procesamiento de mensajes, y se puede extender para incluir lógica más compleja según sea necesario.
*/
class EchoStrategy : public MessageProcessor {
public:
    void processMessage(int client_fd, const std::string& message) override;
};

#endif