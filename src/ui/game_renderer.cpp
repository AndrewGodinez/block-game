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
    if (pieceId >= 1 && pieceId <= 7 && texturesLoaded[pieceId]) {
        sf::Sprite sprite(blockTextures[pieceId]);
        float scale = customSize / 256.f;
        sprite.setScale({scale, scale});
        sprite.setPosition({pixelX, pixelY});
        window.draw(sprite);
    } else {
        sf::RectangleShape rect({customSize - 1.f, customSize - 1.f});
        rect.setPosition({pixelX, pixelY});
        rect.setFillColor(pieceId == 8 ? sf::Color(255, 215, 0) : getPieceFallbackColor(pieceId));
        rect.setOutlineThickness(1.f);
        rect.setOutlineColor(sf::Color(220, 220, 220));
        window.draw(rect);
    }
}

void GameRenderer::render(sf::RenderWindow& window, const GameLogic& game) {
    const float SCREEN_W = 1280.f;
    const float SCREEN_H = 700.f;
    const float LEFT_X = 140.f;
    const float RIGHT_X = 870.f;

    if (bgLoaded) {
        sf::Sprite bgSprite(bgTexture);
        float sx = SCREEN_W / static_cast<float>(bgTexture.getSize().x);
        float sy = SCREEN_H / static_cast<float>(bgTexture.getSize().y);
        bgSprite.setScale({sx, sy});
        window.draw(bgSprite);
    }

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

        sf::Text linesText(font);
        linesText.setString("Lineas: " + std::to_string(game.getLinesClearedTotal()));
        linesText.setCharacterSize(22);
        if (clearTimer > 0.0f) {
            linesText.setFillColor(sf::Color(255, 245, 160));
            linesText.setOutlineColor(sf::Color(85, 60, 25));
        } else {
            linesText.setFillColor(sf::Color(135, 220, 145));
            linesText.setOutlineColor(sf::Color(35, 60, 35));
        }
        linesText.setOutlineThickness(1.2f);
        linesText.setPosition({LEFT_X + 40.f, boardOffsetY + 65.f});
        window.draw(linesText);

        sf::Text holdTitle(font);
        holdTitle.setString("EN ESPERA (Hold: [C])");
        holdTitle.setCharacterSize(20);
        holdTitle.setFillColor(sf::Color(240, 210, 140));
        holdTitle.setOutlineColor(textOutline);
        holdTitle.setOutlineThickness(1.2f);
        holdTitle.setPosition({LEFT_X + 35.f, boardOffsetY + 105.f});
        window.draw(holdTitle);

        PieceType heldPiece = game.getHeldPiece();
        if (heldPiece != PieceType::NONE) {
            BlockOffset hBlocks[4];
            int hCount = getPieceOffsets(heldPiece, 0, hBlocks);
            int hId = getPieceId(heldPiece);
            float previewCellSize = 22.f;
            float holdStartX = LEFT_X + 105.f;
            float holdStartY = boardOffsetY + 145.f;
            if (heldPiece == PieceType::I) {
                holdStartX = LEFT_X + 95.f;
                holdStartY = boardOffsetY + 135.f;
            } else if (heldPiece == PieceType::O) {
                holdStartX = LEFT_X + 115.f;
                holdStartY = boardOffsetY + 145.f;
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
        controlsTitle.setPosition({LEFT_X + 40.f, boardOffsetY + 225.f});
        window.draw(controlsTitle);

        const char* controlsGuide = 
            "[A / D]     Mover\n"
            "[W]         Rotar\n"
            "[S]         Bajar\n"
            "[ESPACIO]   Caer\n"
            "[C]         Guardar\n"
            "[R]         Reiniciar\n"
            "[ESC]       Salir";

        sf::Text controlsText(font);
        controlsText.setString(controlsGuide);
        controlsText.setCharacterSize(18);
        controlsText.setFillColor(textFill);
        controlsText.setOutlineColor(textOutline);
        controlsText.setOutlineThickness(1.0f);
        controlsText.setPosition({LEFT_X + 40.f, boardOffsetY + 260.f});
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
            } else if (nextPieces[i] == PieceType::O) {
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

    if (game.isGameOver()) {
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
            goText.setPosition({boardOffsetX + 35.f, boardOffsetY + 240.f});
            window.draw(goText);

            sf::Color textFill(245, 235, 210);
            sf::Color textOutline(65, 48, 30);

            sf::Text subText(font);
            subText.setString("Presiona [R] para reiniciar");
            subText.setCharacterSize(20);
            subText.setFillColor(textFill);
            subText.setOutlineColor(textOutline);
            subText.setOutlineThickness(1.2f);
            subText.setPosition({boardOffsetX + 20.f, boardOffsetY + 310.f});
            window.draw(subText);
        }
    }
}
