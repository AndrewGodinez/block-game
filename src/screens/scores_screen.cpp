#include "scores_screen.h"

ScoresScreen::ScoresScreen(GameRenderer& rend)
    : renderer(rend), nextScreen(ScreenType::NONE),
      pendingScore(0), isNewRecord(false), enteringName(false), blinkTimer(0.0f) {
}

void ScoresScreen::setPendingScore(int score) {
    pendingScore = score;
    isNewRecord = scoreManager.qualifiesForTop10(pendingScore);
    enteringName = isNewRecord;
    inputName.clear();
    newlyAddedName.clear();
    nextScreen = ScreenType::NONE;
}

void ScoresScreen::clearPendingScore() {
    pendingScore = 0;
    isNewRecord = false;
    enteringName = false;
    inputName.clear();
    newlyAddedName.clear();
    nextScreen = ScreenType::NONE;
}

void ScoresScreen::toggleAlgorithm() {
    scoreManager.toggleAlgorithm();
}

sf::FloatRect ScoresScreen::getAlgorithmButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(220.f, 600.f), sf::Vector2f(410.f, 48.f));
}

sf::FloatRect ScoresScreen::getBackButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(650.f, 600.f), sf::Vector2f(410.f, 48.f));
}

void ScoresScreen::handleEvent(const sf::Event& event) {
    if (enteringName) {
        if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            char32_t c = textEntered->unicode;
            if (c >= 32 && c <= 126 && inputName.size() < 12) {
                inputName += static_cast<char>(c);
            }
        }

        if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Backspace) {
                if (!inputName.empty()) {
                    inputName.pop_back();
                }
            } else if (key->code == sf::Keyboard::Key::Enter) {
                if (inputName.empty()) {
                    inputName = "Caballero";
                }
                newlyAddedName = inputName;
                scoreManager.addScore(inputName, pendingScore);
                enteringName = false;
                pendingScore = 0;
            }
        }
        return;
    }

    if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
        if (key->code == sf::Keyboard::Key::Tab || key->code == sf::Keyboard::Key::A) {
            toggleAlgorithm();
        } else if (key->code == sf::Keyboard::Key::Escape || key->code == sf::Keyboard::Key::Enter) {
            nextScreen = ScreenType::MENU;
        }
    } else if (const auto* mouseBtn = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseBtn->button == sf::Mouse::Button::Left) {
            sf::Vector2f mPos(static_cast<float>(mouseBtn->position.x), static_cast<float>(mouseBtn->position.y));
            if (getAlgorithmButtonBounds().contains(mPos)) {
                toggleAlgorithm();
            } else if (getBackButtonBounds().contains(mPos)) {
                nextScreen = ScreenType::MENU;
            }
        }
    }
}

void ScoresScreen::update(float dt) {
    blinkTimer += dt;
    if (blinkTimer >= 0.8f) {
        blinkTimer = 0.0f;
    }
}

void ScoresScreen::render(sf::RenderWindow& window) {
    const float SCREEN_W = 1280.f;

    renderer.drawBackground(window);

    if (!renderer.isFontLoaded()) return;
    const sf::Font& font = renderer.getFont();
    NineSlice& nineSlice = renderer.getNineSlice();

    sf::Color textFill(245, 235, 210);
    sf::Color textOutline(65, 48, 30);
    sf::Color goldText(255, 235, 140);
    sf::Color goldOutline(85, 60, 25);

    sf::FloatRect panelBounds(sf::Vector2f(190.f, 20.f), sf::Vector2f(900.f, 655.f));
    nineSlice.draw(window, panelBounds);

    sf::RectangleShape innerPlate(sf::Vector2f(860.f, 615.f));
    innerPlate.setPosition({210.f, 40.f});
    innerPlate.setFillColor(sf::Color(20, 22, 28, 225));
    window.draw(innerPlate);

    sf::Text title(font);
    title.setString("SALON DE LA FAMA - TOP 10");
    title.setCharacterSize(40);
    title.setFillColor(goldText);
    title.setOutlineColor(goldOutline);
    title.setOutlineThickness(2.0f);
    sf::FloatRect tb = title.getLocalBounds();
    title.setPosition({(SCREEN_W - tb.size.x) * 0.5f, 50.f});
    window.draw(title);

    if (enteringName) {
        sf::RectangleShape promptBox(sf::Vector2f(820.f, 50.f));
        promptBox.setPosition({230.f, 105.f});
        promptBox.setFillColor(sf::Color(65, 45, 15, 230));
        promptBox.setOutlineThickness(1.5f);
        promptBox.setOutlineColor(sf::Color(255, 215, 0));
        window.draw(promptBox);

        std::string cursor = (blinkTimer < 0.4f) ? "_" : " ";
        std::string promptStr = "¡NUEVO RECORD! (" + std::to_string(pendingScore) + " pts)  Nombre: " + inputName + cursor + "  [ENTER: Guardar]";
        sf::Text promptTxt(font, promptStr, 20);
        promptTxt.setFillColor(sf::Color(255, 240, 160));
        promptTxt.setOutlineColor(goldOutline);
        promptTxt.setOutlineThickness(1.0f);
        promptTxt.setPosition({245.f, 116.f});
        window.draw(promptTxt);
    } else if (pendingScore > 0 && !isNewRecord) {
        sf::Text noticeTxt(font);
        noticeTxt.setString("Partida finalizada: " + std::to_string(pendingScore) + " pts (No alcanzo el Top 10)");
        noticeTxt.setCharacterSize(20);
        noticeTxt.setFillColor(sf::Color(210, 180, 140));
        noticeTxt.setOutlineColor(textOutline);
        noticeTxt.setOutlineThickness(1.0f);
        sf::FloatRect nb = noticeTxt.getLocalBounds();
        noticeTxt.setPosition({(SCREEN_W - nb.size.x) * 0.5f, 116.f});
        window.draw(noticeTxt);
    } else {
        sf::Text sub(font);
        sub.setString("Los caballeros y damas mas habiles del reino");
        sub.setCharacterSize(20);
        sub.setFillColor(textFill);
        sub.setOutlineColor(textOutline);
        sub.setOutlineThickness(1.0f);
        sf::FloatRect sb = sub.getLocalBounds();
        sub.setPosition({(SCREEN_W - sb.size.x) * 0.5f, 116.f});
        window.draw(sub);
    }

    sf::RectangleShape headerLine(sf::Vector2f(820.f, 28.f));
    headerLine.setPosition({230.f, 165.f});
    headerLine.setFillColor(sf::Color(45, 38, 28, 220));
    window.draw(headerLine);

    sf::Text hPos(font, "POS", 18);
    hPos.setPosition({260.f, 168.f});
    hPos.setFillColor(goldText);
    window.draw(hPos);

    sf::Text hName(font, "CABALLERO / DAMA", 18);
    hName.setPosition({410.f, 168.f});
    hName.setFillColor(goldText);
    window.draw(hName);

    sf::Text hScore(font, "PUNTAJE", 18);
    hScore.setPosition({860.f, 168.f});
    hScore.setFillColor(goldText);
    window.draw(hScore);

    float startY = 200.f;
    float rowHeight = 37.f;
    const doubleList<ScoreEntry>& scores = scoreManager.getScores();
    doubleNode<ScoreEntry>* curr = scores.head;

    for (int i = 0; i < 10 && curr != nullptr; ++i, curr = curr->next) {
        float py = startY + i * rowHeight;
        bool isNew = (!newlyAddedName.empty() && curr->data.name == newlyAddedName);

        if (isNew) {
            sf::RectangleShape rowHighlight(sf::Vector2f(820.f, rowHeight - 4.f));
            rowHighlight.setPosition({230.f, py - 2.f});
            rowHighlight.setFillColor(sf::Color(80, 60, 20, 180));
            rowHighlight.setOutlineThickness(1.5f);
            rowHighlight.setOutlineColor(sf::Color(255, 220, 100));
            window.draw(rowHighlight);
        } else if (i % 2 == 1) {
            sf::RectangleShape rowAlt(sf::Vector2f(820.f, rowHeight - 4.f));
            rowAlt.setPosition({230.f, py - 2.f});
            rowAlt.setFillColor(sf::Color(30, 32, 40, 120));
            window.draw(rowAlt);
        }

        sf::Color rowColor = (i == 0) ? goldText : (isNew ? sf::Color(255, 245, 180) : textFill);

        sf::Text posTxt(font, "#" + std::to_string(i + 1), 20);
        posTxt.setPosition({260.f, py});
        posTxt.setFillColor(rowColor);
        posTxt.setOutlineColor(textOutline);
        posTxt.setOutlineThickness(1.0f);
        window.draw(posTxt);

        sf::Text nameTxt(font, curr->data.name, 20);
        nameTxt.setPosition({410.f, py});
        nameTxt.setFillColor(rowColor);
        nameTxt.setOutlineColor(textOutline);
        nameTxt.setOutlineThickness(1.0f);
        window.draw(nameTxt);

        sf::Text scoreTxt(font, std::to_string(curr->data.score) + " pts", 20);
        scoreTxt.setPosition({860.f, py});
        scoreTxt.setFillColor(rowColor);
        scoreTxt.setOutlineColor(textOutline);
        scoreTxt.setOutlineThickness(1.0f);
        window.draw(scoreTxt);
    }

    sf::FloatRect algoBounds = getAlgorithmButtonBounds();
    nineSlice.draw(window, algoBounds, sf::Color(220, 210, 190));
    std::string algoName = (scoreManager.getCurrentAlgorithm() == SortAlgorithm::MERGE_SORT) ?
                           "Algoritmo: MergeSort O(n log n) [TAB]" :
                           "Algoritmo: Insercion O(n2) [TAB]";
    sf::Text algoTxt(font, algoName, 18);
    algoTxt.setFillColor(goldText);
    algoTxt.setOutlineColor(goldOutline);
    algoTxt.setOutlineThickness(1.2f);
    sf::FloatRect ab = algoTxt.getLocalBounds();
    algoTxt.setPosition({algoBounds.position.x + (algoBounds.size.x - ab.size.x) * 0.5f,
                         algoBounds.position.y + (algoBounds.size.y - ab.size.y) * 0.5f - 3.f});
    window.draw(algoTxt);

    sf::FloatRect backBounds = getBackButtonBounds();
    nineSlice.draw(window, backBounds, sf::Color(220, 210, 190));
    sf::Text backTxt(font, "VOLVER AL MENU [ESC / ENTER]", 18);
    backTxt.setFillColor(textFill);
    backTxt.setOutlineColor(textOutline);
    backTxt.setOutlineThickness(1.2f);
    sf::FloatRect bb = backTxt.getLocalBounds();
    backTxt.setPosition({backBounds.position.x + (backBounds.size.x - bb.size.x) * 0.5f,
                         backBounds.position.y + (backBounds.size.y - bb.size.y) * 0.5f - 3.f});
    window.draw(backTxt);
}

ScreenType ScoresScreen::getNextScreen() const {
    return nextScreen;
}

void ScoresScreen::clearNextScreen() {
    nextScreen = ScreenType::NONE;
}
