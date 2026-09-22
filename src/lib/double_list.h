#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct doubleList {
    doubleNode<T>* head = nullptr;
    doubleNode<T>* tail = nullptr;
    doubleNode<T>* current = nullptr;
    int count = 0;
};
