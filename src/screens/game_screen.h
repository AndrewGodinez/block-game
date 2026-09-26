#pragma once
#include "screen.h"
#include "core/game_logic.h"
#include "ui/game_renderer.h"

class GameScreen : public Screen {
private:
    GameRenderer& renderer;
    GameLogic game;
    ScreenType nextScreen;
    int finalScore;
    bool wasReplaying;

public:
    explicit GameScreen(GameRenderer& renderer);

    void startNewGame();
    int getFinalScore() const;

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    ScreenType getNextScreen() const override;
    void clearNextScreen() override;
};
