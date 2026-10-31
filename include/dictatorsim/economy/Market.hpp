#pragma once
#include "Company.hpp"
namespace ds {
    class Market {
        public: static Company* seller(std::vector<Company>& companies, Sector sector);
    };
}
