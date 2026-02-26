#ifndef TIME_STRATEGY_HPP
#define TIME_STRATEGY_HPP

#include "message_processor.hpp"

class TimeStrategy : public MessageProcessor {
public:
    json processMessage(int client_fd, const json& message) override;
};

#endif