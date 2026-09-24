#include "pieces.h"

int getPieceOffsets(PieceType type, int rotation, BlockOffset outBlocks[4]) {
    int r = ((rotation % 4) + 4) % 4;

    switch (type) {
        case PieceType::I: {
            static const BlockOffset iRot[4][4] = {
                {{0, 1}, {1, 1}, {2, 1}, {3, 1}},
                {{2, 0}, {2, 1}, {2, 2}, {2, 3}},
                {{0, 2}, {1, 2}, {2, 2}, {3, 2}},
                {{1, 0}, {1, 1}, {1, 2}, {1, 3}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = iRot[r][i];
            return 4;
        }
        case PieceType::O: {
            static const BlockOffset oRot[4] = {
                {1, 0}, {2, 0}, {1, 1}, {2, 1}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = oRot[i];
            return 4;
        }
        case PieceType::T: {
            static const BlockOffset tRot[4][4] = {
                {{1, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {1, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {1, 2}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = tRot[r][i];
            return 4;
        }
        case PieceType::S: {
            static const BlockOffset sRot[4][4] = {
                {{1, 0}, {2, 0}, {0, 1}, {1, 1}},
                {{1, 0}, {1, 1}, {2, 1}, {2, 2}},
                {{1, 1}, {2, 1}, {0, 2}, {1, 2}},
                {{0, 0}, {0, 1}, {1, 1}, {1, 2}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = sRot[r][i];
            return 4;
        }
        case PieceType::Z: {
            static const BlockOffset zRot[4][4] = {
                {{0, 0}, {1, 0}, {1, 1}, {2, 1}},
                {{2, 0}, {1, 1}, {2, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {1, 2}, {2, 2}},
                {{1, 0}, {0, 1}, {1, 1}, {0, 2}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = zRot[r][i];
            return 4;
        }
        case PieceType::J: {
            static const BlockOffset jRot[4][4] = {
                {{0, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {2, 0}, {1, 1}, {1, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {2, 2}},
                {{1, 0}, {1, 1}, {0, 2}, {1, 2}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = jRot[r][i];
            return 4;
        }
        case PieceType::L: {
            static const BlockOffset lRot[4][4] = {
                {{2, 0}, {0, 1}, {1, 1}, {2, 1}},
                {{1, 0}, {1, 1}, {1, 2}, {2, 2}},
                {{0, 1}, {1, 1}, {2, 1}, {0, 2}},
                {{0, 0}, {1, 0}, {1, 1}, {1, 2}}
            };
            for (int i = 0; i < 4; ++i) outBlocks[i] = lRot[r][i];
            return 4;
        }
        case PieceType::SPECIAL: {
            outBlocks[0] = {1, 1};
            return 1;
        }
        case PieceType::NONE:
        default:
            return 0;
    }
}

int getPieceId(PieceType type) {
    switch (type) {
        case PieceType::I: return 1;
        case PieceType::O: return 2;
        case PieceType::T: return 3;
        case PieceType::S: return 4;
        case PieceType::Z: return 5;
        case PieceType::J: return 6;
        case PieceType::L: return 7;
        case PieceType::SPECIAL: return 8;
        case PieceType::NONE:
        default:
            return 0;
    }
}
