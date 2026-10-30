#include "dictatorsim/providers/CSVProfessionProvider.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <limits>
#include <cmath>
namespace {

ds::Profession parseProfession(const std::string& name) {

    static const std::unordered_map<std::string, ds::Profession> professions = {
        {"Unemployed", ds::Profession::Unemployed},
        {"Student", ds::Profession::Student},
        {"Retired", ds::Profession::Retired},
        {"Farmer", ds::Profession::Farmer},
        {"Baker", ds::Profession::Baker},
        {"Teacher", ds::Profession::Teacher},
        {"Researcher", ds::Profession::Researcher},
        {"Engineer", ds::Profession::Engineer},
        {"Doctor", ds::Profession::Doctor},
        {"Worker", ds::Profession::Worker},
        {"Soldier", ds::Profession::Soldier}
    };

    const auto it = professions.find(name);

    if (it == professions.end()) {
        throw std::invalid_argument(
            "Unknown profession: " + name
        );
    }

    return it->second;
}

}
namespace ds {
    CSVProfessionProvider::CSVProfessionProvider(
            const std::string& filePath
        )
            : filePath(filePath)
    {
    };




    std::vector<ProfessionConfig> CSVProfessionProvider::load()const{
        std::ifstream file(filePath);
        if(!file.is_open()){
            throw std::runtime_error(
                "Cannot open CSV file: "+filePath   
            );
        }
            std::vector<ProfessionConfig> configs;

            std::string line; 

            // skip la premier ligne
            if(!std::getline(file,line)){
                throw std::runtime_error(
                    "CSV file is empty"
                );
            }
            if(
                line!="profession,min_age,max_age,weight"&&
                line != "profession,min_age,max_age,weight\r"
            ){
                throw std::runtime_error("Invalid CSV header");
            }



            std::size_t lineNumber = 1;



            while (std::getline(file, line)) {
                ++lineNumber; 

                if(line.empty()) continue;


                std::stringstream ss(line);
                std::string name;
                std::string minAgeText;
                std::string maxAgeText;
                std::string weightText;

                if (!std::getline(ss, name, ',') ||
                    !std::getline(ss, minAgeText, ',') ||
                    !std::getline(ss, maxAgeText, ',') ||
                    !std::getline(ss, weightText, ',')
                ){
                    throw std::runtime_error(
                        "Invalid CSV row at line " +
                        std::to_string(lineNumber)
                    );
                }
                // Check for unexpected extra columns.
                std::string extra;
                if (std::getline(ss, extra, ',')) {
                    throw std::runtime_error(
                        "Too many CSV columns at line " +
                        std::to_string(lineNumber)
                    );
                }


                // Validate numeric fields before narrowing.
                const int minAge = std::stoi(minAgeText);
                const int maxAge = std::stoi(maxAgeText);
                const double weight = std::stod(weightText);

                if (minAge < 0 || maxAge > 120 ||
                    minAge > maxAge || !std::isfinite(weight) ||
                    weight < 0.0
                ) {
                    throw std::runtime_error(
                        "Invalid profession parameters at line " +
                        std::to_string(lineNumber)
                    );
                }
                ProfessionConfig config {
                    parseProfession(name),
                    static_cast<std::uint8_t>(minAge),
                    static_cast<std::uint8_t>(maxAge),
                    weight
                };
                configs.push_back(config);
            }
            if(configs.empty()){
                throw std::runtime_error{
                    "CSV contains no professions"
                };
            }
            return configs;
    }

}
