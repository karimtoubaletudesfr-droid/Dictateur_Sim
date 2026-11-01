#include "dictatorsim/ui/UITheme.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <algorithm>
namespace ds {
UITheme::UITheme(const std::string& path){if(!font_.loadFromFile(path))throw std::runtime_error("Cannot load font: "+path+" (use --font FILE)");}
void UITheme::panel(sf::RenderTarget& target,const std::string& key,sf::FloatRect b,sf::Color color){auto& shape=shapes_[key];shape.setPosition(b.left,b.top);shape.setSize({b.width,b.height});shape.setFillColor(color);shape.setOutlineThickness(0);
    if(color==panelColor){sf::RectangleShape shadow({b.width,b.height});shadow.setPosition(b.left+4,b.top+5);shadow.setFillColor({0,0,0,65});target.draw(shadow);shape.setOutlineThickness(1);shape.setOutlineColor({69,94,109,160});}
    target.draw(shape);
    if(color==panelColor){sf::RectangleShape line({b.width,2});line.setPosition(b.left,b.top);line.setFillColor({73,192,160,130});target.draw(line);}}
void UITheme::text(sf::RenderTarget& target,const std::string& key,const std::string& value,float x,float y,unsigned size,sf::Color color){
    auto pair=texts_.try_emplace(key);auto& t=pair.first->second;
    if(pair.second)t.setFont(font_);
    auto& previous=textValues_[key];
    if(pair.second || previous!=value){previous=value;t.setString(sf::String::fromUtf8(value.begin(),value.end()));}
    t.setPosition(x,y);t.setCharacterSize(size);t.setFillColor(color);target.draw(t);
}
bool UITheme::button(sf::RenderTarget& target,const std::string& key,const std::string& label,sf::FloatRect b,sf::Vector2f mouse,bool click,bool selected,bool enabled){
    bool hover=b.contains(mouse);
    roundPanel(target,{b.left,b.top+2,b.width,b.height},{0,0,0,55},6);
    roundPanel(target,b,!enabled?sf::Color(40,44,49):selected?sf::Color(38,112,99):hover?sf::Color(70,88,94):sf::Color(40,57,66),6);
    if(selected){sf::RectangleShape marker({3,b.height-12});marker.setPosition(b.left,b.top+6);marker.setFillColor(accent);target.draw(marker);}
    text(target,key+"-text",label,b.left+10,b.top+(b.height-18)/2-2,14,enabled?ink:muted);
    bool activated=enabled && hover && click;
    if(activated && buttonSound_)buttonSound_();
    return activated;
}
std::string UITheme::wrap(const std::string& value,std::size_t columns){std::istringstream in(value);std::string word,out;std::size_t length=0;while(in>>word){if(length && length+word.size()+1>columns){out+='\n';length=0;}if(length){out+=' ';++length;}out+=word;length+=word.size();}return out;}
std::string UITheme::money(std::int64_t n){std::ostringstream out;if(n<0){out<<'-';n=-n;}out<<n/100<<'.'<<std::setfill('0')<<std::setw(2)<<n%100;return out.str();}
std::string UITheme::number(double n){std::ostringstream out;out<<std::fixed<<std::setprecision(1)<<n;return out.str();}
}

namespace ds {
void UITheme::roundPanel(sf::RenderTarget& t,sf::FloatRect b,sf::Color color,float r){
    r=std::min(r,std::min(b.width,b.height)/2);
    sf::ConvexShape shape(32);
    const sf::Vector2f centers[]={{b.left+b.width-r,b.top+r},{b.left+b.width-r,b.top+b.height-r},{b.left+r,b.top+b.height-r},{b.left+r,b.top+r}};
    for(int corner=0;corner<4;++corner)for(int i=0;i<8;++i){float angle=(-90+corner*90+i*90.f/7)*3.14159265f/180;shape.setPoint(corner*8+i,centers[corner]+sf::Vector2f(std::cos(angle)*r,std::sin(angle)*r));}
    shape.setFillColor(color);t.draw(shape);
}
static void paintFace(sf::RenderTarget& t,sf::Vector2f pos,float r,int mood){
    const sf::Color colors[]={{245,201,84},{239,113,91},{241,168,75},{157,162,215},{202,198,174}};
    auto disk=[&](float x,float y,float radius,sf::Color c){sf::CircleShape s(radius,28);s.setOrigin(radius,radius);s.setPosition(pos.x+x*r,pos.y+y*r);s.setFillColor(c);t.draw(s);};
    auto line=[&](float x,float y,float xx,float yy){sf::Vertex v[]={{{pos.x+x*r,pos.y+y*r},{53,43,43}},{{pos.x+xx*r,pos.y+yy*r},{53,43,43}}};t.draw(v,2,sf::Lines);};
    disk(.05f,.16f,r+2,{0,0,0,85});disk(0,0,r,colors[std::clamp(mood,0,4)]);
    disk(-.27f,-.38f,r*.16f,{255,255,255,35});
    if(mood==3){line(-.55f,-.12f,-.15f,-.12f);line(.15f,-.12f,.55f,-.12f);}
    else {disk(-.33f,-.18f,r*.10f,{53,43,43});disk(.33f,-.18f,r*.10f,{53,43,43});}
    if(mood==1){line(-.6f,-.48f,-.15f,-.29f);line(.15f,-.29f,.6f,-.48f);}
    if(mood==2 || mood==3){disk(0,.36f,r*.2f,{65,47,49});}
    else if(mood==4)line(-.3f,.35f,.3f,.35f);
    else {sf::VertexArray mouth(sf::LineStrip,13);for(int i=0;i<13;++i){float x=-.42f+i*.07f;float y=mood==0?.28f+.21f*(1-x*x/.1764f):.48f-.20f*(1-x*x/.1764f);mouth[i]={{pos.x+x*r,pos.y+y*r},{53,43,43}};}t.draw(mouth);}
}
}

namespace ds {
void UITheme::face(sf::RenderTarget& target,sf::Vector2f position,float radius,int mood){
    if(!facesReady_){
        sf::RenderTexture atlas;
        if(!atlas.create(320,64)){paintFace(target,position,radius,mood);return;}
        atlas.clear(sf::Color::Transparent);
        for(int i=0;i<5;++i)paintFace(atlas,{32+i*64.f,32},24,i);
        atlas.display();faces_=atlas.getTexture();faces_.setSmooth(true);facesReady_=true;
    }
    sf::Sprite sprite(faces_,{std::clamp(mood,0,4)*64,0,64,64});
    sprite.setOrigin(32,32);sprite.setPosition(position);sprite.setScale(radius/24,radius/24);target.draw(sprite);
}
}
