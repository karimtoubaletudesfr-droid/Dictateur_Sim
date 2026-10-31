#include "dictatorsim/politics/Government.hpp"
namespace ds {
    Government::Government(int tax,int welfare,double police) {
        policy.incomeTax=tax;
        policy.welfareRate=welfare;
        policy.police=police;
    }
}
