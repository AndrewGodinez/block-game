template<typename T>
bool enqueue(node<T>*& front, node<T>*& rear, T data) {
    node<T>* nNode = new node<T>{data, nullptr};
    if (isEmpty(front)) {
        front = nNode;
        rear = nNode;
    } else {
        rear->next = nNode;
        rear = nNode;
    }
    return true;
}

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear) {
    if (isEmpty(front)) return false;
    node<T>* aux = front;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete aux;
    return true;
}

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear, T& data) {
    if (isEmpty(front)) return false;
    data = front->data;
    return dequeue(front,rear);
}

template<typename T>
bool peek(node<T>* front, T& data) {
    if (isEmpty(front)) return false;
    data = front->data;
    return true;
}

template<typename T>
void clear(node<T>*& front, node<T>*& rear) {
    while (!isEmpty(front)) {
        dequeue(front, rear);
    }
}

template<typename T>
bool enqueue(Queue<T>& queue, T data) {
    if (enqueue(queue.front, queue.rear, data)) {
        queue.count++;
        return true;
    }
    return false;
}

template<typename T>
bool dequeue(Queue<T>& queue, T& data) {
    if (dequeue(queue.front, queue.rear, data)) {
        queue.count--;
        return true;
    }
    return false;
}

template<typename T>
bool dequeue(Queue<T>& queue) {
    if (dequeue(queue.front, queue.rear)) {
        queue.count--;
        return true;
    }
    return false;
}

template<typename T>
bool peek(const Queue<T>& queue, T& data) {
    return peek(queue.front, data);
}

template<typename T>
bool isEmpty(const Queue<T>& queue) {
    return isEmpty(queue.front);
}

template<typename T>
void clear(Queue<T>& queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
}