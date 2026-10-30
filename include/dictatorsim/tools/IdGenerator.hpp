#pragma once 
#include<cstdint>
namespace ds{
    class IdGenerator{
        private: 
        std::uint32_t nextId;
        public:
        IdGenerator();
        std::uint32_t generate();
    };
}