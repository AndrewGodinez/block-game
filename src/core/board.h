#pragma once
#include "lib/double_list.h"

struct Board {
    doubleList<doubleList<int>*> rows;
    int rowCount = 20;
    int colCount = 10;
};

void initBoard(Board& board);
void clearBoard(Board& board);

int getCell(const Board& board, int col, int row);
bool setCell(Board& board, int col, int row, int value);

bool isRowFull(const doubleList<int>& row);
bool isRowEmpty(const doubleList<int>& row);

int clearFullRows(Board& board);
