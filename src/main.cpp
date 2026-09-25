#include <SFML/Graphics.hpp>
#include "core/game_logic.h"
#include "ui/game_renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 700}), "MedievalBlock - Classical Block Game");
    window.setFramerateLimit(60);

    GameLogic game;
    GameRenderer renderer;
    sf::Clock clock;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }

                if (game.isGameOver()) {
                    if (key->code == sf::Keyboard::Key::R) {
                        game.reset();
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
                    }
                }
            }
        }

        game.update(dt);

        window.clear(sf::Color(20, 22, 28));
        renderer.render(window, game);
        window.display();
    }

    return 0;
}
