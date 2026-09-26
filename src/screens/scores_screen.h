#pragma once
#include "screen.h"
#include "core/score_manager.h"
#include "ui/game_renderer.h"
#include <string>

class ScoresScreen : public Screen {
private:
    GameRenderer& renderer;
    ScreenType nextScreen;
    ScoreManager scoreManager;

    int pendingScore;
    bool isNewRecord;
    bool enteringName;
    std::string inputName;
    std::string newlyAddedName;
    float blinkTimer;

    sf::FloatRect getAlgorithmButtonBounds() const;
    sf::FloatRect getBackButtonBounds() const;

public:
    explicit ScoresScreen(GameRenderer& renderer);

    void setPendingScore(int score);
    void clearPendingScore();
    void toggleAlgorithm();

    void handleEvent(const sf::Event& event) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;

    ScreenType getNextScreen() const override;
    void clearNextScreen() override;
};
