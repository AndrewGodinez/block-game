#include "board.h"

static doubleList<int>* createEmptyRow(int cols) {
    auto* row = new doubleList<int>();
    for (int c = 0; c < cols; ++c) {
        insertTail(*row, 0);
    }
    return row;
}

void initBoard(Board& board) {
    clearBoard(board);
    for (int r = 0; r < board.rowCount; ++r) {
        doubleList<int>* row = createEmptyRow(board.colCount);
        insertTail(board.rows, row);
    }
}

void clearBoard(Board& board) {
    while (!isEmpty(board.rows)) {
        doubleList<int>* row = nullptr;
        removeHead(board.rows, row);
        if (row != nullptr) {
            clear(*row);
            delete row;
        }
    }
}

int getCell(const Board& board, int col, int row) {
    if (col < 0 || col >= board.colCount || row < 0 || row >= board.rowCount) {
        return -1;
    }
    doubleNode<doubleList<int>*>* rowNode = getNode(board.rows, row);
    if (rowNode == nullptr || rowNode->data == nullptr) {
        return -1;
    }
    int val = 0;
    if (get(*(rowNode->data), col, val)) {
        return val;
    }
    return -1;
}

bool setCell(Board& board, int col, int row, int value) {
    if (col < 0 || col >= board.colCount || row < 0 || row >= board.rowCount) {
        return false;
    }
    doubleNode<doubleList<int>*>* rowNode = getNode(board.rows, row);
    if (rowNode == nullptr || rowNode->data == nullptr) {
        return false;
    }
    return set(*(rowNode->data), col, value);
}

bool isRowFull(const doubleList<int>& row) {
    if (isEmpty(row)) return false;
    doubleNode<int>* curr = row.head;
    while (curr != nullptr) {
        if (curr->data == 0) return false;
        curr = curr->next;
    }
    return true;
}

bool isRowEmpty(const doubleList<int>& row) {
    if (isEmpty(row)) return true;
    doubleNode<int>* curr = row.head;
    while (curr != nullptr) {
        if (curr->data != 0) return false;
        curr = curr->next;
    }
    return true;
}

int clearFullRows(Board& board) {
    int cleared = 0;
    doubleNode<doubleList<int>*>* curr = board.rows.tail;

    while (curr != nullptr) {
        doubleNode<doubleList<int>*>* prevNode = curr->prev;

        if (isRowFull(*(curr->data))) {
            clear(*(curr->data));
            delete curr->data;
            removeNode(board.rows, curr);

            doubleList<int>* newRow = createEmptyRow(board.colCount);
            insertHead(board.rows, newRow);
            cleared++;
        }

        curr = prevNode;
    }

    return cleared;
}

bool removeBottomRow(Board& board) {
    if (board.rows.tail == nullptr) return false;
    doubleNode<doubleList<int>*>* tailNode = board.rows.tail;
    if (tailNode->data != nullptr) {
        clear(*(tailNode->data));
        delete tailNode->data;
    }
    removeNode(board.rows, tailNode);
    doubleList<int>* newRow = createEmptyRow(board.colCount);
    insertHead(board.rows, newRow);
    return true;
}

BoardSnapshot captureBoard(const Board& board) {
    BoardSnapshot snapshot;
    for (int r = 0; r < board.rowCount; ++r) {
        for (int c = 0; c < board.colCount; ++c) {
            snapshot.cells[r][c] = getCell(board, c, r);
        }
    }
    return snapshot;
}

void restoreBoard(Board& board, const BoardSnapshot& snapshot) {
    for (int r = 0; r < board.rowCount; ++r) {
        for (int c = 0; c < board.colCount; ++c) {
            setCell(board, c, r, snapshot.cells[r][c]);
        }
    }
}
