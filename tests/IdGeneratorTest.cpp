#include "dictatorsim/tools/IdGenerator.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

void testSequentialIds() {
    ds::IdGenerator generator;

    assert(generator.generate() == 0);
    assert(generator.generate() == 1);
    assert(generator.generate() == 2);
}

void testIndependentGenerators() {
    ds::IdGenerator first;
    ds::IdGenerator second;

    assert(first.generate() == 0);
    assert(first.generate() == 1);

    assert(second.generate() == 0);
    assert(second.generate() == 1);
}

void testUniqueIds() {
    ds::IdGenerator generator;

    constexpr std::uint32_t count = 100000;

    for (std::uint32_t i = 0; i < count; ++i) {
        assert(generator.generate() == i);
    }
}

int main() {
    testSequentialIds();
    testIndependentGenerators();
    testUniqueIds();

    std::cout << "All IdGenerator tests passed!\n";

    return 0;
}