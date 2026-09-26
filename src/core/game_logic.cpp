#include "game_logic.h"

GameLogic::GameLogic() : gameOver(false), linesClearedTotal(0), dropTimer(0.0f), dropInterval(0.8f), lineClearTimer(0.0f), gameTime(0.0f) {
    replayNextPieces[0] = PieceType::NONE;
    replayNextPieces[1] = PieceType::NONE;
    replayNextPieces[2] = PieceType::NONE;
    initBoard(board);
    spawnNextPiece();
    recordCurrentState();
}

GameLogic::~GameLogic() {
    clearBoard(board);
}

void GameLogic::reset() {
    clearBoard(board);
    initBoard(board);
    pieceBag.reset();
    holdSlot.clear();
    eventManager.reset();
    historyManager.reset();
    currentPiece = Piece();
    gameOver = false;
    linesClearedTotal = 0;
    dropTimer = 0.0f;
    lineClearTimer = 0.0f;
    gameTime = 0.0f;
    dropInterval = 0.8f;
    replayNextPieces[0] = PieceType::NONE;
    replayNextPieces[1] = PieceType::NONE;
    replayNextPieces[2] = PieceType::NONE;
    spawnNextPiece();
    recordCurrentState();
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
    if (historyManager.isReplaying()) {
        GameStateSnapshot snap;
        if (historyManager.updateReplay(dt, snap)) {
            applySnapshot(snap);
        }
        return;
    }

    if (lineClearTimer > 0.0f) {
        lineClearTimer -= dt;
        if (lineClearTimer < 0.0f) {
            lineClearTimer = 0.0f;
        }
    }

    if (gameOver) return;

    gameTime += dt;
    GameEvent ev;
    while (eventManager.pollEvent(gameTime, ev)) {
        switch (ev.type) {
            case EventType::SPEED_UP:
                setDropInterval(dropInterval * 0.85f);
                eventManager.scheduleEvent(EventType::SPEED_UP, gameTime + 45.0f);
                break;
            case EventType::MAGIC_PIECE:
                injectSpecialPiece();
                eventManager.scheduleEvent(EventType::MAGIC_PIECE, gameTime + 60.0f);
                break;
            case EventType::EARTHQUAKE:
                triggerEarthquake();
                eventManager.scheduleEvent(EventType::EARTHQUAKE, gameTime + 90.0f);
                break;
        }
    }

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
            recordCurrentState();
        }
    }
}

bool GameLogic::moveLeft() {
    if (gameOver || historyManager.isReplaying()) return false;
    Piece temp = currentPiece;
    ::moveLeft(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        recordCurrentState();
        return true;
    }
    return false;
}

bool GameLogic::moveRight() {
    if (gameOver || historyManager.isReplaying()) return false;
    Piece temp = currentPiece;
    ::moveRight(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        recordCurrentState();
        return true;
    }
    return false;
}

bool GameLogic::moveDown() {
    if (gameOver || historyManager.isReplaying()) return false;
    Piece temp = currentPiece;
    ::moveDown(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        recordCurrentState();
        return true;
    }
    return false;
}

bool GameLogic::rotate() {
    if (gameOver || historyManager.isReplaying()) return false;
    Piece temp = currentPiece;
    ::rotatePiece(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        recordCurrentState();
        return true;
    }
    return false;
}

bool GameLogic::rotateCounter() {
    if (gameOver || historyManager.isReplaying()) return false;
    Piece temp = currentPiece;
    ::rotatePieceCounter(temp);
    if (isValidPosition(temp)) {
        currentPiece = temp;
        recordCurrentState();
        return true;
    }
    return false;
}

int GameLogic::hardDrop() {
    if (gameOver || historyManager.isReplaying() || currentPiece.type == PieceType::NONE) return 0;
    int droppedRows = 0;
    while (true) {
        Piece temp = currentPiece;
        ::moveDown(temp);
        if (isValidPosition(temp)) {
            currentPiece = temp;
            droppedRows++;
        } else {
            break;
        }
    }
    lockCurrentPiece();
    spawnNextPiece();
    dropTimer = 0.0f;
    recordCurrentState();
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
    if (historyManager.isReplaying()) {
        for (int i = 0; i < 3; ++i) {
            outPieces[i] = replayNextPieces[i];
        }
        return;
    }
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
    if (gameOver || historyManager.isReplaying() || currentPiece.type == PieceType::NONE || !holdSlot.canSwap()) {
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

    recordCurrentState();
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

bool GameLogic::getNextEvent(GameEvent& outEvent) const {
    return eventManager.peekNextEvent(outEvent);
}

float GameLogic::getGameTime() const {
    return gameTime;
}

void GameLogic::injectSpecialPiece() {
    pieceBag.injectPieceFront(PieceType::SPECIAL);
}

void GameLogic::triggerEarthquake() {
    removeBottomRow(board);
    lineClearTimer = 0.16f;
}

void GameLogic::recordCurrentState() {
    if (historyManager.isReplaying()) return;
    GameStateSnapshot snap;
    snap.board = captureBoard(board);
    snap.currentPiece = currentPiece;
    snap.heldPiece = holdSlot.getHeldPiece();
    snap.canHold = holdSlot.canSwap();
    snap.linesClearedTotal = linesClearedTotal;
    snap.dropInterval = dropInterval;
    snap.gameTime = gameTime;
    pieceBag.peekNext(snap.nextPieces);
    historyManager.recordState(snap);
}

void GameLogic::applySnapshot(const GameStateSnapshot& snapshot) {
    restoreBoard(board, snapshot.board);
    currentPiece = snapshot.currentPiece;
    holdSlot.setHeldPiece(snapshot.heldPiece, snapshot.canHold);
    linesClearedTotal = snapshot.linesClearedTotal;
    dropInterval = snapshot.dropInterval;
    gameTime = snapshot.gameTime;
    for (int i = 0; i < 3; ++i) {
        replayNextPieces[i] = snapshot.nextPieces[i];
    }
}

bool GameLogic::undo() {
    if (gameOver || historyManager.isReplaying()) return false;
    GameStateSnapshot snap;
    if (historyManager.undo(snap)) {
        applySnapshot(snap);
        dropTimer = 0.0f;
        return true;
    }
    return false;
}

bool GameLogic::redo() {
    if (gameOver || historyManager.isReplaying()) return false;
    GameStateSnapshot snap;
    if (historyManager.redo(snap)) {
        applySnapshot(snap);
        dropTimer = 0.0f;
        return true;
    }
    return false;
}

bool GameLogic::canUndo() const {
    return historyManager.canUndo();
}

bool GameLogic::canRedo() const {
    return historyManager.canRedo();
}

void GameLogic::startReplay() {
    if (historyManager.getTotalSteps() == 0) return;
    historyManager.startReplay();
    GameStateSnapshot snap;
    if (historyManager.getHeadState(snap)) {
        applySnapshot(snap);
    }
}

void GameLogic::stopReplay() {
    historyManager.stopReplay();
}

bool GameLogic::isReplayMode() const {
    return historyManager.isReplaying();
}

bool GameLogic::isPlayingReplay() const {
    return historyManager.isPlayingReplay();
}

void GameLogic::toggleReplayPlay() {
    historyManager.togglePlayPause();
}

bool GameLogic::replayStepForward() {
    GameStateSnapshot snap;
    if (historyManager.replayStepForward(snap)) {
        applySnapshot(snap);
        return true;
    }
    return false;
}

bool GameLogic::replayStepBackward() {
    GameStateSnapshot snap;
    if (historyManager.replayStepBackward(snap)) {
        applySnapshot(snap);
        return true;
    }
    return false;
}

void GameLogic::restartReplay() {
    GameStateSnapshot snap;
    historyManager.restartReplay(snap);
    applySnapshot(snap);
}

int GameLogic::getHistoryStep() const {
    return historyManager.getCurrentStep();
}

int GameLogic::getHistoryTotal() const {
    return historyManager.getTotalSteps();
}

