#pragma once
#include <iostream>
#include "node.h"

template<typename T>
struct doubleList {
    doubleNode<T>* head = nullptr;
    doubleNode<T>* tail = nullptr;
    int count = 0;
};

template<typename T>
bool insertHead(doubleNode<T>*& head, doubleNode<T>*& tail, T data);

template<typename T>
bool insertTail(doubleNode<T>*& head, doubleNode<T>*& tail, T data);

template<typename T>
bool removeHead(doubleNode<T>*& head, doubleNode<T>*& tail);

template<typename T>
bool removeHead(doubleNode<T>*& head, doubleNode<T>*& tail, T& data);

template<typename T>
bool removeTail(doubleNode<T>*& head, doubleNode<T>*& tail);

template<typename T>
bool removeTail(doubleNode<T>*& head, doubleNode<T>*& tail, T& data);

template<typename T>
void clear(doubleNode<T>*& head, doubleNode<T>*& tail);

template<typename T>
bool insertHead(doubleList<T>& list, T data);

template<typename T>
bool insertTail(doubleList<T>& list, T data);

template<typename T>
bool removeHead(doubleList<T>& list);

template<typename T>
bool removeHead(doubleList<T>& list, T& data);

template<typename T>
bool removeTail(doubleList<T>& list);

template<typename T>
bool removeTail(doubleList<T>& list, T& data);

template<typename T>
bool isEmpty(const doubleList<T>& list);

template<typename T>
bool removeNode(doubleNode<T>*& head, doubleNode<T>*& tail, doubleNode<T>* target);

template<typename T>
bool removeNode(doubleList<T>& list, doubleNode<T>* target);

template<typename T>
doubleNode<T>* getNode(const doubleList<T>& list, int index);

template<typename T>
bool get(const doubleList<T>& list, int index, T& data);

template<typename T>
bool set(doubleList<T>& list, int index, T data);

template<typename T>
bool insertAt(doubleList<T>& list, int index, T data);

template<typename T>
bool removeAt(doubleList<T>& list, int index);

template<typename T>
void clear(doubleList<T>& list);

#include "double_list.tpp"