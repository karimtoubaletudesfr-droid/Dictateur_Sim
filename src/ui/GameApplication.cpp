#include "dictatorsim/ui/GameApplication.hpp"
#include <iostream>
namespace ds {
GameApplication::GameApplication(SimulationConfig c,const std::string& font):config_(c),theme_(font),session_(std::make_unique<PlayerSession>(c)){theme_.setButtonSound([this]{audio_.playButton();});}
int GameApplication::run(unsigned smokeFrames,unsigned width,unsigned height,bool benchmark){
    sf::RenderWindow window(sf::VideoMode(width,height),"DictatorSim - Player Government",sf::Style::Default);
    audio_.load("assets/sounds");
    window.setFramerateLimit(60);sf::View view(sf::FloatRect(0,0,1280,720));window.setView(view);
    if(benchmark)session_->setSpeed(SimulationSpeed::Fastest);
    const auto smokeFarmerSalary=session_->simulation().world().config.salaries[3];
    sf::Clock clock,performance;unsigned frames=0;bool menuOpen=false,settingsOpen=false;double computeSeconds=0;
    while(window.isOpen()){
        double dt=clock.restart().asSeconds();bool click=false;
        bool mouseHeld=window.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left);sf::Vector2f mouse=window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event{};
        while(window.pollEvent(event)){
            if(settingsOpen)settings_.event(event);
            if(event.type==sf::Event::Closed)window.close();
            if(event.type==sf::Event::Resized){
                float aspect=event.size.height?float(event.size.width)/event.size.height:16.f/9.f;sf::FloatRect viewport(0,0,1,1);
                if(aspect>16.f/9.f){viewport.width=(16.f/9.f)/aspect;viewport.left=(1-viewport.width)/2;}else{viewport.height=aspect/(16.f/9.f);viewport.top=(1-viewport.height)/2;}
                view.setViewport(viewport);window.setView(view);
            }
            if(event.type==sf::Event::MouseButtonPressed && event.mouseButton.button==sf::Mouse::Left){click=true;mouse=window.mapPixelToCoords({event.mouseButton.x,event.mouseButton.y});}
            if(event.type==sf::Event::MouseWheelScrolled && !menuOpen && !settingsOpen && !session_->request() && !dashboard_.confirming())dashboard_.scroll(mouse,event.mouseWheelScroll.delta);
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){if(settingsOpen)settingsOpen=false;else menuOpen=!menuOpen;session_->setSpeed(SimulationSpeed::Pause);}
                if(event.key.code==sf::Keyboard::P && !settingsOpen && !session_->lost() && !session_->request() && !dashboard_.confirming()){settings_.open(*session_);settingsOpen=true;menuOpen=false;session_->setSpeed(SimulationSpeed::Pause);}
                if(event.key.code==sf::Keyboard::Space && !menuOpen && !settingsOpen && !session_->request() && !dashboard_.confirming())session_->setSpeed(session_->speed()==SimulationSpeed::Pause?SimulationSpeed::Normal:SimulationSpeed::Pause);
            }
        }
        if(!window.isOpen())break;
        // Explicit opt-in UI smoke harness; never active in normal player mode.
        if(smokeFrames && !benchmark){
            if(frames==5){mouse={665,44};click=true;} // 1x button
            if(frames==40){mouse={584,44};click=true;} // pause button
            if(frames==42){mouse={520,582};click=true;}
            if(frames==45){mouse={620,510};click=true;} // review tax decision
            if(frames==48){mouse={740,474};click=true;}
            if(frames==65)session_->notify({0,session_->simulation().world().clock.day(),EventSeverity::Major,EventCategory::Disaster,"SMOKE TEST: severe drought - choose a response",true,false});
            if(frames==75){mouse={600,448};click=true;}
            if(frames==85){mouse={540,128};click=true;}
            if(frames==90)dashboard_.scroll({500,300},-1);
            if(frames==105){mouse={1150,44};click=true;}
            if(frames==108){mouse={600,326};click=true;}
            if(frames==112){mouse={1070,44};click=true;}
            if(frames==114){mouse={665,140};click=true;}
            if(frames==116){mouse={753,140};click=true;}
            if(frames==118){mouse={841,140};click=true;}
            if(frames==120){mouse={929,140};click=true;}
            if(frames==121){mouse={1017,140};click=true;}
            if(frames==122){settings_.open(*session_);settingsOpen=true;}
            if(frames==125){mouse={1100,175};click=true;}
            if(frames==128){mouse={980,657};click=true;}
            if(frames==135){mouse={900,132};click=true;}
            if(frames==142)settingsOpen=false;
            if(frames==143){mouse={1105,140};click=true;}
            if(frames==146){mouse={1193,140};click=true;}
            if(frames==149){mouse={1017,140};click=true;}
            if(frames==152){mouse={570,690};click=true;}
            if(frames==162){mouse={150,274};click=true;}
            if(frames==172){mouse={150,326};click=true;}
            if(frames==182){mouse={150,378};click=true;}
            if(frames==192){mouse={150,430};click=true;}
            if(frames==202){mouse={150,482};click=true;}
            if(frames==212){mouse={80,690};click=true;}
            if(frames==232){settings_.open(*session_);settingsOpen=true;}
            if(frames==234){mouse={1100,132};click=true;}
            if(frames==236){mouse={220,305};click=true;}
            if(frames==238){mouse={220,455};click=true;}
            if(frames==240){mouse={430,305};click=true;}
            if(frames==242){mouse={850,455};click=true;}
            if(frames==244){mouse={640,305};click=true;}
            if(frames==245){mouse={1000,305};mouseHeld=true;}
            if(frames==248){mouse={430,305};click=true;} // confirm through UI pipeline
        }
        sf::Clock logical;
        if(!menuOpen && !settingsOpen && !dashboard_.confirming())session_->advance(dt);
        computeSeconds+=logical.getElapsedTime().asSeconds();
        window.clear(theme_.background);
        if(dashboard_.draw(window,theme_,*session_,mouse,click,menuOpen||settingsOpen)){menuOpen=true;session_->setSpeed(SimulationSpeed::Pause);click=false;}
        if(settingsOpen){
            auto choice=settings_.draw(window,theme_,*session_,audio_,mouse,click,mouseHeld);
            if(choice==SettingsChoice::Close)settingsOpen=false;
            if(choice==SettingsChoice::Restart){config_=settings_.draft();session_=std::make_unique<PlayerSession>(config_);dashboard_=DashboardScreen{};settingsOpen=false;clock.restart();}
        }
        if(menuOpen || session_->lost()){
            auto choice=menu_.draw(window,theme_,mouse,click,session_->lost());
            if(choice==MenuChoice::Settings){settings_.open(*session_);settingsOpen=true;menuOpen=false;}
            if(choice==MenuChoice::Return)menuOpen=false;
            if(choice==MenuChoice::Quit)window.close();
            if(choice==MenuChoice::NewGame){config_=session_->simulation().world().config;session_=std::make_unique<PlayerSession>(config_);dashboard_=DashboardScreen{};menuOpen=false;clock.restart();}
        }
        window.display();++frames;
        if(smokeFrames && (frames==60 || frames==70 || frames==100 || frames==120 || frames==130 || frames==140 || frames==145 || frames==148 || frames==151 || frames==160 || frames==170 || frames==180 || frames==190 || frames==200 || frames==210 || frames==220 || frames==250)){
            sf::Texture image;image.create(window.getSize().x,window.getSize().y);image.update(window);
            image.copyToImage().saveToFile("build/sfml-"+std::to_string(frames)+".png");
        }
        if(smokeFrames && frames>=smokeFrames){
            sf::Texture capture;capture.create(window.getSize().x,window.getSize().y);capture.update(window);
            capture.copyToImage().saveToFile("build/sfml-smoke.png");
            std::cout<<"SFML smoke: frames="<<frames<<" seconds="<<performance.getElapsedTime().asSeconds()<<" logical_seconds="<<computeSeconds<<" decisions="<<session_->decisions().history().records().size()<<" tax="<<session_->simulation().world().government.policy.incomeTax<<" day="<<session_->simulation().world().clock.day()<<" conserved="<<session_->simulation().world().ledger.conserved()<<'\n';
            if(!benchmark && smokeFrames>=250){
                std::cout<<"Audio smoke: music_loaded="<<audio_.musicReady()<<" effects_loaded="<<audio_.effectsReady()<<" music_volume="<<audio_.musicVolume()<<" effects_volume="<<audio_.effectsVolume()<<" playing="<<audio_.musicPlaying()<<'\n';
                if(!audio_.musicReady() || !audio_.effectsReady() || !audio_.musicPlaying() || audio_.musicVolume()!=25 || audio_.effectsVolume()!=75)return 4;
            }
            window.close();
            if(!benchmark && smokeFrames>=142 && session_->simulation().world().config.salaries[3]!=smokeFarmerSalary+5000)return 3;
            if(!benchmark && (session_->decisions().history().records().empty() || session_->simulation().world().government.policy.incomeTax!=config_.taxRate+500 || session_->request() || session_->speed()!=SimulationSpeed::Pause))return 2;
        }
    }
    return 0;
}
}
