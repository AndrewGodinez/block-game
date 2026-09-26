#include "history_manager.h"

HistoryManager::HistoryManager()
    : current(nullptr), replayCursor(nullptr), replaying(false),
      playingReplay(false), replayTimer(0.0f), replayInterval(0.18f),
      currentIndex(0), replayIndex(0) {
}

HistoryManager::~HistoryManager() {
    reset();
}

void HistoryManager::truncateFuture() {
    if (current == nullptr) return;
    doubleNode<GameStateSnapshot>* toDelete = current->next;
    while (toDelete != nullptr) {
        doubleNode<GameStateSnapshot>* next = toDelete->next;
        delete toDelete;
        history.count--;
        toDelete = next;
    }
    current->next = nullptr;
    history.tail = current;
}

void HistoryManager::reset() {
    clear(history);
    current = nullptr;
    replayCursor = nullptr;
    replaying = false;
    playingReplay = false;
    replayTimer = 0.0f;
    currentIndex = 0;
    replayIndex = 0;
}

void HistoryManager::recordState(const GameStateSnapshot& snapshot) {
    if (current != nullptr && current != history.tail) {
        truncateFuture();
    }
    insertTail(history, snapshot);
    current = history.tail;
    currentIndex = history.count;
}

bool HistoryManager::canUndo() const {
    return !replaying && current != nullptr && current->prev != nullptr;
}

bool HistoryManager::canRedo() const {
    return !replaying && current != nullptr && current->next != nullptr;
}

bool HistoryManager::undo(GameStateSnapshot& outState) {
    if (!canUndo()) return false;
    current = current->prev;
    currentIndex--;
    outState = current->data;
    return true;
}

bool HistoryManager::redo(GameStateSnapshot& outState) {
    if (!canRedo()) return false;
    current = current->next;
    currentIndex++;
    outState = current->data;
    return true;
}

void HistoryManager::startReplay() {
    replaying = true;
    playingReplay = false;
    replayCursor = history.head;
    replayIndex = (history.head != nullptr) ? 1 : 0;
    replayTimer = 0.0f;
}

void HistoryManager::stopReplay() {
    replaying = false;
    playingReplay = false;
    replayCursor = nullptr;
    replayIndex = 0;
}

bool HistoryManager::isReplaying() const {
    return replaying;
}

bool HistoryManager::isPlayingReplay() const {
    return playingReplay;
}

void HistoryManager::togglePlayPause() {
    if (!replaying) return;
    playingReplay = !playingReplay;
}

bool HistoryManager::replayStepForward(GameStateSnapshot& outState) {
    if (!replaying || replayCursor == nullptr || replayCursor->next == nullptr) {
        return false;
    }
    replayCursor = replayCursor->next;
    replayIndex++;
    outState = replayCursor->data;
    return true;
}

bool HistoryManager::replayStepBackward(GameStateSnapshot& outState) {
    if (!replaying || replayCursor == nullptr || replayCursor->prev == nullptr) {
        return false;
    }
    replayCursor = replayCursor->prev;
    replayIndex--;
    outState = replayCursor->data;
    return true;
}

void HistoryManager::restartReplay(GameStateSnapshot& outState) {
    if (!replaying || history.head == nullptr) return;
    replayCursor = history.head;
    replayIndex = 1;
    outState = replayCursor->data;
}

bool HistoryManager::updateReplay(float dt, GameStateSnapshot& outState) {
    if (!replaying || !playingReplay || replayCursor == nullptr) {
        return false;
    }
    replayTimer += dt;
    if (replayTimer >= replayInterval) {
        replayTimer = 0.0f;
        if (replayCursor->next != nullptr) {
            replayCursor = replayCursor->next;
            replayIndex++;
            outState = replayCursor->data;
            return true;
        } else {
            playingReplay = false;
        }
    }
    return false;
}

int HistoryManager::getCurrentStep() const {
    return replaying ? replayIndex : currentIndex;
}

int HistoryManager::getTotalSteps() const {
    return history.count;
}

bool HistoryManager::getHeadState(GameStateSnapshot& outState) const {
    if (history.head == nullptr) return false;
    outState = history.head->data;
    return true;
}
