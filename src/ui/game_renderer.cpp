#include "game_renderer.h"
#include <string>

GameRenderer::GameRenderer(float cSize, float offX, float offY)
    : fontLoaded(false), bgLoaded(false), cellSize(cSize), boardOffsetX(offX), boardOffsetY(offY) {
    fontLoaded = font.openFromFile("src/assets/Deutsch.ttf") || font.openFromFile("../src/assets/Deutsch.ttf");

    for (int i = 0; i < 9; ++i) {
        texturesLoaded[i] = false;
    }

    const std::string textureFiles[7] = {
        "src/assets/block_i.png", "src/assets/block_o.png", "src/assets/block_t.png",
        "src/assets/block_s.png", "src/assets/block_z.png", "src/assets/block_j.png",
        "src/assets/block_l.png"
    };

    for (int i = 0; i < 7; ++i) {
        texturesLoaded[i + 1] = blockTextures[i + 1].loadFromFile(textureFiles[i]) ||
                                blockTextures[i + 1].loadFromFile("../" + textureFiles[i]);
    }

    bgLoaded = bgTexture.loadFromFile("src/assets/bg.jpg") || bgTexture.loadFromFile("../src/assets/bg.jpg");
    panelNineSlice.loadFromFile("src/assets/ui_generic.png");
}

sf::Color GameRenderer::getPieceFallbackColor(int pieceId) const {
    switch (pieceId) {
        case 1: return sf::Color(0, 220, 220);
        case 2: return sf::Color(230, 220, 0);
        case 3: return sf::Color(160, 40, 220);
        case 4: return sf::Color(40, 210, 40);
        case 5: return sf::Color(220, 40, 40);
        case 6: return sf::Color(40, 80, 220);
        case 7: return sf::Color(230, 140, 20);
        case 8: return sf::Color(255, 215, 0);
        default: return sf::Color(35, 38, 48);
    }
}

void GameRenderer::drawCell(sf::RenderWindow& window, float pixelX, float pixelY, int pieceId, float customSize) {
    if (pieceId == 8) {
        sf::RectangleShape outerGlow({customSize + 8.f, customSize + 8.f});
        outerGlow.setPosition({pixelX - 4.f, pixelY - 4.f});
        outerGlow.setFillColor(sf::Color(255, 215, 0, 45));
        window.draw(outerGlow);

        sf::RectangleShape midGlow({customSize + 4.f, customSize + 4.f});
        midGlow.setPosition({pixelX - 2.f, pixelY - 2.f});
        midGlow.setFillColor(sf::Color(255, 230, 80, 85));
        window.draw(midGlow);

        if (texturesLoaded[2]) {
            sf::Sprite sprite(blockTextures[2]);
            sprite.setColor(sf::Color(255, 245, 175));
            float scale = customSize / 256.f;
            sprite.setScale({scale, scale});
            sprite.setPosition({pixelX, pixelY});
            window.draw(sprite);
        } else {
            sf::RectangleShape rect({customSize - 1.f, customSize - 1.f});
            rect.setPosition({pixelX, pixelY});
            rect.setFillColor(sf::Color(255, 215, 0));
            window.draw(rect);
        }

        sf::RectangleShape rim({customSize, customSize});
        rim.setFillColor(sf::Color::Transparent);
        rim.setOutlineThickness(1.5f);
        rim.setOutlineColor(sf::Color(255, 255, 200, 220));
        rim.setPosition({pixelX, pixelY});
        window.draw(rim);
        return;
    }

    if (pieceId >= 1 && pieceId <= 7 && texturesLoaded[pieceId]) {
        sf::Sprite sprite(blockTextures[pieceId]);
        float scale = customSize / 256.f;
        sprite.setScale({scale, scale});
        sprite.setPosition({pixelX, pixelY});
        window.draw(sprite);
    } else {
        sf::RectangleShape rect({customSize - 1.f, customSize - 1.f});
        rect.setPosition({pixelX, pixelY});
        rect.setFillColor(getPieceFallbackColor(pieceId));
        rect.setOutlineThickness(1.f);
        rect.setOutlineColor(sf::Color(220, 220, 220));
        window.draw(rect);
    }
}

void GameRenderer::drawBackground(sf::RenderWindow& window) const {
    const float SCREEN_W = 1280.f;
    const float SCREEN_H = 700.f;

    if (bgLoaded) {
        sf::Sprite bgSprite(bgTexture);
        float sx = SCREEN_W / static_cast<float>(bgTexture.getSize().x);
        float sy = SCREEN_H / static_cast<float>(bgTexture.getSize().y);
        bgSprite.setScale({sx, sy});
        window.draw(bgSprite);
    }
}

void GameRenderer::render(sf::RenderWindow& window, const GameLogic& game) {
    const float LEFT_X = 140.f;
    const float RIGHT_X = 870.f;

    drawBackground(window);

    const Board& board = game.getBoard();
    float boardWidth = board.colCount * cellSize;
    float boardHeight = board.rowCount * cellSize;
    const float boardMargin = 35.f;

    panelNineSlice.draw(window, sf::FloatRect(sf::Vector2f(boardOffsetX - boardMargin, boardOffsetY - boardMargin),
                                              sf::Vector2f(boardWidth + 2.f * boardMargin, boardHeight + 2.f * boardMargin)));

    sf::RectangleShape boardInner({boardWidth, boardHeight});
    boardInner.setPosition({boardOffsetX, boardOffsetY});
    boardInner.setFillColor(sf::Color(27, 27, 27, 210));
    window.draw(boardInner);

    for (int r = 0; r < board.rowCount; ++r) {
        for (int c = 0; c < board.colCount; ++c) {
            float px = boardOffsetX + c * cellSize;
            float py = boardOffsetY + r * cellSize;
            int val = getCell(board, c, r);
            if (val > 0) {
                drawCell(window, px, py, val, cellSize);
            } else {
                sf::RectangleShape gridCell({cellSize - 2.f, cellSize - 2.f});
                gridCell.setPosition({px + 1.f, py + 1.f});
                gridCell.setFillColor(sf::Color(50, 50, 50));
                gridCell.setOutlineThickness(1.f);
                gridCell.setOutlineColor(sf::Color(100, 100, 100));
                window.draw(gridCell);
            }
        }
    }

    const Piece& current = game.getCurrentPiece();
    if (current.type != PieceType::NONE) {
        BlockOffset blocks[4];
        int count = getAbsoluteBlocks(current, blocks);
        int pId = getPieceId(current.type);
        for (int i = 0; i < count; ++i) {
            float px = boardOffsetX + blocks[i].col * cellSize;
            float py = boardOffsetY + blocks[i].row * cellSize;
            drawCell(window, px, py, pId, cellSize);
        }
    }

    float clearTimer = game.getLineClearTimer();
    if (clearTimer > 0.0f) {
        float ratio = clearTimer / 0.16f;
        if (ratio > 1.0f) ratio = 1.0f;

        sf::RectangleShape flash({boardWidth, boardHeight});
        flash.setPosition({boardOffsetX, boardOffsetY});
        flash.setFillColor(sf::Color(255, 230, 160, static_cast<std::uint8_t>(ratio * 75)));
        window.draw(flash);

        sf::RectangleShape rimHighlight({boardWidth, boardHeight});
        rimHighlight.setPosition({boardOffsetX, boardOffsetY});
        rimHighlight.setFillColor(sf::Color::Transparent);
        rimHighlight.setOutlineThickness(2.f);
        rimHighlight.setOutlineColor(sf::Color(255, 220, 120, static_cast<std::uint8_t>(ratio * 160)));
        window.draw(rimHighlight);
    }

    if (fontLoaded) {
        panelNineSlice.draw(window, sf::FloatRect(sf::Vector2f(LEFT_X, boardOffsetY), sf::Vector2f(270.f, 500.f)));

        sf::Color textFill(245, 235, 210);
        sf::Color textOutline(65, 48, 30);

        sf::Text titleText(font);
        titleText.setString("MEDIEVAL BLOCK");
        titleText.setCharacterSize(28);
        titleText.setFillColor(textFill);
        titleText.setOutlineColor(textOutline);
        titleText.setOutlineThickness(1.5f);
        titleText.setPosition({LEFT_X + 35.f, boardOffsetY + 20.f});
        window.draw(titleText);

        sf::Text scoreText(font);
        scoreText.setString("Puntaje: " + std::to_string(game.getScore()));
        scoreText.setCharacterSize(22);
        scoreText.setFillColor(sf::Color(255, 230, 140));
        scoreText.setOutlineColor(textOutline);
        scoreText.setOutlineThickness(1.2f);
        scoreText.setPosition({LEFT_X + 40.f, boardOffsetY + 55.f});
        window.draw(scoreText);

        sf::Text linesText(font);
        linesText.setString("Lineas: " + std::to_string(game.getLinesClearedTotal()));
        linesText.setCharacterSize(20);
        if (clearTimer > 0.0f) {
            linesText.setFillColor(sf::Color(255, 245, 160));
            linesText.setOutlineColor(sf::Color(85, 60, 25));
        } else {
            linesText.setFillColor(sf::Color(135, 220, 145));
            linesText.setOutlineColor(sf::Color(35, 60, 35));
        }
        linesText.setOutlineThickness(1.2f);
        linesText.setPosition({LEFT_X + 40.f, boardOffsetY + 82.f});
        window.draw(linesText);

        GameEvent nextEv;
        if (game.getNextEvent(nextEv)) {
            float remaining = nextEv.triggerTime - game.getGameTime();
            if (remaining < 0.0f) remaining = 0.0f;
            std::string evName = "Aceleracion";
            if (nextEv.type == EventType::MAGIC_PIECE) evName = "Pieza Dorada";
            else if (nextEv.type == EventType::EARTHQUAKE) evName = "Terremoto";

            sf::Text evTitle(font);
            evTitle.setString("EVENTO:");
            evTitle.setCharacterSize(17);
            evTitle.setFillColor(sf::Color(240, 210, 140));
            evTitle.setOutlineColor(textOutline);
            evTitle.setOutlineThickness(1.2f);
            evTitle.setPosition({LEFT_X + 40.f, boardOffsetY + 108.f});
            window.draw(evTitle);

            sf::Text evDesc(font);
            evDesc.setString(evName + " (" + std::to_string(static_cast<int>(remaining)) + "s)");
            evDesc.setCharacterSize(17);
            evDesc.setFillColor(textFill);
            evDesc.setOutlineColor(textOutline);
            evDesc.setOutlineThickness(1.0f);
            evDesc.setPosition({LEFT_X + 40.f, boardOffsetY + 128.f});
            window.draw(evDesc);
        }

        sf::Text holdTitle(font);
        holdTitle.setString("EN ESPERA (Hold: [C])");
        holdTitle.setCharacterSize(20);
        holdTitle.setFillColor(sf::Color(240, 210, 140));
        holdTitle.setOutlineColor(textOutline);
        holdTitle.setOutlineThickness(1.2f);
        holdTitle.setPosition({LEFT_X + 35.f, boardOffsetY + 155.f});
        window.draw(holdTitle);

        PieceType heldPiece = game.getHeldPiece();
        if (heldPiece != PieceType::NONE) {
            BlockOffset hBlocks[4];
            int hCount = getPieceOffsets(heldPiece, 0, hBlocks);
            int hId = getPieceId(heldPiece);
            float previewCellSize = 22.f;
            float holdStartX = LEFT_X + 105.f;
            float holdStartY = boardOffsetY + 190.f;
            if (heldPiece == PieceType::I) {
                holdStartX = LEFT_X + 95.f;
                holdStartY = boardOffsetY + 180.f;
            } else if (heldPiece == PieceType::O || heldPiece == PieceType::SPECIAL) {
                holdStartX = LEFT_X + 115.f;
                holdStartY = boardOffsetY + 190.f;
            }
            for (int b = 0; b < hCount; ++b) {
                float px = holdStartX + hBlocks[b].col * previewCellSize;
                float py = holdStartY + hBlocks[b].row * previewCellSize;
                drawCell(window, px, py, hId, previewCellSize);
            }
        }

        sf::Text controlsTitle(font);
        controlsTitle.setString("CONTROLES");
        controlsTitle.setCharacterSize(20);
        controlsTitle.setFillColor(sf::Color(240, 210, 140));
        controlsTitle.setOutlineColor(textOutline);
        controlsTitle.setOutlineThickness(1.2f);
        controlsTitle.setPosition({LEFT_X + 40.f, boardOffsetY + 250.f});
        window.draw(controlsTitle);

        const char* controlsGuide = game.isReplayMode() ?
            "[ESPACIO]   Play/Pausa\n"
            "[A / <]     Paso Atras\n"
            "[D / >]     Paso Adelante\n"
            "[R]         Reiniciar\n"
            "[ESC]       Salir Replay" :
            "[A / D]     Mover\n"
            "[W]         Rotar\n"
            "[S]         Bajar\n"
            "[ESPACIO]   Caer\n"
            "[C]         Guardar\n"
            "[Z / Y]     Undo/Redo\n"
            "[R]         Reiniciar\n"
            "[ESC]       Salir";

        sf::Text controlsText(font);
        controlsText.setString(controlsGuide);
        controlsText.setCharacterSize(18);
        controlsText.setFillColor(textFill);
        controlsText.setOutlineColor(textOutline);
        controlsText.setOutlineThickness(1.0f);
        controlsText.setPosition({LEFT_X + 40.f, boardOffsetY + 280.f});
        window.draw(controlsText);

        panelNineSlice.draw(window, sf::FloatRect(sf::Vector2f(RIGHT_X, boardOffsetY), sf::Vector2f(270.f, 500.f)));

        sf::Text nextTitle(font);
        nextTitle.setString("SIGUIENTES (Cola)");
        nextTitle.setCharacterSize(22);
        nextTitle.setFillColor(textFill);
        nextTitle.setOutlineColor(textOutline);
        nextTitle.setOutlineThickness(1.5f);
        nextTitle.setPosition({RIGHT_X + 35.f, boardOffsetY + 25.f});
        window.draw(nextTitle);

        PieceType nextPieces[3];
        game.peekNextPieces(nextPieces);

        for (int i = 0; i < 3; ++i) {
            if (nextPieces[i] == PieceType::NONE) continue;

            BlockOffset pBlocks[4];
            int count = getPieceOffsets(nextPieces[i], 0, pBlocks);
            int pId = getPieceId(nextPieces[i]);
            float previewCellSize = 22.f;

            float startX = RIGHT_X + 100.f;
            float startY = boardOffsetY + 85.f + i * 110.f;
            if (nextPieces[i] == PieceType::I) {
                startX = RIGHT_X + 90.f;
                startY = boardOffsetY + 75.f + i * 110.f;
            } else if (nextPieces[i] == PieceType::O || nextPieces[i] == PieceType::SPECIAL) {
                startX = RIGHT_X + 110.f;
                startY = boardOffsetY + 85.f + i * 110.f;
            }

            for (int b = 0; b < count; ++b) {
                float px = startX + pBlocks[b].col * previewCellSize;
                float py = startY + pBlocks[b].row * previewCellSize;
                drawCell(window, px, py, pId, previewCellSize);
            }
        }
    }

    if (game.isReplayMode()) {
        sf::RectangleShape banner({boardWidth, 40.f});
        banner.setPosition({boardOffsetX, boardOffsetY - 45.f});
        banner.setFillColor(sf::Color(35, 30, 25, 235));
        banner.setOutlineThickness(1.5f);
        banner.setOutlineColor(sf::Color(240, 210, 140));
        window.draw(banner);

        if (fontLoaded) {
            sf::Color textOutline(65, 48, 30);
            sf::Text replayTitle(font);
            std::string stateStr = game.isPlayingReplay() ? "PLAY" : "PAUSA";
            replayTitle.setString("REPLAY [" + stateStr + "]  " + std::to_string(game.getHistoryStep()) + " / " + std::to_string(game.getHistoryTotal()));
            replayTitle.setCharacterSize(18);
            replayTitle.setFillColor(sf::Color(255, 230, 120));
            replayTitle.setOutlineColor(textOutline);
            replayTitle.setOutlineThickness(1.2f);
            replayTitle.setPosition({boardOffsetX + 20.f, boardOffsetY - 37.f});
            window.draw(replayTitle);
        }
    } else if (game.isGameOver()) {
        sf::RectangleShape overlay({boardWidth, boardHeight});
        overlay.setPosition({boardOffsetX, boardOffsetY});
        overlay.setFillColor(sf::Color(0, 0, 0, 215));
        window.draw(overlay);

        if (fontLoaded) {
            sf::Text goText(font);
            goText.setString("GAME OVER");
            goText.setCharacterSize(42);
            goText.setFillColor(sf::Color(240, 60, 60));
            goText.setOutlineColor(sf::Color(70, 15, 15));
            goText.setOutlineThickness(2.0f);
            goText.setPosition({boardOffsetX + 35.f, boardOffsetY + 200.f});
            window.draw(goText);

            sf::Color textFill(245, 235, 210);
            sf::Color textOutline(65, 48, 30);

            sf::Text scoreOver(font);
            scoreOver.setString("Puntaje: " + std::to_string(game.getScore()));
            scoreOver.setCharacterSize(24);
            scoreOver.setFillColor(sf::Color(255, 235, 140));
            scoreOver.setOutlineColor(textOutline);
            scoreOver.setOutlineThickness(1.5f);
            scoreOver.setPosition({boardOffsetX + 55.f, boardOffsetY + 255.f});
            window.draw(scoreOver);

            sf::Text subText(font);
            subText.setString("[P] Ver Replay\n[ENTER] Ver Puntajes\n[R] Reiniciar");
            subText.setCharacterSize(20);
            subText.setFillColor(textFill);
            subText.setOutlineColor(textOutline);
            subText.setOutlineThickness(1.2f);
            subText.setPosition({boardOffsetX + 45.f, boardOffsetY + 295.f});
            window.draw(subText);
        }
    }
}

sf::FloatRect GameRenderer::getPlayButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(500.f, 320.f), sf::Vector2f(280.f, 60.f));
}

sf::FloatRect GameRenderer::getExitButtonBounds() const {
    return sf::FloatRect(sf::Vector2f(500.f, 405.f), sf::Vector2f(280.f, 60.f));
}

void GameRenderer::renderMenu(sf::RenderWindow& window, int selectedIndex) {
    const float SCREEN_W = 1280.f;
    const float SCREEN_H = 700.f;

    if (bgLoaded) {
        sf::Sprite bgSprite(bgTexture);
        float sx = SCREEN_W / static_cast<float>(bgTexture.getSize().x);
        float sy = SCREEN_H / static_cast<float>(bgTexture.getSize().y);
        bgSprite.setScale({sx, sy});
        window.draw(bgSprite);
    }

    if (!fontLoaded) return;

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
    title.setPosition({(SCREEN_W - titleBounds.size.x) * 0.5f, 150.f});
    window.draw(title);

    sf::Text subtitle(font);
    subtitle.setString("TETRIS MEDIEVAL - UNA EIF207");
    subtitle.setCharacterSize(22);
    subtitle.setFillColor(textFill);
    subtitle.setOutlineColor(textOutline);
    subtitle.setOutlineThickness(1.2f);
    sf::FloatRect subBounds = subtitle.getLocalBounds();
    subtitle.setPosition({(SCREEN_W - subBounds.size.x) * 0.5f, 230.f});
    window.draw(subtitle);

    sf::FloatRect playBounds = getPlayButtonBounds();
    sf::Color playTint = (selectedIndex == 0) ? sf::Color(255, 245, 205) : sf::Color(180, 180, 180, 220);
    panelNineSlice.draw(window, playBounds, playTint);

    if (selectedIndex == 0) {
        sf::RectangleShape highlight(playBounds.size);
        highlight.setPosition(playBounds.position);
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineThickness(2.f);
        highlight.setOutlineColor(sf::Color(255, 225, 130, 200));
        window.draw(highlight);
    }

    sf::Text playText(font);
    playText.setString(selectedIndex == 0 ? "> JUGAR <" : "JUGAR");
    playText.setCharacterSize(26);
    playText.setFillColor(selectedIndex == 0 ? goldText : textFill);
    playText.setOutlineColor(selectedIndex == 0 ? goldOutline : textOutline);
    playText.setOutlineThickness(1.5f);
    sf::FloatRect ptb = playText.getLocalBounds();
    playText.setPosition({playBounds.position.x + (playBounds.size.x - ptb.size.x) * 0.5f,
                          playBounds.position.y + (playBounds.size.y - ptb.size.y) * 0.5f - 4.f});
    window.draw(playText);

    sf::FloatRect exitBounds = getExitButtonBounds();
    sf::Color exitTint = (selectedIndex == 1) ? sf::Color(255, 245, 205) : sf::Color(180, 180, 180, 220);
    panelNineSlice.draw(window, exitBounds, exitTint);

    if (selectedIndex == 1) {
        sf::RectangleShape highlight(exitBounds.size);
        highlight.setPosition(exitBounds.position);
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineThickness(2.f);
        highlight.setOutlineColor(sf::Color(255, 225, 130, 200));
        window.draw(highlight);
    }

    sf::Text exitText(font);
    exitText.setString(selectedIndex == 1 ? "> SALIR <" : "SALIR");
    exitText.setCharacterSize(26);
    exitText.setFillColor(selectedIndex == 1 ? goldText : textFill);
    exitText.setOutlineColor(selectedIndex == 1 ? goldOutline : textOutline);
    exitText.setOutlineThickness(1.5f);
    sf::FloatRect etb = exitText.getLocalBounds();
    exitText.setPosition({exitBounds.position.x + (exitBounds.size.x - etb.size.x) * 0.5f,
                          exitBounds.position.y + (exitBounds.size.y - etb.size.y) * 0.5f - 4.f});
    window.draw(exitText);

    sf::Text hint(font);
    hint.setString("[W / S / Flechas]  Navegar      [ENTER / ESPACIO / Clic]  Seleccionar");
    hint.setCharacterSize(18);
    hint.setFillColor(textFill);
    hint.setOutlineColor(textOutline);
    hint.setOutlineThickness(1.0f);
    sf::FloatRect htb = hint.getLocalBounds();
    hint.setPosition({(SCREEN_W - htb.size.x) * 0.5f, 530.f});
    window.draw(hint);
}

