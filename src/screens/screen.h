#pragma once
#include <SFML/Graphics.hpp>

enum class ScreenType {
    NONE,
    MENU,
    GAME,
    SCORES,
    EXIT
};

class Screen {
public:
    virtual ~Screen() = default;

    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;

    virtual ScreenType getNextScreen() const { return ScreenType::NONE; }
    virtual void clearNextScreen() {}
};
