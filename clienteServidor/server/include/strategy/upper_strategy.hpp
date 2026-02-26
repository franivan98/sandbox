#ifndef UPPER_STRATEGY_HPP
#define UPPER_STRATEGY_HPP

#include "message_processor.hpp"


class UpperStrategy : public MessageProcessor {
public:
    json processMessage(int client_fd, const json& message) override;
};

#endif