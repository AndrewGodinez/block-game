#pragma once
#include "board.h"
#include "piece.h"
#include "piece_bag.h"
#include "hold_slot.h"
#include "event_manager.h"
#include "history_manager.h"

class GameLogic {
private:
    Board board;
    Piece currentPiece;
    PieceBag pieceBag;
    HoldSlot holdSlot;
    EventManager eventManager;
    HistoryManager historyManager;
    bool gameOver;
    int linesClearedTotal;
    int score;
    float dropTimer;
    float dropInterval;
    float lineClearTimer;
    float gameTime;
    PieceType replayNextPieces[3];

    bool isValidPosition(const Piece& piece) const;
    void recordCurrentState();
    void applySnapshot(const GameStateSnapshot& snapshot);

public:
    GameLogic();
    ~GameLogic();

    void reset();

    bool spawnPiece(PieceType type);
    bool spawnNextPiece();
    void update(float dt);

    bool moveLeft();
    bool moveRight();
    bool moveDown();
    bool rotate();
    bool rotateCounter();
    int hardDrop();

    int lockCurrentPiece();
    bool holdCurrentPiece();

    void peekNextPieces(PieceType outPieces[3]) const;
    PieceType getHeldPiece() const;
    bool canHold() const;

    bool isGameOver() const;
    int getLinesClearedTotal() const;
    int getScore() const;
    const Board& getBoard() const;
    const Piece& getCurrentPiece() const;
    float getDropInterval() const;
    void setDropInterval(float interval);
    float getLineClearTimer() const;
    bool getNextEvent(GameEvent& outEvent) const;
    float getGameTime() const;
    void injectSpecialPiece();
    void triggerEarthquake();

    bool undo();
    bool redo();
    bool canUndo() const;
    bool canRedo() const;

    void startReplay();
    void stopReplay();
    bool isReplayMode() const;
    bool isPlayingReplay() const;
    void toggleReplayPlay();
    bool replayStepForward();
    bool replayStepBackward();
    void restartReplay();
    int getHistoryStep() const;
    int getHistoryTotal() const;
};
