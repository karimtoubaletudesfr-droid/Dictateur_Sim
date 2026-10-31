#pragma once
#include "Account.hpp"
#include "Transaction.hpp"
#include <vector>
#include <fstream>
namespace ds {
    // Owns every monetary balance. No business system can mint money.
    class Ledger {
        std::vector<Account> accounts_;
        std::vector<Transaction> journal_;
        Money initial_ = 0;
        bool sealed_ = false;
        std::ofstream archive_;
        std::uint64_t nextTransaction_ = 0;
        // Reserved for a future audited monetary authority; deliberately inaccessible.
        void monetaryOperation(Money amount);
        public:
        AccountId openAccount(Money initial, const std::string& owner);
        void seal();
        void archiveTo(const std::string& path);
        void flushArchive();
        bool transfer(AccountId from, AccountId to, Money amount, TransactionType type, std::uint64_t day);
        Money balance(AccountId id) const;
        Money moneySupply() const;
        Money initialSupply() const;
        bool conserved() const;
        const std::vector<Transaction>& transactions() const;
        const std::vector<Account>& accounts() const;
    };
}
