#include "dictatorsim/ui/UITheme.hpp"
#include <sstream>
#include <iomanip>
#include <stdexcept>
namespace ds {
UITheme::UITheme(const std::string& path){if(!font_.loadFromFile(path))throw std::runtime_error("Cannot load font: "+path+" (use --font FILE)");}
void UITheme::panel(sf::RenderTarget& target,const std::string& key,sf::FloatRect b,sf::Color color){auto& shape=shapes_[key];shape.setPosition(b.left,b.top);shape.setSize({b.width,b.height});shape.setFillColor(color);target.draw(shape);}
void UITheme::text(sf::RenderTarget& target,const std::string& key,const std::string& value,float x,float y,unsigned size,sf::Color color){
    auto pair=texts_.try_emplace(key);auto& t=pair.first->second;
    if(pair.second)t.setFont(font_);
    auto& previous=textValues_[key];
    if(pair.second || previous!=value){previous=value;t.setString(value);}
    t.setPosition(x,y);t.setCharacterSize(size);t.setFillColor(color);target.draw(t);
}
bool UITheme::button(sf::RenderTarget& target,const std::string& key,const std::string& label,sf::FloatRect b,sf::Vector2f mouse,bool click,bool selected,bool enabled){
    bool hover=b.contains(mouse);
    panel(target,key,b,!enabled?sf::Color(35,40,52):selected?sf::Color(32,112,104):hover?sf::Color(58,78,103):sf::Color(36,51,74));
    text(target,key+"-text",label,b.left+10,b.top+9,14,enabled?ink:muted);
    return enabled && hover && click;
}
std::string UITheme::wrap(const std::string& value,std::size_t columns){std::istringstream in(value);std::string word,out;std::size_t length=0;while(in>>word){if(length && length+word.size()+1>columns){out+='\n';length=0;}if(length){out+=' ';++length;}out+=word;length+=word.size();}return out;}
std::string UITheme::money(std::int64_t n){std::ostringstream out;if(n<0){out<<'-';n=-n;}out<<n/100<<'.'<<std::setfill('0')<<std::setw(2)<<n%100;return out.str();}
std::string UITheme::number(double n){std::ostringstream out;out<<std::fixed<<std::setprecision(1)<<n;return out.str();}
}
