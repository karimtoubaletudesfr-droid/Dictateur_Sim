#pragma once
namespace ds {
    enum class TransactionType {
        Wage, Purchase, Tax, Repair, Medical, Reconstruction, Theft, Restitution, Fine, Subsidy, Contract, Corruption, MonetaryCreation, MonetaryDestruction
    };
    enum class TransactionStatus {
        Accepted, InvalidAmount, UnknownAccount, SameAccount, InsufficientFunds, Overflow
    };
}
