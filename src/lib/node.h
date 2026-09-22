#pragma once

template<typename T>
struct node {
	T data;
	node<T>* next = nullptr;
};

template<typename T>
struct doubleNode {
    T data;
    doubleNode<T>* prev = nullptr;
    doubleNode<T>* next = nullptr;
};

template<typename T>
bool isEmpty(node<T>* head) {
	return head == nullptr;
}

template<typename T>
bool isEmpty(doubleNode<T>* head) {
    return head == nullptr;
}