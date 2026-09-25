#pragma once
#include "board.h"
#include "piece.h"
#include "piece_bag.h"
#include "hold_slot.h"

class GameLogic {
private:
    Board board;
    Piece currentPiece;
    PieceBag pieceBag;
    HoldSlot holdSlot;
    bool gameOver;
    int linesClearedTotal;
    float dropTimer;
    float dropInterval;
    float lineClearTimer;

    bool isValidPosition(const Piece& piece) const;

public:
    GameLogic();
    ~GameLogic();

    void reset();

    bool spawnPiece(PieceType type);
    bool spawnNextPiece();
    void update(float dt);

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate();
    bool rotateCounter();
    int hardDrop();

    int lockCurrentPiece();
    bool holdCurrentPiece();

    void peekNextPieces(PieceType outPieces[3]) const;
    PieceType getHeldPiece() const;
    bool canHold() const;

    bool isGameOver() const;
    int getLinesClearedTotal() const;
    const Board& getBoard() const;
    const Piece& getCurrentPiece() const;
    float getDropInterval() const;
    void setDropInterval(float interval);
    float getLineClearTimer() const;
};
