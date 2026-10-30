#include "dictatorsim/core/RandomEngine.hpp"
#include <stdexcept>
#include <cmath>

namespace ds{
    RandomEngine::RandomEngine(std::uint32_t seed): engin(seed)
    {
    }
    int RandomEngine::randomInt(int min, int max){
        if(min>max){
            throw std::invalid_argument("Invalid integer range");
        }
        std::uniform_int_distribution<int> distribution(min,max);
        return distribution(engin);
    }
    float RandomEngine::randomFloat(float min , float max){
        if(!(min<max)){
            throw std::invalid_argument("Invalid float range");
        }
        std::uniform_real_distribution<float> distribution(min,max);
        return distribution(engin);
    }
    bool RandomEngine::probability(double p){
        if(!(p>=0.0 && p<=1.0)){
            throw std::invalid_argument("Invalid probability value it must be between 0.0 and 1.0");
        }
        std::bernoulli_distribution distribution(p);
        return distribution(engin);
    }
    std::size_t RandomEngine::weightedChoice(const std::vector<double>& weights){
        if(weights.empty()){
            throw std::invalid_argument("Weights vector is empty");
        }
        std::discrete_distribution<std::size_t> distribution(weights.begin(),weights.end());
        return distribution(engin);
    }
}
