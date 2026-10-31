#include "dictatorsim/providers/CSVProfessionProvider.hpp"

#include <fstream>
#include <algorithm>
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
                std::size_t parsed=0;
                const int minAge = std::stoi(minAgeText,&parsed);
                if(parsed!=minAgeText.size())throw std::runtime_error("invalid minimum age");
                const int maxAge = std::stoi(maxAgeText,&parsed);
                if(parsed!=maxAgeText.size())throw std::runtime_error("invalid maximum age");
                if(!weightText.empty() && weightText.back()=='\r')weightText.pop_back();
                const double weight = std::stod(weightText,&parsed);
                if(parsed!=weightText.size())throw std::runtime_error("invalid weight text");

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

namespace ds {
std::vector<ProfessionConfig> CSVProfessionProvider::loadWithEconomics(const std::string& path)const {
 auto configs=load();std::ifstream in(path);if(!in)throw std::runtime_error("missing profession economics: "+path);
 std::string line;std::getline(in,line);
 if(line!="profession,salary,sector,productivity,physical_load,mental_load,hours,skill")throw std::runtime_error("invalid profession economics header");
 std::vector<Profession> seen;
 while(std::getline(in,line)) {
  if(!line.empty() && line.back()=='\r')line.pop_back();
  if(line.empty())continue;
  std::stringstream ss(line);std::vector<std::string> fields;std::string value;while(std::getline(ss,value,','))fields.push_back(value);
  if(fields.size()!=8)throw std::runtime_error("invalid profession economics row");
  auto profession=parseProfession(fields[0]);
  if(std::find(seen.begin(),seen.end(),profession)!=seen.end())throw std::runtime_error("duplicate profession economics");
  seen.push_back(profession);
  auto it=std::find_if(configs.begin(),configs.end(),[&](const auto& c){return c.profession==profession;});if(it==configs.end())throw std::runtime_error("unknown configured profession");
  std::size_t end=0;auto salary=std::stoll(fields[1],&end);if(end!=fields[1].size() || salary<0 || salary>1000000000)throw std::runtime_error("invalid salary");it->referenceSalary=salary;
  int sector=std::stoi(fields[2],&end);if(end!=fields[2].size()||sector<0||sector>5)throw std::runtime_error("invalid sector");it->sector=static_cast<Sector>(sector);
  double* destinations[]={&it->productivity,&it->physicalLoad,&it->mentalLoad,&it->workHours,&it->skill};
  for(int i=0;i<5;++i){double n=std::stod(fields[i+3],&end);if(end!=fields[i+3].size()||!std::isfinite(n)||n<0||n>24)throw std::runtime_error("invalid profession coefficient");*destinations[i]=n;}
 }
 if(seen.size()!=configs.size())throw std::runtime_error("missing profession economics row");
 return configs;
}
}
