#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct Queue {
    node<T>* front = nullptr;
    node<T>* rear = nullptr;
    int count = 0;
};

template<typename T>
bool enqueue(node<T>*& front, node<T>*& rear, T data);

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear, T& data);

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear);

template<typename T>
bool peek(node<T>* front, T& data);

template<typename T>
void clear(node<T>*& front, node<T>*& rear);

template<typename T>
bool enqueue(Queue<T>& queue, T data);

template<typename T>
bool dequeue(Queue<T>& queue, T& data);

template<typename T>
bool dequeue(Queue<T>& queue);

template<typename T>
bool peek(const Queue<T>& queue, T& data);

template<typename T>
bool isEmpty(const Queue<T>& queue);

template<typename T>
void clear(Queue<T>& queue);

#include "queue.tpp"