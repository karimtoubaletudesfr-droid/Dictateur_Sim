#include "dictatorsim/economy/Money.hpp"
#include <stdexcept>
namespace ds {
    Money fraction(Money n, int b) {
        if(n<0 || b<0 || b>10000) throw std::invalid_argument("invalid fraction");
        return (n/10000)*b + ((n%10000)*b)/10000;
    }
}
