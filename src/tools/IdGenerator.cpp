#include "dictatorsim/tools/IdGenerator.hpp"
#include<limits>
#include<stdexcept>

namespace ds{
    IdGenerator::IdGenerator():nextId(0){}
    std::uint32_t IdGenerator::generate(){
        if(nextId==std::numeric_limits<std::uint32_t>::max()){
            throw std::overflow_error("ID limit reached");
        }
        return nextId++;
    }
}