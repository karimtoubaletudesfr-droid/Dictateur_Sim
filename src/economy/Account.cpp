#include "dictatorsim/economy/Account.hpp"
#include <stdexcept>
#include <utility>
namespace ds {
    Account::Account(AccountId id, Money balance, std::string owner):id_(id),balance_(balance),owner_(std::move(owner)) {
        if(balance<0) throw std::invalid_argument("negative opening balance");
    }
    AccountId Account::id() const {
        return id_;
    }
    Money Account::balance() const {
        return balance_;
    }
    const std::string& Account::owner() const {
        return owner_;
    }
}
