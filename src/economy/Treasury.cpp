#include "dictatorsim/economy/Treasury.hpp"
namespace ds {
    Treasury::Treasury(AccountId a):account_(a) {
    }
    AccountId Treasury::account() const {
        return account_;
    }
    Money Treasury::balance(const Ledger& l) const {
        return l.balance(account_);
    }
    bool Treasury::pay(Ledger& l, AccountId to, Money n, TransactionType t, std::uint64_t day) const {
        return l.transfer(account_,to,n,t,day);
    }
}
