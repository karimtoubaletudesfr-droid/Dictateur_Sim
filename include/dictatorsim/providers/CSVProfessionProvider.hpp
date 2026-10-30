#pragma once

#include <string>

#include "dictatorsim/providers/IProfessionProvider.hpp"

namespace ds {

class CSVProfessionProvider : public IProfessionProvider {
private:
    std::string filePath;

public:
    explicit CSVProfessionProvider(const std::string& filePath);

    std::vector<ProfessionConfig> load() const override;
};

}