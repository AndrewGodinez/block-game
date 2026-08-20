#include <iostream>
#include "in/stack.h"

using namespace std;

int main() {
	node<float>* head = nullptr;

	push(head, 2.03f);
	cout << head->data;
	cout << pop(head);

	return 0;

}
