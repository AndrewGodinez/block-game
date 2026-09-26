#pragma once
#include <string>
#include "lib/double_list.h"

struct ScoreEntry {
    std::string name;
    int score = 0;
};

enum class SortAlgorithm {
    MERGE_SORT,
    INSERTION_SORT
};

class ScoreManager {
private:
    doubleList<ScoreEntry> scores;
    SortAlgorithm currentAlgorithm;
    std::string scoresFilePath;

    static doubleNode<ScoreEntry>* split(doubleNode<ScoreEntry>* head);
    static doubleNode<ScoreEntry>* merge(doubleNode<ScoreEntry>* first, doubleNode<ScoreEntry>* second);
    static doubleNode<ScoreEntry>* mergeSortNodes(doubleNode<ScoreEntry>* node);

public:
    ScoreManager();
    explicit ScoreManager(const std::string& filepath);
    ~ScoreManager();

    ScoreManager(const ScoreManager&) = delete;
    ScoreManager& operator=(const ScoreManager&) = delete;

    void load(const std::string& filename = "");
    void save(const std::string& filename = "") const;

    void insertionSort();
    void mergeSort();
    void sort(SortAlgorithm algo);
    void toggleAlgorithm();
    SortAlgorithm getCurrentAlgorithm() const;

    bool qualifiesForTop10(int score) const;
    void addScore(const std::string& name, int score);

    const doubleList<ScoreEntry>& getScores() const;
    int getCount() const;

    static void insertionSort(doubleList<ScoreEntry>& list);
    static void mergeSort(doubleList<ScoreEntry>& list);
    static void sortScores(doubleList<ScoreEntry>& list, SortAlgorithm algo);
    static std::string resolveScoresPath(const std::string& preferred = "scores.txt");
    static void loadScores(doubleList<ScoreEntry>& list, const std::string& filename = "");
    static void saveScores(const std::string& filename, const doubleList<ScoreEntry>& scores);
    static bool qualifiesForTop10(int score, const doubleList<ScoreEntry>& list);
};
