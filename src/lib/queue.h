#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct Queue {
    node<T>* front = nullptr;
    node<T>* rear = nullptr;
};

template<typename T, typename U = T>
bool enqueue(node<T>*& front, node<T>*& rear, U data);

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear, T& data);

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear);

template<typename T>
bool peek(node<T>* front, T& data);

template<typename T, typename U = T>
bool enqueue(Queue<T>& q, U data);

template<typename T>
bool dequeue(Queue<T>& q, T& data);

template<typename T>
bool dequeue(Queue<T>& q);

template<typename T>
bool peek(const Queue<T>& q, T& data);

template<typename T>
bool isEmpty(const Queue<T>& q);

#include "queue.tpp"