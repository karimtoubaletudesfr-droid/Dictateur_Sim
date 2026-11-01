#pragma once
#include "SimulationConfig.hpp"
#include <functional>
#include <vector>
namespace ds {
struct RuntimeParameter {
    std::string category, label, description, unit;
    double minimum, maximum, step;
    std::function<double(const SimulationConfig&)> get;
    std::function<void(SimulationConfig&,double)> set;
};
const std::vector<RuntimeParameter>& runtimeParameters();
}
