#pragma once
#include "pieces.h"

struct HoldSlotImpl;

class HoldSlot {
private:
    HoldSlotImpl* pImpl;

public:
    HoldSlot();
    ~HoldSlot();

    HoldSlot(const HoldSlot&) = delete;
    HoldSlot& operator=(const HoldSlot&) = delete;

    bool canSwap() const;
    void resetTurn();
    void clear();

    PieceType swap(PieceType current);
    PieceType getHeldPiece() const;
    bool hasPiece() const;
    void setHeldPiece(PieceType type, bool allowedToSwap);
};
