#include "game_logic.h"

GameLogic::GameLogic() : gameOver(false), linesClearedTotal(0), dropTimer(0.0f), dropInterval(0.8f), lineClearTimer(0.0f) {
    initBoard(board);
    spawnNextPiece();
}

GameLogic::~GameLogic() {
    clearBoard(board);
}

void GameLogic::reset() {
    clearBoard(board);
    initBoard(board);
    pieceBag.reset();
    holdSlot.clear();
    currentPiece = Piece();
    gameOver = false;
    linesClearedTotal = 0;
    dropTimer = 0.0f;
    lineClearTimer = 0.0f;
    spawnNextPiece();
}

bool GameLogic::isValidPosition(const Piece& piece) const {
    BlockOffset blocks[4];
    int count = getAbsoluteBlocks(piece, blocks);
    for (int i = 0; i < count; ++i) {
        int c = blocks[i].col;
        int r = blocks[i].row;
        if (c < 0 || c >= board.colCount || r < 0 || r >= board.rowCount) {
            return false;
        }
        if (getCell(board, c, r) != 0) {
            return false;
        }
    }
    return true;
}

bool GameLogic::spawnPiece(PieceType type) {
    currentPiece = createPiece(type, 3, 0);
    if (!isValidPosition(currentPiece)) {
        gameOver = true;
        return false;
    }
    return true;
}

bool GameLogic::spawnNextPiece() {
    return spawnPiece(pieceBag.nextPiece());
}

void GameLogic::update(float dt) {
    if (lineClearTimer > 0.0f) {
        lineClearTimer -= dt;
        if (lineClearTimer < 0.0f) {
            lineClearTimer = 0.0f;
        }
    }

    if (gameOver) return;

    if (currentPiece.type == PieceType::NONE) {
        spawnNextPiece();
        if (gameOver) return;
    }

    dropTimer += dt;
    if (dropTimer >= dropInterval) {
        dropTimer = 0.0f;
        if (!moveDown()) {
            lockCurrentPiece();
            spawnNextPiece();
        }
    }
}

bool GameLogic::moveLeft() {
    if (gameOver) return false;
    Piece temp = currentPiece;
    ::moveLeft(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        return true;
    }
    return false;
}

bool GameLogic::moveRight() {
    if (gameOver) return false;
    Piece temp = currentPiece;
    ::moveRight(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        return true;
    }
    return false;
}

bool GameLogic::moveDown() {
    if (gameOver) return false;
    Piece temp = currentPiece;
    ::moveDown(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        return true;
    }
    return false;
}

bool GameLogic::rotate() {
    if (gameOver) return false;
    Piece temp = currentPiece;
    ::rotatePiece(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        return true;
    }
    return false;
}

bool GameLogic::rotateCounter() {
    if (gameOver) return false;
    Piece temp = currentPiece;
    ::rotatePieceCounter(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        return true;
    }
    return false;
}

int GameLogic::hardDrop() {
    if (gameOver || currentPiece.type == PieceType::NONE) return 0;
    int droppedRows = 0;
    while (moveDown()) {
        droppedRows++;
    }
    lockCurrentPiece();
    spawnNextPiece();
    dropTimer = 0.0f;
    return droppedRows;
}

int GameLogic::lockCurrentPiece() {
    if (gameOver || currentPiece.type == PieceType::NONE) return 0;

    BlockOffset blocks[4];
    int count = getAbsoluteBlocks(currentPiece, blocks);
    int pieceId = getPieceId(currentPiece.type);

    for (int i = 0; i < count; ++i) {
        setCell(board, blocks[i].col, blocks[i].row, pieceId);
    }

    if (currentPiece.type == PieceType::SPECIAL && count > 0) {
        int rowToClear = blocks[0].row;
        if (rowToClear >= 0 && rowToClear < board.rowCount) {
            for (int c = 0; c < board.colCount; ++c) {
                setCell(board, c, rowToClear, 1);
            }
        }
    }

    int cleared = clearFullRows(board);
    linesClearedTotal += cleared;
    if (cleared > 0) {
        lineClearTimer = 0.16f;
    }
    currentPiece = Piece();
    holdSlot.resetTurn();
    return cleared;
}

void GameLogic::peekNextPieces(PieceType outPieces[3]) const {
    pieceBag.peekNext(outPieces);
}

bool GameLogic::isGameOver() const {
    return gameOver;
}

int GameLogic::getLinesClearedTotal() const {
    return linesClearedTotal;
}

const Board& GameLogic::getBoard() const {
    return board;
}

const Piece& GameLogic::getCurrentPiece() const {
    return currentPiece;
}

float GameLogic::getDropInterval() const {
    return dropInterval;
}

void GameLogic::setDropInterval(float interval) {
    if (interval > 0.05f) {
        dropInterval = interval;
    }
}

bool GameLogic::holdCurrentPiece() {
    if (gameOver || currentPiece.type == PieceType::NONE || !holdSlot.canSwap()) {
        return false;
    }

    PieceType held = holdSlot.swap(currentPiece.type);
    if (held == PieceType::NONE) {
        currentPiece = createPiece(pieceBag.nextPiece(), 3, 0);
    } else {
        currentPiece = createPiece(held, 3, 0);
    }

    dropTimer = 0.0f;

    if (!isValidPosition(currentPiece)) {
        gameOver = true;
    }

    return true;
}

PieceType GameLogic::getHeldPiece() const {
    return holdSlot.getHeldPiece();
}

bool GameLogic::canHold() const {
    return holdSlot.canSwap();
}

float GameLogic::getLineClearTimer() const {
    return lineClearTimer;
}
