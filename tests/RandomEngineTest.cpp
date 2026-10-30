#include "dictatorsim/core/RandomEngine.hpp"

#include <cassert>
#include <iostream>
#include <vector>
#include <stdexcept>

int main() {
    ds::RandomEngine random(42);

    // Test randomInt
    for (int i = 0; i < 1000; ++i) {
        int value = random.randomInt(1, 100);
        assert(value >= 1 && value <= 100);
    }

    // Test randomFloat
    for (int i = 0; i < 1000; ++i) {
        float value = random.randomFloat(0.0f, 1.0f);
        assert(value >= 0.0f && value < 1.0f);
    }

    // Test probability
    assert(random.probability(0.0) == false);
    assert(random.probability(1.0) == true);

    // Test weightedChoice
    std::vector<double> weights = {0.0, 0.0, 1.0};

    for (int i = 0; i < 100; ++i) {
        assert(random.weightedChoice(weights) == 2);
    }

    // Test reproducibility
    ds::RandomEngine first(123);
    ds::RandomEngine second(123);

    for (int i = 0; i < 100; ++i) {
        assert(first.randomInt(0, 1000) ==
               second.randomInt(0, 1000));
    }

    // Test invalid probability
    bool exceptionCaught = false;

    try {
        random.probability(1.5);
    } catch (const std::invalid_argument&) {
        exceptionCaught = true;
    }

    assert(exceptionCaught);

    std::cout << "All RandomEngine tests passed!\n";

    return 0;
}