#ifndef MESSAGE_PROCESSOR_HPP
#define MESSAGE_PROCESSOR_HPP

#include <string>

/**
 * MessageProcessor es una clase abstracta que define la interfaz para procesar mensajes recibidos de los clientes.
 * Las clases que hereden de MessageProcessor deben implementar el método processMessage, que se encargará de manejar la lógica específica para procesar el mensaje recibido y enviar una respuesta al cliente.
 */
class MessageProcessor {
public:
    // Destructor virtual para permitir la herencia
    virtual ~MessageProcessor() = default;
    // Método puro virtual que debe ser implementado por las clases derivadas
    virtual void processMessage(int client_fd, const std::string& message) = 0;
};
#endif