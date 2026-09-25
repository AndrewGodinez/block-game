#pragma once
#include "lib/queue.h"
#include "pieces.h"

class PieceBag {
private:
    Queue<PieceType> queue;

    void refillBag();

public:
    PieceBag();
    ~PieceBag();

    void reset();

    PieceType nextPiece();
    void peekNext(PieceType outPieces[3]) const;

    int getCount() const;
    bool isEmpty() const;
};
