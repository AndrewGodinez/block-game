#include <SFML/Graphics.hpp>
#include "ui/game_renderer.h"
#include "screens/menu_screen.h"
#include "screens/game_screen.h"
#include "screens/scores_screen.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 700}), "MedievalBlock - Classical Block Game");
    window.setFramerateLimit(60);

    GameRenderer renderer;
    sf::Clock clock;

    MenuScreen menuScreen(renderer);
    GameScreen gameScreen(renderer);
    ScoresScreen scoresScreen(renderer);

    Screen* currentScreen = &menuScreen;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else {
                currentScreen->handleEvent(*event);
            }
        }

        currentScreen->update(dt);

        ScreenType next = currentScreen->getNextScreen();
        if (next != ScreenType::NONE) {
            currentScreen->clearNextScreen();

            if (next == ScreenType::EXIT) {
                window.close();
            } else if (next == ScreenType::MENU) {
                menuScreen.reset();
                currentScreen = &menuScreen;
            } else if (next == ScreenType::GAME) {
                gameScreen.startNewGame();
                currentScreen = &gameScreen;
            } else if (next == ScreenType::SCORES) {
                if (currentScreen == &gameScreen) {
                    scoresScreen.setPendingScore(gameScreen.getFinalScore());
                } else {
                    scoresScreen.clearPendingScore();
                }
                currentScreen = &scoresScreen;
            }
        }

        window.clear(sf::Color(20, 22, 28));
        currentScreen->render(window);
        window.display();
    }

    return 0;
}
