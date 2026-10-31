#pragma once
#include "Resource.hpp"
#include "Ledger.hpp"
#include <vector>
namespace ds {
    class Company {
        public:
        std::uint32_t id;
        Sector sector;
        AccountId account;
        std::vector<std::uint32_t> employees;
        double productivity, capacity, stock=0;
        Money price;
        Money revenue=0;
        Company(std::uint32_t id, Sector sector, AccountId account, double productivity, Money price);
        double produce(double labor, double multiplier);
        bool sell(Ledger& ledger, AccountId buyer, std::uint64_t day, TransactionType type=TransactionType::Purchase);
        void damage(double fraction);
    };
}
