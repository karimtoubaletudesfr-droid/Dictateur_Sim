#include "dictatorsim/ui/UrbanMap.hpp"
#include <algorithm>
#include <cmath>
namespace ds {
UrbanMap::UrbanMap(){
    auto quad=[&](float x,float y,float w,float h,sf::Color c){for(auto point:{sf::Vector2f{x,y},sf::Vector2f{x+w,y},sf::Vector2f{x+w,y+h},sf::Vector2f{x,y+h}})city_.append(sf::Vertex(point,c));};
    quad(0,0,1280,720,{17,32,37});
    // Deterministic top-down city, built once and rendered in a single draw call.
    for(int y=0;y<8;++y)for(int x=0;x<12;++x){
        float bx=18+x*110.f,by=12+y*96.f;
        quad(bx-8,by-8,102,88,{40,53,60});
        if((x*3+y*5)%11==0){quad(bx,by,86,72,{30,66,55});
            quad(bx+40,by,6,72,{76,91,74});
            for(int tree=0;tree<6;++tree){float tx=bx+9+(tree%2)*53,ty=by+8+(tree/2)*22;quad(tx+2,ty+3,14,14,{18,43,38});quad(tx,ty,14,14,{46,92,68});}
            continue;}
        for(int b=0;b<4;++b){float xx=bx+(b%2)*45,yy=by+(b/2)*38;int tint=(x*7+y*11+b*13)%22;
            float bw=27+(x+y+b)%3*5.f,bh=22+(x+b)%2*7.f;
            quad(xx+3,yy+4,bw,bh,{10,23,29});quad(xx,yy,bw,bh,sf::Color(59+tint,76+tint,83+tint));quad(xx+5,yy+5,bw-10,3,{89,107,110});
        }
    }
    quad(0,350,1280,24,{60,72,77});
    for(int x=0;x<1280;x+=32)quad(float(x),361,16,2,{133,136,113});
    quad(985,0,68,720,{25,64,78});quad(976,0,9,720,{51,78,77});quad(1053,0,9,720,{51,78,77});
    quad(975,345,90,34,{77,88,89});
}
void UrbanMap::draw(sf::RenderTarget& t,UITheme& ui,const PlayerSession& session,bool expanded,sf::Vector2f mouse,bool click){
    t.draw(city_);
    if(expanded)ui.panel(t,"citizen-map-contrast",{0,82,1280,590},{5,17,25,55});
    const auto& w=session.simulation().world();
    auto alert=[&](float x,float y,float radius,const std::string& label,const std::string& key){
        sf::CircleShape halo(radius,40);halo.setOrigin(radius,radius);halo.setPosition(x,y);halo.setFillColor({230,55,65,65});halo.setOutlineColor({250,95,100,180});halo.setOutlineThickness(2);t.draw(halo);
        sf::CircleShape dot(7,16);dot.setOrigin(7,7);dot.setPosition(x,y);dot.setFillColor(ui.danger);t.draw(dot);
        if(expanded){ui.panel(t,key+"-plate",{x-100,y+radius+7,220,28},{13,24,34,230});ui.text(t,key,label,x-92,y+radius+11,12,ui.ink);}
    };
    if(w.society.protesters)alert(365,285,50+std::min(40.,double(w.society.protesters)*.1),"Manifestants : "+std::to_string(w.society.protesters),"map-protest");
    if(w.society.rebellion)alert(685,440,90,"Rebellion en cours","map-rebellion");
    if(w.government.state.pendingCoup)alert(795,210,74,"Menace de coup d'Etat","map-coup");
    if(w.government.state.regime==RegimeStatus::Crisis && !w.society.rebellion && !w.government.state.pendingCoup)
        alert(650,225,70,"Crise politique","map-political");
    // Only the most recent disaster per region is needed; bound work per frame.
    bool marked[3]={false,false,false};unsigned checked=0;
    for(auto it=w.disasters.rbegin();it!=w.disasters.rend() && checked++<128;++it){
        if(w.clock.day()>it->day+10)break;
        if(it->region<3 && !marked[it->region]){
            marked[it->region]=true;
            alert(180+it->region*280.f,480-it->region*90.f,58,"Catastrophe / region "+std::to_string(it->region+1),"map-disaster"+std::to_string(it->region));
        }
    }

    if(!expanded)ui.panel(t,"map-dim",{0,0,1280,720},{5,12,22,85});
    else {
        const auto& citizens=w.population.getPopulation();
        ui.roundPanel(t,{610,96,646,69},{16,33,39,245},10);
        ui.text(t,"map-density-label","NOMBRE DE VISAGES / affichage uniquement",626,103,12,ui.muted);
        const std::size_t choices[]={48,100,200,300,400,600,800};
        for(int option=0;option<7;++option){
            auto limit=choices[option];
            if(ui.button(t,"map-density-"+std::to_string(limit),std::to_string(limit),{626+option*88.f,126,78,30},mouse,click,faceLimit_==limit))faceLimit_=limit;
        }
        std::size_t count=std::min(faceLimit_,citizens.size());
        // Fit the sample to available land; density never changes the population.
        std::size_t columns=std::max<std::size_t>(1,static_cast<std::size_t>(std::ceil(std::sqrt(count*1130./380.))));
        std::size_t rows=std::max<std::size_t>(1,(count+columns-1)/columns);
        float cellWidth=1130.f/columns,cellHeight=380.f/rows;
        float radius=std::min(13.f,.31f*std::min(cellWidth,cellHeight));
        const Citizen* hovered=nullptr;int hoveredMood=4;
        const char* moods[]={"Content","En colere","A faim","Fatigue","Neutre"};
        ui.roundPanel(t,{24,96,570,65},{16,33,39,240},10);
        ui.text(t,"map-title","LA VILLE A UN VISAGE.",40,106,21,ui.ink);
        ui.text(t,"map-subtitle",std::to_string(count)+" citoyens reels sur "+std::to_string(citizens.size())+" / Survolez les visages pour les rencontrer.",40,137,11,ui.muted);
        for(std::size_t i=0;i<count;++i){
            const auto& c=citizens[i*citizens.size()/count];
            // Deterministic jitter avoids rigid ranks and does not consume game randomness.
            float jitterX=(float((c.getId()*17)%101)/100.f-.5f)*.22f;
            float jitterY=(float((c.getId()*7)%101)/100.f-.5f)*.22f;
            float x=30+(float(i%columns)+.5f+jitterX)*cellWidth;
            if(x>970)x+=96; // Leave the river free.
            float y=184+(float(i/columns)+.5f+jitterY)*cellHeight;
            int mood=c.foodSatisfaction<35?2:c.fatigue>=65?3:c.anger>=60?1:c.happiness>=60?0:4;
            float dx=mouse.x-x,dy=mouse.y-y;
            bool over=dx*dx+dy*dy <= (radius+2)*(radius+2);
            // Ordinary faces use the cached atlas; extra geometry only for highlights.
            if(over || c.protesting){
                float ringRadius=over?radius+6:radius+3;
                sf::CircleShape ring(ringRadius,20);ring.setOrigin(ringRadius,ringRadius);ring.setPosition(x,y);
                ring.setFillColor({16,30,38,190});ring.setOutlineThickness(2);
                ring.setOutlineColor(c.protesting?ui.danger:sf::Color(255,229,151));t.draw(ring);
            }
            ui.face(t,{x,y},over?radius+4:radius,mood);
            if(over){hovered=&c;hoveredMood=mood;}
        }
        ui.roundPanel(t,{24,590,810,65},{14,29,36,245},10);
        for(int i=0;i<5;++i){ui.face(t,{49+i*150.f,614},13,i);ui.text(t,"map-mood"+std::to_string(i),moods[i],69+i*150.f,606,12,ui.ink);}
        ui.text(t,"map-schematic","Positions schematiques. Cercle rouge : manifestant. Zones rouges : crises. Visage = besoin prioritaire.",40,637,10,ui.muted);
        if(hovered){
            ui.roundPanel(t,{856,548,400,110},{12,27,34,250},10);
            ui.face(t,{887,580},18,hoveredMood);
            ui.text(t,"citizen-title","Citoyen #"+std::to_string(hovered->getId())+" / "+moods[hoveredMood],916,560,16,ui.ink);
            ui.text(t,"citizen-values","Bonheur "+ui.number(hovered->happiness)+"   Colere "+ui.number(hovered->anger)+"\nRepas "+ui.number(hovered->foodSatisfaction)+"   Fatigue "+ui.number(hovered->fatigue),916,590,13,ui.muted);
            ui.text(t,"citizen-scale","Indicateurs / 100  |  "+std::string(hovered->protesting?"Manifeste actuellement":"Ne manifeste pas"),875,636,10,ui.accent);
        }
    }
}
}
