#include "dictatorsim/ui/OfficeScreen.hpp"
#include <cmath>
#include <algorithm>
namespace ds {
bool OfficeScreen::draw(sf::RenderTarget& t,UITheme& ui,const PlayerSession& session,sf::Vector2f mouse,bool click){
    const auto& w=session.simulation().world();const auto s=session.simulation().report().observed;
    const sf::Color brass(205,173,108), paper(235,226,202), print(49,57,53), green(32,67,61);
    auto rect=[&](const char* key,sf::FloatRect b,sf::Color c){ui.panel(t,key,b,c);};
    rect("office-wall",{0,0,1280,720},{24,42,43});
    for(int i=0;i<9;++i){float x=i*160.f;ui.panel(t,"wall-inset"+std::to_string(i),{x+10,100,138,266},{29,49,49});ui.panel(t,"wall-trim"+std::to_string(i),{x+10,100,1,266},{71,83,69});}
    rect("wall-rail",{0,368,1280,8},brass);
    // Window, skyline and warm evening glow behind the desk.
    ui.roundPanel(t,{52,115,288,234},{12,25,29},4);
    rect("window-sky",{61,124,270,214},{93,120,125});
    sf::CircleShape sun(34,40);sun.setPosition(236,142);sun.setFillColor({222,186,128});t.draw(sun);
    for(int i=0;i<11;++i){float h=38+(i*31)%74;ui.panel(t,"skyline"+std::to_string(i),{61+i*25.f,338-h,23,h},{42,67,72});}
    rect("window-cross",{191,124,6,214},{25,43,44});rect("window-cross2",{61,227,270,6},{25,43,44});
    // Republic crest, framed over the writing pad.
    ui.roundPanel(t,{401,116,258,91},{15,30,32},8);
    ui.text(t,"office-crest","D / S",425,127,27,brass);
    ui.text(t,"office-crest-title","LA PRÉSIDENCE",508,135,15,paper);
    ui.text(t,"office-crest-sub","SERVIR. DECIDER.",508,159,10,brass);
    rect("flag-pole",{717,128,4,253},brass);
    // Original fictional banner: violet field, turquoise diagonal, gilded orbital seal.
    rect("flag-shadow",{724,143,87,122},{0,0,0,65});
    rect("flag-field",{721,140,87,122},{65,43,99});
    sf::ConvexShape sash(4);
    sash.setPoint(0,{721,218});sash.setPoint(1,{808,151});
    sash.setPoint(2,{808,185});sash.setPoint(3,{721,252});
    sash.setFillColor({36,157,151});t.draw(sash);
    sf::CircleShape seal(21,6);seal.setOrigin(21,21);seal.setPosition(764,196);
    seal.setFillColor({65,43,99});seal.setOutlineColor(brass);seal.setOutlineThickness(3);t.draw(seal);
    sf::CircleShape diamond(10,4);diamond.setOrigin(10,10);diamond.setPosition(764,196);diamond.setFillColor(paper);t.draw(diamond);
    for(int i=0;i<3;++i){sf::CircleShape spark(3,4);spark.setPosition(747+i*13.f,237);spark.setFillColor(brass);t.draw(spark);}
    rect("flag-hoist",{721,140,3,122},brass);
    // Desk perspective, individual wood boards and leather writing surface.
    sf::ConvexShape desk(4);desk.setPoint(0,{0,392});desk.setPoint(1,{1280,374});desk.setPoint(2,{1390,720});desk.setPoint(3,{-100,720});desk.setFillColor({87,55,41});t.draw(desk);
    for(int i=0;i<14;++i)ui.panel(t,"woodgrain"+std::to_string(i),{0,405+i*23.f,1280,2},{108,71,50,100});
    ui.roundPanel(t,{337,368,423,278},{43,29,24},15);ui.roundPanel(t,{345,374,407,260},green,12);
    // Newspaper doubles as a readable morning briefing.
    ui.roundPanel(t,{43,355,279,296},{0,0,0,90},3);rect("paper",{36,347,279,296},paper);
    ui.text(t,"newspaper-date","EDITION DU JOUR  /  "+std::to_string(s.day),53,359,10,print);
    ui.text(t,"newspaper-masthead","LE NATIONAL",52,375,27,print);
    rect("newspaper-rule",{53,415,245,2},print);
    ui.text(t,"newspaper-headline",s.averageHappiness>=60?"Un pays qui garde le sourire":s.averageAnger>=60?"La colere gagne les rues":"Le pays attend vos decisions",53,429,13,print);
    ui.text(t,"newspaper-deck","Le bilan de votre administration",53,451,11,{102,105,91});
    const std::pair<std::string,std::string> rows[]={{"Citoyens",std::to_string(s.population)},{"Tresor public",ui.money(s.treasuryBalance)},{"Bonheur",ui.number(s.averageHappiness)+" / 100"},{"Stabilite",ui.number(s.nationalStability)+" / 100"},{"Manifestants",std::to_string(w.society.protesters)}};
    for(int i=0;i<5;++i){float y=480+i*27.f;ui.text(t,"paper-label"+std::to_string(i),rows[i].first,53,y,12,print);ui.text(t,"paper-value"+std::to_string(i),rows[i].second,185,y,12,print);}
    // A folder you actually open to govern; the documents remain fully functional.
    ui.roundPanel(t,{368,388,136,32},{190,159,102},5);ui.roundPanel(t,{368,410,361,199},{213,188,140},6);
    rect("folder-sheet",{382,422,333,174},{245,237,218});
    ui.text(t,"folder-classification","CONFIDENTIEL  /  CABINET",400,436,11,{149,74,57});
    ui.text(t,"folder-title","Vos décisions\nfont l'histoire.",400,464,27,print);
    ui.text(t,"folder-desc","Ordres, budgets et bilan national",400,539,13,{107,106,90});
    bool open=ui.button(t,"office-orders","OUVRIR LES DOSSIERS  >",{399,568,290,34},mouse,click);
    // Television chassis, stand, studio set and news ticker.
    ui.roundPanel(t,{843,122,389,254},{0,0,0,95},18);ui.roundPanel(t,{833,112,389,254},{18,23,27},18);
    ui.roundPanel(t,{846,126,363,213},{25,58,73},10);
    rect("tv-studio-wall",{856,166,164,110},{37,80,94});
    for(int i=0;i<4;++i)ui.panel(t,"tv-studio-line"+std::to_string(i),{862+i*39.f,170,2,101},{76,126,136,130});
    sf::CircleShape head(19,30);head.setPosition(920,182);head.setFillColor({204,157,120});t.draw(head);
    ui.roundPanel(t,{912,218,55,53},{25,35,53},12);rect("anchor-tie",{937,222,6,34},{170,75,64});
    rect("anchor-desk",{877,261,141,15},{125,161,169});
    ui.text(t,"tv-channel","DS 24",860,136,20,paper);
    bool pulse=std::fmod(ambience_.getElapsedTime().asSeconds(),2)<1.3;
    ui.roundPanel(t,{1114,137,80,22},pulse?sf::Color(179,61,58):sf::Color(132,51,50),4);ui.text(t,"tv-live","EN DIRECT",1122,141,10,sf::Color::White);
    ui.text(t,"tv-bulletin","LE POINT\nSUR LE PAYS",1034,184,19,paper);
    ui.text(t,"tv-stat","Loyaute : "+ui.number(s.militaryLoyalty),1034,240,12,{160,205,209});
    std::string headline="Le gouvernement prend ses fonctions.";
    const auto& events=w.logger.entries();if(!events.empty())headline=events.back().message;
    if(headline.size()>85)headline=headline.substr(0,82)+"...";
    rect("tv-ticker",{846,282,363,57},{13,32,42});ui.text(t,"tv-news",ui.wrap(headline,45),858,290,12,paper);
    for(int i=0;i<35;++i)ui.panel(t,"tv-scan"+std::to_string(i),{847,128+i*6.f,361,1},{0,0,0,15});
    ui.text(t,"tv-brand","RESEAU NATIONAL  /  24 HEURES SUR 24",901,348,8,{130,142,143});
    rect("tv-leg",{1000,366,55,24},{24,29,31});ui.roundPanel(t,{955,386,145,9},{23,29,30},4);
    // Desk plaque, cup and a compact live dispatch card.
    ui.roundPanel(t,{843,425,365,155},{26,44,43},10);
    ui.text(t,"desk-note-title","LE POULS DE LA NATION",861,441,15,brass);
    ui.face(t,{886,498},21,s.averageAnger>60?1:s.averageHappiness>=60?0:4);
    ui.text(t,"desk-note-state",s.averageAnger>60?"La tension monte.":s.averageHappiness>=60?"Le moral tient bon.":"Une population a l'ecoute.",924,478,15,paper);
    ui.text(t,"desk-note-hint","Explorez la carte pour voir\nles besoins de vos citoyens.",924,507,12,{165,186,172});
    ui.roundPanel(t,{844,603,257,34},{48,37,26},4);ui.text(t,"desk-nameplate","BUREAU DU PRESIDENT",861,610,15,brass);
    sf::CircleShape saucer(25,40);saucer.setPosition(1143,597);saucer.setFillColor({218,212,188});t.draw(saucer);
    sf::CircleShape coffee(17,40);coffee.setPosition(1151,605);coffee.setFillColor({65,40,28});t.draw(coffee);
    return open;
}
}
