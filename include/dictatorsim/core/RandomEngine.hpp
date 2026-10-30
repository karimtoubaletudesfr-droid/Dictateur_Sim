#pragma once 
#include <cstdint>
#include <random>
#include <vector>

namespace ds{
    class RandomEngine{
        private:
        std::mt19937 engin;
        public:
            // pour force la const de l'objet et eviter RandomeEngin e =42 
            explicit RandomEngine(std::uint32_t seed);
            int randomInt(int min , int max);
            float randomFloat(float min, float max);
            bool probability(double p);
            std::size_t weightedChoice(
                const std::vector<double>& weights
            );

    };
}
