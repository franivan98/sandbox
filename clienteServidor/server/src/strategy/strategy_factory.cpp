#include "strategy/strategy_factory.hpp"
#include "strategy/upper_strategy.hpp"
#include "strategy/time_strategy.hpp"
#include "strategy/echo_strategy.hpp"

std::unique_ptr<MessageProcessor> StrategyFactory::create(const std::string& message)  {
    if(message == "TIME"){
        return std::make_unique<TimeStrategy>();
    } else if(message == "upper"){
        return std::make_unique<UpperStrategy>();
    } else {
        return std::make_unique<EchoStrategy>();
    }
}