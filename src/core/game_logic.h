#pragma once
#include "board.h"
#include "piece.h"

class GameLogic {
private:
    Board board;
    Piece currentPiece;
    bool gameOver;
    int linesClearedTotal;

    bool isValidPosition(const Piece& piece) const;

public:
    GameLogic();
    ~GameLogic();

    void reset();

    bool spawnPiece(PieceType type);

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
};
