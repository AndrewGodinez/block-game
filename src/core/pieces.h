#pragma once

enum class PieceType {
    NONE = 0,
    I,
    O,
    T,
    S,
    Z,
    J,
    L,
    SPECIAL
};

struct BlockOffset {
    int col;
    int row;
};

int getPieceOffsets(PieceType type, int rotation, BlockOffset outBlocks[4]);

int getPieceId(PieceType type);
