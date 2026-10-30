#include "dictatorsim/providers/CSVProfessionProvider.hpp"

#include <cassert>
#include <iostream>

int main() {

    ds::CSVProfessionProvider provider(
        "configs/professions.csv"
    );

    const auto configs = provider.load();

    assert(configs.size() == 11);

    bool doctorFound = false;
    bool retiredFound = false;

    for (const auto& config : configs) {

        assert(config.minAge <= config.maxAge);
        assert(config.weight >= 0.0);

        if (config.profession == ds::Profession::Doctor) {
            doctorFound = true;

            assert(config.minAge == 26);
            assert(config.maxAge == 70);
            assert(config.weight == 7.0);
        }

        if (config.profession == ds::Profession::Retired) {
            retiredFound = true;

            assert(config.minAge == 65);
        }
    }

    assert(doctorFound);
    assert(retiredFound);

    std::cout << "All CSVProfessionProvider tests passed!\n";

    return 0;
}