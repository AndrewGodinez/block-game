#include "score_manager.h"
#include <fstream>

ScoreManager::ScoreManager()
    : currentAlgorithm(SortAlgorithm::MERGE_SORT), scoresFilePath(resolveScoresPath("scores.txt")) {
    load(scoresFilePath);
}

ScoreManager::ScoreManager(const std::string& filepath)
    : currentAlgorithm(SortAlgorithm::MERGE_SORT), scoresFilePath(resolveScoresPath(filepath)) {
    load(scoresFilePath);
}

ScoreManager::~ScoreManager() {
    clear(scores);
}

void ScoreManager::load(const std::string& filename) {
    if (!filename.empty()) {
        scoresFilePath = resolveScoresPath(filename);
    }
    loadScores(scores, scoresFilePath);
    sort(currentAlgorithm);
}

void ScoreManager::save(const std::string& filename) const {
    std::string path = filename.empty() ? scoresFilePath : resolveScoresPath(filename);
    saveScores(path, scores);
}

void ScoreManager::insertionSort() {
    insertionSort(scores);
}

void ScoreManager::mergeSort() {
    mergeSort(scores);
}

void ScoreManager::sort(SortAlgorithm algo) {
    currentAlgorithm = algo;
    sortScores(scores, algo);
}

void ScoreManager::toggleAlgorithm() {
    if (currentAlgorithm == SortAlgorithm::MERGE_SORT) {
        currentAlgorithm = SortAlgorithm::INSERTION_SORT;
    } else {
        currentAlgorithm = SortAlgorithm::MERGE_SORT;
    }
    sort(currentAlgorithm);
}

SortAlgorithm ScoreManager::getCurrentAlgorithm() const {
    return currentAlgorithm;
}

bool ScoreManager::qualifiesForTop10(int score) const {
    return qualifiesForTop10(score, scores);
}

void ScoreManager::addScore(const std::string& name, int score) {
    insertTail(scores, ScoreEntry{name, score});
    sort(currentAlgorithm);
    save(scoresFilePath);
}

const doubleList<ScoreEntry>& ScoreManager::getScores() const {
    return scores;
}

int ScoreManager::getCount() const {
    return scores.count;
}

void ScoreManager::insertionSort(doubleList<ScoreEntry>& list) {
    if (list.count <= 1 || list.head == nullptr) return;

    doubleNode<ScoreEntry>* curr = list.head->next;
    while (curr != nullptr) {
        ScoreEntry key = curr->data;
        doubleNode<ScoreEntry>* prevNode = curr->prev;

        while (prevNode != nullptr && prevNode->data.score < key.score) {
            prevNode->next->data = prevNode->data;
            prevNode = prevNode->prev;
        }

        if (prevNode == nullptr) {
            list.head->data = key;
        } else {
            prevNode->next->data = key;
        }

        curr = curr->next;
    }
}

doubleNode<ScoreEntry>* ScoreManager::split(doubleNode<ScoreEntry>* head) {
    doubleNode<ScoreEntry>* fast = head;
    doubleNode<ScoreEntry>* slow = head;
    while (fast->next != nullptr && fast->next->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
    }
    doubleNode<ScoreEntry>* temp = slow->next;
    slow->next = nullptr;
    if (temp != nullptr) {
        temp->prev = nullptr;
    }
    return temp;
}

doubleNode<ScoreEntry>* ScoreManager::merge(doubleNode<ScoreEntry>* first, doubleNode<ScoreEntry>* second) {
    if (first == nullptr) return second;
    if (second == nullptr) return first;

    doubleNode<ScoreEntry>* newHead = nullptr;
    doubleNode<ScoreEntry>* tail = nullptr;

    if (first->data.score >= second->data.score) {
        newHead = first;
        first = first->next;
    } else {
        newHead = second;
        second = second->next;
    }
    newHead->prev = nullptr;
    tail = newHead;

    while (first != nullptr && second != nullptr) {
        if (first->data.score >= second->data.score) {
            tail->next = first;
            first->prev = tail;
            tail = first;
            first = first->next;
        } else {
            tail->next = second;
            second->prev = tail;
            tail = second;
            second = second->next;
        }
    }

    if (first != nullptr) {
        tail->next = first;
        first->prev = tail;
    } else if (second != nullptr) {
        tail->next = second;
        second->prev = tail;
    }

    return newHead;
}

doubleNode<ScoreEntry>* ScoreManager::mergeSortNodes(doubleNode<ScoreEntry>* node) {
    if (node == nullptr || node->next == nullptr) {
        return node;
    }
    doubleNode<ScoreEntry>* second = split(node);
    node = mergeSortNodes(node);
    second = mergeSortNodes(second);
    return merge(node, second);
}

void ScoreManager::mergeSort(doubleList<ScoreEntry>& list) {
    if (list.count <= 1 || list.head == nullptr) return;

    list.head = mergeSortNodes(list.head);

    doubleNode<ScoreEntry>* curr = list.head;
    while (curr != nullptr && curr->next != nullptr) {
        curr = curr->next;
    }
    list.tail = curr;
}

void ScoreManager::sortScores(doubleList<ScoreEntry>& list, SortAlgorithm algo) {
    if (algo == SortAlgorithm::INSERTION_SORT) {
        insertionSort(list);
    } else {
        mergeSort(list);
    }
}

std::string ScoreManager::resolveScoresPath(const std::string& preferred) {
    if (!preferred.empty()) {
        std::ifstream test(preferred);
        if (test.is_open()) return preferred;
    }

    const std::string candidates[] = {
        "scores.txt",
        "../scores.txt",
        "sandbox/scores.txt",
        "../sandbox/scores.txt"
    };

    for (const auto& path : candidates) {
        std::ifstream test(path);
        if (test.is_open()) return path;
    }

    return "scores.txt";
}

void ScoreManager::loadScores(doubleList<ScoreEntry>& list, const std::string& filename) {
    clear(list);
    std::string path = resolveScoresPath(filename);
    std::ifstream file(path);

    if (!file.is_open()) {
        insertTail(list, ScoreEntry{"Arthur", 12000});
        insertTail(list, ScoreEntry{"Lancelot", 9500});
        insertTail(list, ScoreEntry{"Guinevere", 7200});
        insertTail(list, ScoreEntry{"Merlin", 5400});
        insertTail(list, ScoreEntry{"Galahad", 3100});
        insertTail(list, ScoreEntry{"Caballero", 250});
        return;
    }

    std::string name;
    int sc = 0;
    while (file >> name >> sc) {
        insertTail(list, ScoreEntry{name, sc});
    }

    if (isEmpty(list)) {
        insertTail(list, ScoreEntry{"Arthur", 12000});
        insertTail(list, ScoreEntry{"Lancelot", 9500});
        insertTail(list, ScoreEntry{"Guinevere", 7200});
        insertTail(list, ScoreEntry{"Merlin", 5400});
        insertTail(list, ScoreEntry{"Galahad", 3100});
        insertTail(list, ScoreEntry{"Caballero", 250});
    }
}

void ScoreManager::saveScores(const std::string& filename, const doubleList<ScoreEntry>& scores) {
    std::string path = resolveScoresPath(filename);
    std::ofstream file(path);
    if (!file.is_open()) return;

    doubleNode<ScoreEntry>* curr = scores.head;
    int count = 0;
    while (curr != nullptr && count < 10) {
        file << curr->data.name << " " << curr->data.score << "\n";
        curr = curr->next;
        count++;
    }
}

bool ScoreManager::qualifiesForTop10(int score, const doubleList<ScoreEntry>& list) {
    if (score <= 0) return false;
    if (list.count < 10) return true;

    doubleNode<ScoreEntry>* curr = list.head;
    if (curr == nullptr) return true;

    int minScore = curr->data.score;
    int count = 0;
    while (curr != nullptr && count < 10) {
        if (curr->data.score < minScore) {
            minScore = curr->data.score;
        }
        curr = curr->next;
        count++;
    }

    return score > minScore;
}
