#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <cstdint>
#include <string>
namespace ds {
class UITheme {
    sf::Font font_;
    std::map<std::string,sf::Text> texts_;
    std::map<std::string,std::string> textValues_;
    std::map<std::string,sf::RectangleShape> shapes_;
public:
    const sf::Color background{12,19,32}, panelColor{22,32,49}, muted{151,171,194}, ink{232,239,249}, accent{73,192,160}, danger{239,100,110};
    explicit UITheme(const std::string& fontPath);
    void panel(sf::RenderTarget& target,const std::string& key,sf::FloatRect bounds,sf::Color color);
    void text(sf::RenderTarget& target,const std::string& key,const std::string& value,float x,float y,unsigned size=16,sf::Color color=sf::Color::White);
    bool button(sf::RenderTarget& target,const std::string& key,const std::string& label,sf::FloatRect bounds,sf::Vector2f mouse,bool click,bool selected=false,bool enabled=true);
    static std::string wrap(const std::string& text,std::size_t columns);
    static std::string money(std::int64_t cents);
    static std::string number(double value);
};
}
