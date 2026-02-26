#ifndef STRATEGY_FACTORY_HPP
#define STRATEGY_FACTORY_HPP

#include <memory>
#include <string>
#include "message_processor.hpp"

/**
 * StrategyFactory es una clase que se encarga de crear instancias de MessageProcessor basándose en el contenido del mensaje recibido.
 * El método estático create recibe un mensaje y devuelve un puntero único a una instancia de MessageProcessor que implementa la lógica 
 * específica para procesar ese mensaje. Esto permite una fácil extensión del sistema para manejar diferentes tipos de mensajes sin modificar el código existente, siguiendo el principio de abierto/cerrado.
 */
class StrategyFactory {
public:
    static std::unique_ptr<MessageProcessor> create(const json& message);
};

#endif