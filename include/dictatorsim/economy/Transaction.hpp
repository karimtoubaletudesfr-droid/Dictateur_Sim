#pragma once
#include "Money.hpp"
#include "TransactionType.hpp"
namespace ds {
    struct Transaction {
        std::uint64_t id, day;
        AccountId source, destination;
        Money amount;
        TransactionType type;
        TransactionStatus status;
    };
}
