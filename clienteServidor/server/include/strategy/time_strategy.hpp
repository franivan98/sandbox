#ifndef TIME_STRATEGY_HPP
#define TIME_STRATEGY_HPP

#include "message_processor.hpp"

class TimeStrategy : public MessageProcessor {
public:
    void processMessage(int client_fd, const std::string& message) override;
};

#endif