#include "menu_screen.h"

MenuScreen::MenuScreen(GameRenderer& rend)
    : renderer(rend), menuSelection(0), nextScreen(ScreenType::NONE) {
}

void MenuScreen::reset() {
    menuSelection = 0;
    nextScreen = ScreenType::NONE;
}

sf::FloatRect MenuScreen::getPlayButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(500.f, 305.f), sf::Vector2f(280.f, 55.f));
}

sf::FloatRect MenuScreen::getScoresButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(500.f, 375.f), sf::Vector2f(280.f, 55.f));
}

sf::FloatRect MenuScreen::getExitButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(500.f, 445.f), sf::Vector2f(280.f, 55.f));
}

void MenuScreen::handleEvent(const sf::Event& event) {
    if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        sf::Vector2f mPos(static_cast<float>(mouseMoved->position.x), static_cast<float>(mouseMoved->position.y));
        if (getPlayButtonBounds().contains(mPos)) {
            menuSelection = 0;
        } else if (getScoresButtonBounds().contains(mPos)) {
            menuSelection = 1;
        } else if (getExitButtonBounds().contains(mPos)) {
            menuSelection = 2;
        }
    } else if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
            if (getPlayButtonBounds().contains(mPos)) {
                nextScreen = ScreenType::GAME;
            } else if (getScoresButtonBounds().contains(mPos)) {
                nextScreen = ScreenType::SCORES;
            } else if (getExitButtonBounds().contains(mPos)) {
                nextScreen = ScreenType::EXIT;
            }
        }
    } else if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Up || key->code == sf::Keyboard::Key::W) {
            menuSelection = (menuSelection - 1 + 3) % 3;
        } else if (key->code == sf::Keyboard::Key::Down || key->code == sf::Keyboard::Key::S) {
            menuSelection = (menuSelection + 1) % 3;
        } else if (key->code == sf::Keyboard::Key::Enter || key->code == sf::Keyboard::Key::Space) {
            if (menuSelection == 0) {
                nextScreen = ScreenType::GAME;
            } else if (menuSelection == 1) {
                nextScreen = ScreenType::SCORES;
            } else {
                nextScreen = ScreenType::EXIT;
            }
        } else if (key->code == sf::Keyboard::Key::Escape) {
            nextScreen = ScreenType::EXIT;
        }
    }
}

void MenuScreen::update(float) {
}

void MenuScreen::render(sf::RenderWindow& window) {
    const float SCREEN_W = 1280.f;

    renderer.drawBackground(window);

    if (!renderer.isFontLoaded()) return;
    const sf::Font& font = renderer.getFont();
    NineSlice& nineSlice = renderer.getNineSlice();

    sf::Color textFill(245, 235, 210);
    sf::Color textOutline(65, 48, 30);
    sf::Color goldText(255, 235, 140);
    sf::Color goldOutline(85, 60, 25);

    sf::Text title(font);
    title.setString("MEDIEVAL BLOCK");
    title.setCharacterSize(64);
    title.setFillColor(goldText);
    title.setOutlineColor(goldOutline);
    title.setOutlineThickness(2.5f);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition({(SCREEN_W - titleBounds.size.x) * 0.5f, 130.f});
    window.draw(title);

    sf::Text subtitle(font);
    subtitle.setString("TETRIS MEDIEVAL - UNA EIF207");
    subtitle.setCharacterSize(22);
    subtitle.setFillColor(textFill);
    subtitle.setOutlineColor(textOutline);
    subtitle.setOutlineThickness(1.2f);
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition({(SCREEN_W - subBounds.size.x) * 0.5f, 215.f});
    window.draw(subtitle);

    auto drawMenuButton = [&](sf::FloatRect bounds, int index, const std::string& label) {
        bool selected = (menuSelection == index);
        sf::Color tint = selected ? sf::Color(255, 245, 205) : sf::Color(180, 180, 180, 220);
        nineSlice.draw(window, bounds, tint);

        if (selected) {
            sf::RectangleShape highlight(bounds.size);
            highlight.setPosition(bounds.position);
            highlight.setFillColor(sf::Color::Transparent);
            highlight.setOutlineThickness(2.f);
            highlight.setOutlineColor(sf::Color(255, 225, 130, 200));
            window.draw(highlight);
        }

        sf::Text btnText(font);
        btnText.setString(selected ? ("> " + label + " <") : label);
        btnText.setCharacterSize(24);
        btnText.setFillColor(selected ? goldText : textFill);
        btnText.setOutlineColor(selected ? goldOutline : textOutline);
        btnText.setOutlineThickness(1.5f);
        sf::FloatRect btb = btnText.getLocalBounds();
        btnText.setPosition({bounds.position.x + (bounds.size.x - btb.size.x) * 0.5f,
                             bounds.position.y + (bounds.size.y - btb.size.y) * 0.5f - 4.f});
        window.draw(btnText);
    };

    drawMenuButton(getPlayButtonBounds(), 0, "JUGAR");
    drawMenuButton(getScoresButtonBounds(), 1, "PUNTAJES");
    drawMenuButton(getExitButtonBounds(), 2, "SALIR");

    sf::Text hint(font);
    hint.setString("[W / S / Flechas]  Navegar      [ENTER / ESPACIO / Clic]  Seleccionar      [ESC]  Salir");
    hint.setCharacterSize(18);
    hint.setFillColor(textFill);
    hint.setOutlineColor(textOutline);
    hint.setOutlineThickness(1.0f);
    sf::FloatRect htb = hint.getLocalBounds();
    hint.setPosition({(SCREEN_W - htb.size.x) * 0.5f, 545.f});
    window.draw(hint);
}

ScreenType MenuScreen::getNextScreen() const {
    return nextScreen;
}

void MenuScreen::clearNextScreen() {
    nextScreen = ScreenType::NONE;
}
