#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct Stack {
    node<T>* head = nullptr;
    int count = 0;
};

template<typename T>
bool pop(node<T> *&head);

template<typename T>
bool pop(node<T>*& head, T& data);

template<typename T>
bool push(node<T> *&head, T data);

template <typename T>
bool peek(node<T>* head, T& data);

template<typename T>
void clear(node<T>*& head);

template<typename T>
bool push(Stack<T>& stack, T data);

template<typename T>
bool pop(Stack<T>& stack, T& data);

template<typename T>
bool pop(Stack<T>& stack);

template<typename T>
bool peek(const Stack<T>& stack, T& data);

template<typename T>
bool isEmpty(const Stack<T>& stack);

template<typename T>
void clear(Stack<T>& stack);

#include "stack.tpp"
