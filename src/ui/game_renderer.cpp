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

    if (bgLoaded) {
        sf::Sprite bgSprite(bgTexture);
        float sx = SCREEN_W / static_cast<float>(bgTexture.getSize().x);
        float sy = SCREEN_H / static_cast<float>(bgTexture.getSize().y);
        bgSprite.setScale({sx, sy});
        window.draw(bgSprite);
    }

    sf::RectangleShape mainPlate({SCREEN_W - 40.f, SCREEN_H - 40.f});
    mainPlate.setPosition({20.f, 20.f});
    mainPlate.setFillColor(sf::Color(10, 15, 22, 215));
    mainPlate.setOutlineThickness(2.f);
    mainPlate.setOutlineColor(sf::Color(190, 160, 110));
    window.draw(mainPlate);

    const Board& board = game.getBoard();
    float boardWidth = board.colCount * cellSize;
    float boardHeight = board.rowCount * cellSize;

    sf::RectangleShape boardFrame({boardWidth + 6.f, boardHeight + 6.f});
    boardFrame.setPosition({boardOffsetX - 3.f, boardOffsetY - 3.f});
    boardFrame.setFillColor(sf::Color(12, 14, 18));
    boardFrame.setOutlineThickness(3.f);
    boardFrame.setOutlineColor(sf::Color(180, 150, 90));
    window.draw(boardFrame);

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
                gridCell.setFillColor(sf::Color(20, 24, 30));
                gridCell.setOutlineThickness(1.f);
                gridCell.setOutlineColor(sf::Color(32, 38, 48));
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

    if (fontLoaded) {
        sf::RectangleShape statsBox({270.f, 400.f});
        statsBox.setPosition({LEFT_X, boardOffsetY});
        statsBox.setFillColor(sf::Color(18, 22, 30));
        statsBox.setOutlineThickness(2.f);
        statsBox.setOutlineColor(sf::Color(120, 100, 70));
        window.draw(statsBox);

        sf::Text titleText(font);
        titleText.setString("MEDIEVAL BLOCK");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color(240, 220, 160));
        titleText.setPosition({LEFT_X + 20.f, boardOffsetY + 15.f});
        window.draw(titleText);

        sf::Text linesText(font);
        linesText.setString("Lineas: " + std::to_string(game.getLinesClearedTotal()));
        linesText.setCharacterSize(24);
        linesText.setFillColor(sf::Color(100, 220, 120));
        linesText.setPosition({LEFT_X + 25.f, boardOffsetY + 70.f});
        window.draw(linesText);

        sf::Text controlsTitle(font);
        controlsTitle.setString("CONTROLES");
        controlsTitle.setCharacterSize(22);
        controlsTitle.setFillColor(sf::Color(240, 200, 80));
        controlsTitle.setPosition({LEFT_X + 25.f, boardOffsetY + 130.f});
        window.draw(controlsTitle);

        const char* controlsGuide = 
            "[A / D]     Mover\n"
            "[W]         Rotar\n"
            "[S]         Bajar\n"
            "[ESPACIO]   Caer\n"
            "[R]         Reiniciar\n"
            "[ESC]       Salir";

        sf::Text controlsText(font);
        controlsText.setString(controlsGuide);
        controlsText.setCharacterSize(18);
        controlsText.setFillColor(sf::Color(190, 195, 210));
        controlsText.setPosition({LEFT_X + 25.f, boardOffsetY + 175.f});
        window.draw(controlsText);

        const float RIGHT_X = 870.f;
        sf::RectangleShape nextBox({270.f, 415.f});
        nextBox.setPosition({RIGHT_X, boardOffsetY});
        nextBox.setFillColor(sf::Color(18, 22, 30));
        nextBox.setOutlineThickness(2.f);
        nextBox.setOutlineColor(sf::Color(120, 100, 70));
        window.draw(nextBox);

        sf::Text nextTitle(font);
        nextTitle.setString("SIGUIENTES (Cola)");
        nextTitle.setCharacterSize(22);
        nextTitle.setFillColor(sf::Color(240, 220, 160));
        nextTitle.setPosition({RIGHT_X + 25.f, boardOffsetY + 15.f});
        window.draw(nextTitle);

        PieceType nextPieces[3];
        game.peekNextPieces(nextPieces);

        for (int i = 0; i < 3; ++i) {
            if (nextPieces[i] == PieceType::NONE) continue;
            float slotY = boardOffsetY + 55.f + i * 115.f;
            sf::RectangleShape slot({220.f, 95.f});
            slot.setPosition({RIGHT_X + 25.f, slotY});
            slot.setFillColor(sf::Color(12, 15, 20));
            slot.setOutlineThickness(1.f);
            slot.setOutlineColor(sf::Color(60, 65, 80));
            window.draw(slot);

            BlockOffset pBlocks[4];
            int count = getPieceOffsets(nextPieces[i], 0, pBlocks);
            int pId = getPieceId(nextPieces[i]);
            float previewCellSize = 22.f;

            float startX = RIGHT_X + 75.f;
            float startY = slotY + 22.f;
            if (nextPieces[i] == PieceType::I) {
                startX = RIGHT_X + 65.f;
                startY = slotY + 15.f;
            } else if (nextPieces[i] == PieceType::O) {
                startX = RIGHT_X + 85.f;
                startY = slotY + 22.f;
            }

            for (int b = 0; b < count; ++b) {
                float px = startX + pBlocks[b].col * previewCellSize;
                float py = startY + pBlocks[b].row * previewCellSize;
                drawCell(window, px, py, pId, previewCellSize);
            }
        }
    }

    if (game.isGameOver()) {
        sf::RectangleShape overlay({boardWidth + 6.f, boardHeight + 6.f});
        overlay.setPosition({boardOffsetX - 3.f, boardOffsetY - 3.f});
        overlay.setFillColor(sf::Color(0, 0, 0, 205));
        window.draw(overlay);

        if (fontLoaded) {
            sf::Text goText(font);
            goText.setString("GAME OVER");
            goText.setCharacterSize(42);
            goText.setFillColor(sf::Color(240, 60, 60));
            goText.setPosition({boardOffsetX + 35.f, boardOffsetY + 240.f});
            window.draw(goText);

            sf::Text subText(font);
            subText.setString("Presiona [R] para reiniciar");
            subText.setCharacterSize(20);
            subText.setFillColor(sf::Color(220, 220, 220));
            subText.setPosition({boardOffsetX + 20.f, boardOffsetY + 310.f});
            window.draw(subText);
        }
    }
}
