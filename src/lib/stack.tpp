template<typename T>
bool pop(node<T> *&head) {
    if (isEmpty(head)) return false;
    node<T>* aux = head;
    head = aux->next;
    delete aux;
    return true;
}

template<typename T>
bool pop(node<T>*& head, T& data) {
    if(isEmpty(head)) return false;
    data = head->data;
    return pop(head); 
}

template<typename T>
bool push(node<T>*& head, T data) {
    node<T>* nNode = new node<T> {data, head};
    head = nNode;
    return true;
}


template <typename T>
bool peek(node<T>* head, T& data) {
    if (isEmpty(head)) return false;
    data = head->data;
    return true;
}


template<typename T>
void clear(node<T>*& head) {
    while(!isEmpty(head)) {
        pop(head);
    }
}

template<typename T>
bool push(Stack<T>& stack, T data) {
    if (push(stack.head, data)) {
        stack.count++;
        return true;
    }
    return false;
}

template<typename T>
bool pop(Stack<T>& stack, T& data) {
    if (pop(stack.head, data)) {
        stack.count--;
        return true;
    }
    return false;
}

template<typename T>
bool pop(Stack<T>& stack) {
    if (pop(stack.head)) {
        stack.count--;
        return true;
    }
    return false;
}

template<typename T>
bool peek(const Stack<T>& stack, T& data) {
    return peek(stack.head, data);
}

template<typename T>
bool isEmpty(const Stack<T>& stack) {
    return isEmpty(stack.head);
}

template<typename T>
void clear(Stack<T>& stack) {
    while (!isEmpty(stack)) {
        pop(stack);
    }
}