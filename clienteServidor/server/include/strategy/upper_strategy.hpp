#ifndef UPPER_STRATEGY_HPP
#define UPPER_STRATEGY_HPP

#include "message_processor.hpp"


class UpperStrategy : public MessageProcessor {
public:
    void processMessage(int client_fd, const std::string& message) override;
};

#endif