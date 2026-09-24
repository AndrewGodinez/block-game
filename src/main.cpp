#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Block Game - Hello World");
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = font.openFromFile("src/assets/Deutsch.ttf");

    sf::Text text(font);
    if (fontLoaded) {
        text.setString("Hello World - SFML");
        text.setCharacterSize(40);
        text.setFillColor(sf::Color::White);
        text.setPosition({240.f, 260.f});
    }

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        window.clear(sf::Color(30, 30, 45));
        if (fontLoaded) {
            window.draw(text);
        }
        window.display();
    }

    return 0;
}
