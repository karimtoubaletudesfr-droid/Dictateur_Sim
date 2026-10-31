#include "TestWorld.hpp"
#include "dictatorsim/providers/CSVProfessionProvider.hpp"
#include <limits>
#include <fstream>
#include <stdexcept>
int main() {
    ds::SimulationConfig c;
    c.load("configs");
    assert(c.population==1000 && c.regionalRisks[1][1]==0);
    c.detection=std::numeric_limits<double>::quiet_NaN();
    bool bad=false;
    try {
        c.validate();
    }
    catch(const std::invalid_argument&) {
        bad=true;
    }
    assert(bad);
    auto jobs=ds::CSVProfessionProvider("configs/professions.csv").loadWithEconomics("configs/profession-economics.csv");
    bool researcher=false;
    for(const auto& j:jobs)if(j.profession==ds::Profession::Researcher) {
        assert(j.referenceSalary==90000 && j.mentalLoad>j.physicalLoad);
        researcher=true;
    }
    assert(researcher);
    ds::SimulationConfig overflow;
    overflow.citizenShare=std::numeric_limits<int>::max();
    overflow.companyShare=std::numeric_limits<int>::max();
    bad=false;
    try {
        overflow.validate();
    }
    catch(const std::invalid_argument&) {
        bad=true;
    }
    assert(bad);
    {
        std::ofstream out("build/invalid-profession.csv");
        out<<"profession,min_age,max_age,weight\nFarmer,0oops,120,1\n";
    }
    bad=false;
    try {
        ds::CSVProfessionProvider("build/invalid-profession.csv").load();
    }
    catch(const std::runtime_error&) {
        bad=true;
    }
    assert(bad);
    ds::RandomEngine r(42);
    for(auto weights:{std::vector<double>{0,0},std::vector<double>{1,-1}}) {
        bad=false;
        try {
            r.weightedChoice(weights);
        }
        catch(const std::invalid_argument&) {
            bad=true;
        }
        assert(bad);
    }
}
