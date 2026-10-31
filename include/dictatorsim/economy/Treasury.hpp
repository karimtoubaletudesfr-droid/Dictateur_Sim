#pragma once
#include "Ledger.hpp"
namespace ds {
    class Treasury {
        AccountId account_;
        public:
        explicit Treasury(AccountId account);
        AccountId account() const;
        Money balance(const Ledger& ledger) const;
        bool pay(Ledger& ledger, AccountId to, Money amount, TransactionType type, std::uint64_t day) const;
    };
}
