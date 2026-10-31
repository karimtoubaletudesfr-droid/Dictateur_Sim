#pragma once
#include "Money.hpp"
#include <string>
namespace ds {
    class Ledger;
    class Account {
        friend class Ledger;
        AccountId id_;
        Money balance_;
        std::string owner_;
        public:
        Account(AccountId id, Money balance, std::string owner);
        AccountId id() const;
        Money balance() const;
        const std::string& owner() const;
    };
}
