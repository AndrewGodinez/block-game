#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class NineSlice {
private:
    sf::Texture texture;
    bool textureLoaded;
    float marginLeft;
    float marginTop;
    float marginRight;
    float marginBottom;

public:
    NineSlice();
    bool loadFromFile(const std::string& filepath, float ml = 35.f, float mt = 35.f, float mr = 35.f, float mb = 35.f);
    void draw(sf::RenderTarget& target, sf::FloatRect bounds, sf::Color tint = sf::Color::White) const;
    bool isLoaded() const;
};
