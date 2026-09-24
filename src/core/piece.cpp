#include "piece.h"

Piece createPiece(PieceType type, int spawnCol, int spawnRow) {
    Piece piece;
    piece.type = type;
    piece.rotation = 0;
    piece.col = spawnCol;
    piece.row = spawnRow;
    return piece;
}

void moveLeft(Piece& piece) {
    piece.col--;
}

void moveRight(Piece& piece) {
    piece.col++;
}

void moveDown(Piece& piece) {
    piece.row++;
}

void rotatePiece(Piece& piece) {
    piece.rotation = (piece.rotation + 1) % 4;
}

void rotatePieceCounter(Piece& piece) {
    piece.rotation = (piece.rotation + 3) % 4;
}

int getAbsoluteBlocks(const Piece& piece, BlockOffset outBlocks[4]) {
    BlockOffset localOffsets[4];
    int count = getPieceOffsets(piece.type, piece.rotation, localOffsets);
    for (int i = 0; i < count; ++i) {
        outBlocks[i].col = piece.col + localOffsets[i].col;
        outBlocks[i].row = piece.row + localOffsets[i].row;
    }
    return count;
}
