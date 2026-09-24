#pragma once
#include "board.h"
#include "piece.h"

class GameLogic {
private:
    Board board;
    Piece currentPiece;
    bool gameOver;
    int linesClearedTotal;
    float dropTimer;
    float dropInterval;

    bool isValidPosition(const Piece& piece) const;
    PieceType getRandomPieceType();

public:
    GameLogic();
    ~GameLogic();

    void reset();

    bool spawnPiece(PieceType type);
    void update(float dt);

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate();
    bool rotateCounter();
    int hardDrop();

    int lockCurrentPiece();

    bool isGameOver() const;
    int getLinesClearedTotal() const;
    const Board& getBoard() const;
    const Piece& getCurrentPiece() const;
    float getDropInterval() const;
    void setDropInterval(float interval);
};
