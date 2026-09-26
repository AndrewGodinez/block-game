#include "hold_slot.h"
#include "lib/stack.h"

struct HoldSlotImpl {
    Stack<PieceType> slot;
    bool canHold;
};

HoldSlot::HoldSlot() : pImpl(new HoldSlotImpl) {
    pImpl->canHold = true;
}

HoldSlot::~HoldSlot() {
    clear();
    delete pImpl;
}

bool HoldSlot::canSwap() const {
    return pImpl->canHold;
}

void HoldSlot::resetTurn() {
    pImpl->canHold = true;
}

void HoldSlot::clear() {
    ::clear(pImpl->slot);
    pImpl->canHold = true;
}

PieceType HoldSlot::swap(PieceType current) {
    if (!pImpl->canHold || current == PieceType::NONE) {
        return PieceType::NONE;
    }

    if (isEmpty(pImpl->slot)) {
        push(pImpl->slot, current);
        pImpl->canHold = false;
        return PieceType::NONE;
    }

    PieceType held = PieceType::NONE;
    pop(pImpl->slot, held);
    push(pImpl->slot, current);
    pImpl->canHold = false;
    return held;
}

PieceType HoldSlot::getHeldPiece() const {
    PieceType held = PieceType::NONE;
    if (!isEmpty(pImpl->slot)) {
        peek(pImpl->slot, held);
    }
    return held;
}

bool HoldSlot::hasPiece() const {
    return !isEmpty(pImpl->slot);
}

void HoldSlot::setHeldPiece(PieceType type, bool allowedToSwap) {
    ::clear(pImpl->slot);
    if (type != PieceType::NONE) {
        push(pImpl->slot, type);
    }
    pImpl->canHold = allowedToSwap;
}
