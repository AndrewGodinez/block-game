#include "game_screen.h"

GameScreen::GameScreen(GameRenderer& rend)
    : renderer(rend), nextScreen(ScreenType::NONE), finalScore(0), wasReplaying(false) {
}

void GameScreen::startNewGame() {
    game.reset();
    finalScore = 0;
    wasReplaying = false;
    nextScreen = ScreenType::NONE;
}

int GameScreen::getFinalScore() const {
    return finalScore;
}

void GameScreen::handleEvent(const sf::Event& event) {
    const auto* key = event.getIf<sf::Event::KeyPressed>();
    if (!key) return;

    if (game.isReplayMode()) {
        if (key->code == sf::Keyboard::Key::Escape || key->code == sf::Keyboard::Key::Enter) {
            game.stopReplay();
            nextScreen = ScreenType::SCORES;
        } else if (key->code == sf::Keyboard::Key::Space) {
            game.toggleReplayPlay();
        } else if (key->code == sf::Keyboard::Key::Right || key->code == sf::Keyboard::Key::D) {
            game.replayStepForward();
        } else if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
            game.replayStepBackward();
        } else if (key->code == sf::Keyboard::Key::R) {
            game.restartReplay();
        }
    } else if (game.isGameOver()) {
        if (key->code == sf::Keyboard::Key::P) {
            game.startReplay();
            wasReplaying = true;
        } else if (key->code == sf::Keyboard::Key::Enter) {
            nextScreen = ScreenType::SCORES;
        } else if (key->code == sf::Keyboard::Key::R) {
            startNewGame();
        } else if (key->code == sf::Keyboard::Key::Escape) {
            nextScreen = ScreenType::MENU;
        }
    } else {
        if (key->code == sf::Keyboard::Key::Escape) {
            nextScreen = ScreenType::MENU;
        } else if (key->code == sf::Keyboard::Key::Left || key->code == sf::Keyboard::Key::A) {
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

void GameScreen::update(float dt) {
    if (game.isGameOver()) {
        finalScore = game.getScore();
    }

    bool wasPlayingReplayBefore = game.isPlayingReplay();
    game.update(dt);

    if (game.isReplayMode() && wasPlayingReplayBefore && !game.isPlayingReplay()) {
        if (game.getHistoryStep() >= game.getHistoryTotal()) {
            game.stopReplay();
            nextScreen = ScreenType::SCORES;
        }
    }
}

void GameScreen::render(sf::RenderWindow& window) {
    renderer.render(window, game);
}

ScreenType GameScreen::getNextScreen() const {
    return nextScreen;
}

void GameScreen::clearNextScreen() {
    nextScreen = ScreenType::NONE;
}
