#include "strategy/strategy_factory.hpp"
#include "strategy/upper_strategy.hpp"
#include "strategy/time_strategy.hpp"
#include "strategy/echo_strategy.hpp"

std::unique_ptr<MessageProcessor> StrategyFactory::create(const json& message)  {
    
    if(!message.contains("type") || !message["type"].is_string()) {
        return std::make_unique<EchoStrategy>();
    }

    std::string type = message["type"];
    if(type == "upper") {
        return std::make_unique<UpperStrategy>();
    } else if(type == "time") {
        return std::make_unique<TimeStrategy>();
    } else {
        return std::make_unique<EchoStrategy>();
    }
}