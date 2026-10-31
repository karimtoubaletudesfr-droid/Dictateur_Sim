#include "dictatorsim/ui/GameApplication.hpp"
#include <iostream>
namespace ds {
GameApplication::GameApplication(SimulationConfig c,const std::string& font):config_(c),theme_(font),session_(std::make_unique<PlayerSession>(c)){}
int GameApplication::run(unsigned smokeFrames,unsigned width,unsigned height,bool benchmark){
    sf::RenderWindow window(sf::VideoMode(width,height),"DictatorSim - Player Government",sf::Style::Default);
    window.setFramerateLimit(60);sf::View view(sf::FloatRect(0,0,1280,720));window.setView(view);
    if(benchmark)session_->setSpeed(SimulationSpeed::Fastest);
    sf::Clock clock,performance;unsigned frames=0;bool menuOpen=false;double computeSeconds=0;
    while(window.isOpen()){
        double dt=clock.restart().asSeconds();bool click=false;sf::Vector2f mouse=window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::Event event{};
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed)window.close();
            if(event.type==sf::Event::Resized){
                float aspect=event.size.height?float(event.size.width)/event.size.height:16.f/9.f;sf::FloatRect viewport(0,0,1,1);
                if(aspect>16.f/9.f){viewport.width=(16.f/9.f)/aspect;viewport.left=(1-viewport.width)/2;}else{viewport.height=aspect/(16.f/9.f);viewport.top=(1-viewport.height)/2;}
                view.setViewport(viewport);window.setView(view);
            }
            if(event.type==sf::Event::MouseButtonPressed && event.mouseButton.button==sf::Mouse::Left){click=true;mouse=window.mapPixelToCoords({event.mouseButton.x,event.mouseButton.y});}
            if(event.type==sf::Event::MouseWheelScrolled && !menuOpen && !session_->request() && !dashboard_.confirming())dashboard_.scroll(mouse,event.mouseWheelScroll.delta);
            if(event.type==sf::Event::KeyPressed){
                if(event.key.code==sf::Keyboard::Escape){menuOpen=!menuOpen;session_->setSpeed(SimulationSpeed::Pause);}
                if(event.key.code==sf::Keyboard::Space && !menuOpen && !session_->request() && !dashboard_.confirming())session_->setSpeed(session_->speed()==SimulationSpeed::Pause?SimulationSpeed::Normal:SimulationSpeed::Pause);
            }
        }
        if(!window.isOpen())break;
        // Explicit opt-in UI smoke harness; never active in normal player mode.
        if(smokeFrames && !benchmark){
            if(frames==5){mouse={665,44};click=true;} // 1x button
            if(frames==40){mouse={584,44};click=true;} // pause button
            if(frames==45){mouse={620,510};click=true;} // review tax decision
            if(frames==48){mouse={740,474};click=true;}
            if(frames==65)session_->notify({0,session_->simulation().world().clock.day(),EventSeverity::Major,EventCategory::Disaster,"SMOKE TEST: severe drought - choose a response",true,false});
            if(frames==75){mouse={600,448};click=true;}
            if(frames==85){mouse={540,128};click=true;}
            if(frames==90)dashboard_.scroll({500,300},-1);
            if(frames==105){mouse={1150,44};click=true;}
            if(frames==108){mouse={600,326};click=true;} // confirm through UI pipeline
        }
        sf::Clock logical;
        if(!menuOpen && !dashboard_.confirming())session_->advance(dt);
        computeSeconds+=logical.getElapsedTime().asSeconds();
        window.clear(theme_.background);
        if(dashboard_.draw(window,theme_,*session_,mouse,click,menuOpen)){menuOpen=true;session_->setSpeed(SimulationSpeed::Pause);click=false;}
        if(menuOpen || session_->lost()){
            auto choice=menu_.draw(window,theme_,mouse,click,session_->lost());
            if(choice==MenuChoice::Return)menuOpen=false;
            if(choice==MenuChoice::Quit)window.close();
            if(choice==MenuChoice::NewGame){session_=std::make_unique<PlayerSession>(config_);dashboard_=DashboardScreen{};menuOpen=false;clock.restart();}
        }
        window.display();++frames;
        if(smokeFrames && (frames==60 || frames==70 || frames==100)){
            sf::Texture image;image.create(window.getSize().x,window.getSize().y);image.update(window);
            image.copyToImage().saveToFile("build/sfml-"+std::to_string(frames)+".png");
        }
        if(smokeFrames && frames>=smokeFrames){
            sf::Texture capture;capture.create(window.getSize().x,window.getSize().y);capture.update(window);
            capture.copyToImage().saveToFile("build/sfml-smoke.png");
            std::cout<<"SFML smoke: frames="<<frames<<" seconds="<<performance.getElapsedTime().asSeconds()<<" logical_seconds="<<computeSeconds<<" decisions="<<session_->decisions().history().records().size()<<" tax="<<session_->simulation().world().government.policy.incomeTax<<" day="<<session_->simulation().world().clock.day()<<" conserved="<<session_->simulation().world().ledger.conserved()<<'\n';
            window.close();
            if(!benchmark && (session_->decisions().history().records().empty() || session_->simulation().world().government.policy.incomeTax!=config_.taxRate+500 || session_->request() || session_->speed()!=SimulationSpeed::Pause))return 2;
        }
    }
    return 0;
}
}
