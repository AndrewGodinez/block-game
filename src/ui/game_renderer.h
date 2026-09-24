#pragma once
#include <SFML/Graphics.hpp>
#include "core/game_logic.h"

class GameRenderer {
private:
    sf::Font font;
    bool fontLoaded;
    sf::Texture blockTextures[9];
    bool texturesLoaded[9];
    sf::Texture bgTexture;
    bool bgLoaded;

    float cellSize;
    float boardOffsetX;
    float boardOffsetY;

    sf::Color getPieceFallbackColor(int pieceId) const;

public:
    GameRenderer(float cellSize = 31.0f, float offsetX = 485.0f, float offsetY = 38.0f);

    void render(sf::RenderWindow& window, const GameLogic& game);
    void drawCell(sf::RenderWindow& window, float pixelX, float pixelY, int pieceId, float customSize = 31.0f);
};
