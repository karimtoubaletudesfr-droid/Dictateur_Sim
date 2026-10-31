#include "dictatorsim/economy/Ledger.hpp"
#include <limits>
#include <stdexcept>
namespace ds {
    AccountId Ledger::openAccount(Money initial, const std::string& owner) {
        if(sealed_) throw std::logic_error("opening allocation is sealed");
        if(initial<0 || initial>std::numeric_limits<Money>::max()-initial_) throw std::overflow_error("opening money overflow");
        if(accounts_.size()>=std::numeric_limits<AccountId>::max()) throw std::overflow_error("account IDs exhausted");
        auto id=static_cast<AccountId>(accounts_.size());
        accounts_.emplace_back(id,initial,owner);
        initial_+=initial;
        return id;
    }
    void Ledger::seal() {
        sealed_=true;
    }
    bool Ledger::transfer(AccountId from, AccountId to, Money n, TransactionType type, std::uint64_t day) {
        if(!sealed_) throw std::logic_error("seal opening allocation first");
        if(type==TransactionType::MonetaryCreation || type==TransactionType::MonetaryDestruction) throw std::logic_error("monetary policy disabled");
        TransactionStatus s=TransactionStatus::Accepted;
        if(n<=0) s=TransactionStatus::InvalidAmount;
        else if(from>=accounts_.size() || to>=accounts_.size()) s=TransactionStatus::UnknownAccount;
        else if(from==to) s=TransactionStatus::SameAccount;
        else if(accounts_[from].balance_<n) s=TransactionStatus::InsufficientFunds;
        else if(accounts_[to].balance_>std::numeric_limits<Money>::max()-n) s=TransactionStatus::Overflow;
        // Allocate the journal entry before changing balances: allocation failure is atomic.
        if(archive_.is_open() && journal_.size()>=100000)journal_.clear();
        journal_.push_back({nextTransaction_,day,from,to,n,type,s});
        if(archive_.is_open()) {
            archive_<<nextTransaction_<<','<<day<<','<<from<<','<<to<<','<<n<<','<<static_cast<int>(type)<<','<<static_cast<int>(s)<<'\n';
            if(!archive_) {
                journal_.pop_back();
                throw std::runtime_error("transaction archive write failed");
            }
        }
        ++nextTransaction_;
        if(s!=TransactionStatus::Accepted) return false;
        accounts_[from].balance_-=n;
        accounts_[to].balance_+=n;
        return true;
    }
    Money Ledger::balance(AccountId id) const {
        return accounts_.at(id).balance_;
    }
    Money Ledger::moneySupply() const {
        Money sum=0;
        for(const auto& a:accounts_) {
            if(a.balance_<0 || a.balance_>std::numeric_limits<Money>::max()-sum) throw std::overflow_error("invalid monetary aggregate");
            sum+=a.balance_;
        }
        return sum;
    }
    Money Ledger::initialSupply() const {
        return initial_;
    }
    bool Ledger::conserved() const {
        return moneySupply()==initial_;
    }
    const std::vector<Transaction>& Ledger::transactions() const {
        return journal_;
    }
    const std::vector<Account>& Ledger::accounts() const {
        return accounts_;
    }
    void Ledger::monetaryOperation(Money) {
        throw std::logic_error("V1 monetary policy disabled");
    }
}
namespace ds {
    void Ledger::archiveTo(const std::string& path) {
        if(nextTransaction_!=0 || archive_.is_open())throw std::logic_error("archive must start before first transfer");
        archive_.open(path);
        if(!archive_)throw std::runtime_error("cannot create transaction archive");
        archive_<<"id,day,source,destination,amount_cents,type,status\n";
    }
}
namespace ds {
    void Ledger::flushArchive() {
        if(archive_.is_open()) {
            archive_.flush();
            if(!archive_)throw std::runtime_error("transaction archive flush failed");
        }
    }
}
