#include "piece_bag.h"
#include <cstdlib>
#include <utility>

void PieceBag::refillBag() {
    PieceType bag[7] = {
        PieceType::I, PieceType::O, PieceType::T, PieceType::S,
        PieceType::Z, PieceType::J, PieceType::L
    };
    for (int i = 6; i > 0; --i) {
        int j = std::rand() % (i + 1);
        std::swap(bag[i], bag[j]);
    }
    for (int i = 0; i < 7; ++i) {
        enqueue(queue, bag[i]);
    }
}

PieceBag::PieceBag() {
    refillBag();
    refillBag();
}

PieceBag::~PieceBag() {
    clear(queue);
}

void PieceBag::reset() {
    clear(queue);
    refillBag();
    refillBag();
}

PieceType PieceBag::nextPiece() {
    if (queue.count <= 4) {
        refillBag();
    }
    PieceType p = PieceType::NONE;
    if (dequeue(queue, p)) {
        return p;
    }
    return PieceType::NONE;
}

void PieceBag::peekNext(PieceType outPieces[3]) const {
    const node<PieceType>* curr = queue.front;
    for (int i = 0; i < 3; ++i) {
        if (curr != nullptr) {
            outPieces[i] = curr->data;
            curr = curr->next;
        } else {
            outPieces[i] = PieceType::NONE;
        }
    }
}

int PieceBag::getCount() const {
    return queue.count;
}

bool PieceBag::isEmpty() const {
    return ::isEmpty(queue);
}
