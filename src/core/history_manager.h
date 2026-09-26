#pragma once
#include "board.h"
#include "piece.h"
#include "pieces.h"
#include "lib/double_list.h"

struct GameStateSnapshot {
    BoardSnapshot board;
    Piece currentPiece;
    PieceType heldPiece;
    bool canHold;
    int linesClearedTotal;
    float dropInterval;
    float gameTime;
    PieceType nextPieces[3];
};

class HistoryManager {
private:
    doubleList<GameStateSnapshot> history;
    doubleNode<GameStateSnapshot>* current;
    doubleNode<GameStateSnapshot>* replayCursor;
    bool replaying;
    bool playingReplay;
    float replayTimer;
    float replayInterval;
    int currentIndex;
    int replayIndex;

    void truncateFuture();

public:
    HistoryManager();
    ~HistoryManager();

    void reset();
    void recordState(const GameStateSnapshot& snapshot);

    bool canUndo() const;
    bool canRedo() const;
    bool undo(GameStateSnapshot& outState);
    bool redo(GameStateSnapshot& outState);

    void startReplay();
    void stopReplay();
    bool isReplaying() const;
    bool isPlayingReplay() const;
    void togglePlayPause();

    bool replayStepForward(GameStateSnapshot& outState);
    bool replayStepBackward(GameStateSnapshot& outState);
    void restartReplay(GameStateSnapshot& outState);
    bool updateReplay(float dt, GameStateSnapshot& outState);

    int getCurrentStep() const;
    int getTotalSteps() const;
    bool getHeadState(GameStateSnapshot& outState) const;
};
