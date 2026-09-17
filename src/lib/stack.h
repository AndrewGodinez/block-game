#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct Stack {
    node<T>* head = nullptr;
};

template<typename T>
bool pop(node<T> *&head);

template<typename T>
bool push(node<T> *&head, T data);

#include "stack.tpp"
