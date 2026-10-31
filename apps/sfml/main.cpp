#include "dictatorsim/ui/GameApplication.hpp"
#include <iostream>
#include <stdexcept>
int main(int argc,char** argv){try{
    ds::SimulationConfig config;config.load("configs");config.ledgerArchive="build/player-transactions.csv";
    std::string font="/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
    unsigned smoke=0,width=1600,height=900;bool benchmark=false;
    for(int i=1;i<argc;++i){std::string key=argv[i];if(key=="--help"){std::cout<<"--population N --seed N --font FILE --smoke FRAMES --benchmark FRAMES --width N --height N\n";return 0;}if(++i>=argc)throw std::invalid_argument("missing option value");std::string value=argv[i];
        if(key=="--font"){font=value;continue;}
        std::size_t end=0;auto n=std::stoul(value,&end);if(end!=value.size() || value[0]=='-' || n>1000000)throw std::invalid_argument("invalid option value");
        if(key=="--population")config.population=n;else if(key=="--seed")config.seed=n;else if(key=="--smoke")smoke=n;else if(key=="--benchmark"){smoke=n;benchmark=true;}else if(key=="--width")width=n;else if(key=="--height")height=n;else throw std::invalid_argument("unknown option "+key);
    }
    if(width<640 || height<360 || width>7680 || height>4320 || (smoke && smoke<60))throw std::invalid_argument("invalid dimensions or smoke duration");
    config.validate();return ds::GameApplication(config,font).run(smoke,width,height,benchmark);
}catch(const std::exception& e){std::cerr<<"DictatorSim SFML: "<<e.what()<<'\n';return 1;}}
