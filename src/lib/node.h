#pragma once

template<typename T>
struct node {
	T data;
	node<T>* next = nullptr;
};

template<typename T>
bool isEmpty(node<T>* head) {
	return head == nullptr;
}
