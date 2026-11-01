#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <cstdint>
#include <string>
#include <functional>
namespace ds {
class UITheme {
    sf::Font font_;
    std::function<void()> buttonSound_;
    sf::Texture faces_;
    bool facesReady_=false;
    std::map<std::string,sf::Text> texts_;
    std::map<std::string,std::string> textValues_;
    std::map<std::string,sf::RectangleShape> shapes_;
public:
    const sf::Color background{12,19,32}, panelColor{20,33,47,240}, muted{151,171,194}, ink{232,239,249}, accent{73,192,160}, danger{239,100,110};
    void roundPanel(sf::RenderTarget&,sf::FloatRect,sf::Color,float radius=12);
    void face(sf::RenderTarget&,sf::Vector2f,float radius,int mood);
    void setButtonSound(std::function<void()> callback) { buttonSound_=callback; }
    explicit UITheme(const std::string& fontPath);
    void panel(sf::RenderTarget& target,const std::string& key,sf::FloatRect bounds,sf::Color color);
    void text(sf::RenderTarget& target,const std::string& key,const std::string& value,float x,float y,unsigned size=16,sf::Color color=sf::Color::White);
    bool button(sf::RenderTarget& target,const std::string& key,const std::string& label,sf::FloatRect bounds,sf::Vector2f mouse,bool click,bool selected=false,bool enabled=true);
    static std::string wrap(const std::string& text,std::size_t columns);
    static std::string money(std::int64_t cents);
    static std::string number(double value);
};
}
