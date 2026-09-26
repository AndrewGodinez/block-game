#include <SFML/Graphics.hpp>
#include "core/game_logic.h"
#include "ui/game_renderer.h"

enum class AppState {
    MENU,
    PLAYING
};

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 700}), "MedievalBlock - Classical Block Game");
    window.setFramerateLimit(60);

    GameLogic game;
    GameRenderer renderer;
    sf::Clock clock;

    AppState appState = AppState::MENU;
    int menuSelection = 0;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                if (appState == AppState::MENU) {
                    sf::Vector2f mPos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));
                    if (renderer.getPlayButtonBounds().contains(mPos)) {
                        menuSelection = 0;
                    } else if (renderer.getExitButtonBounds().contains(mPos)) {
                        menuSelection = 1;
                    }
                }
            } else if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (appState == AppState::MENU && mouseBtn->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mPos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
                    if (renderer.getPlayButtonBounds().contains(mPos)) {
                        game.reset();
                        appState = AppState::PLAYING;
                    } else if (renderer.getExitButtonBounds().contains(mPos)) {
                        window.close();
                    }
                }
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (appState == AppState::MENU) {
                    if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W) {
                        menuSelection = (menuSelection - 1 + 2) % 2;
                    } else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
                        menuSelection = (menuSelection + 1) % 2;
                    } else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) {
                        if (menuSelection == 0) {
                            game.reset();
                            appState = AppState::PLAYING;
                        } else {
                            window.close();
                        }
                    } else if (key->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                } else {
                    if (key->code == sf::Keyboard::Key::Escape) {
                        if (game.isReplayMode()) {
                            game.stopReplay();
                        } else {
                            appState = AppState::MENU;
                        }
                    } else if (game.isReplayMode()) {
                        if (key->code == sf::Keyboard::Key::Space) {
                            game.toggleReplayPlay();
                        } else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
                            game.replayStepForward();
                        } else if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
                            game.replayStepBackward();
                        } else if (key->code == sf::Keyboard::Key::R) {
                            game.stopReplay();
                            game.reset();
                        }
                    } else if (game.isGameOver()) {
                        if (key->code == sf::Keyboard::Key::R) {
                            game.reset();
                        } else if (key->code == sf::Keyboard::Key::P) {
                            game.startReplay();
                        }
                    } else {
                        if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
                            game.moveLeft();
                        } else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
                            game.moveRight();
                        } else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
                            game.moveDown();
                        } else if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W) {
                            game.rotate();
                        } else if (key->code == sf::Keyboard::Key::Space) {
                            game.hardDrop();
                        } else if (key->code == sf::Keyboard::Key::C) {
                            game.holdCurrentPiece();
                        } else if (key->code == sf::Keyboard::Key::Z) {
                            game.undo();
                        } else if (key->code == sf::Keyboard::Key::Y) {
                            game.redo();
                        }
                    }
                }
            }
        }

        if (appState == AppState::PLAYING) {
            game.update(dt);
        }

        window.clear(sf::Color(20, 22, 28));
        if (appState == AppState::MENU) {
            renderer.renderMenu(window, menuSelection);
        } else {
            renderer.render(window, game);
        }
        window.display();
    }

    return 0;
}
