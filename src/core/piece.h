#pragma once
#include "pieces.h"

struct Piece {
    PieceType type = PieceType::NONE;
    int rotation = 0;
    int col = 3;
    int row = 0;
};

Piece createPiece(PieceType type, int spawnCol = 3, int spawnRow = 0);

void moveLeft(Piece& piece);
void moveRight(Piece& piece);
void moveDown(Piece& piece);
void rotatePiece(Piece& piece);
void rotatePieceCounter(Piece& piece);

int getAbsoluteBlocks(const Piece& piece, BlockOffset outBlocks[4]);
