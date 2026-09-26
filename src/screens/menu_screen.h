#pragma once
#include "screen.h"
#include "ui/game_renderer.h"

class MenuScreen : public Screen {
private:
    GameRenderer& renderer;
    int menuSelection;
    ScreenType nextScreen;

    sf::FloatRect getPlayButtonBounds() const;
    sf::FloatRect getScoresButtonBounds() const;
    sf::FloatRect getExitButtonBounds() const;

public:
    explicit MenuScreen(GameRenderer& renderer);

    void reset();
    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    ScreenType getNextScreen() const override;
    void clearNextScreen() override;
};
