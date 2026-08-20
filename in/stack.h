#pragma once
#include <iostream>

template<typename T>
struct node {
	T data;
	node<T>* next = nullptr;
};

template<typename T>
bool pop(node<T> *&head, T data);

template<typename T>
bool peek(node<T> *head, T data);

template<typename T>
bool push(node<T> *&head, T data);

template<typename T>
bool isEmpty(node<T> *head);

#include "stack.tpp"
