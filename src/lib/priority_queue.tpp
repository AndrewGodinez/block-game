template<typename T>
bool enqueue(node<T>*& front, node<T>*& rear, T data) {
    node<T>* nNode = new node<T>{data, nullptr};

    if (isEmpty(front)) {
        front = nNode;
        rear = nNode;
        return true;
    }

    if (data < front->data) {
        nNode->next = front;
        front = nNode;
        return true;
    }

    if (!(data < rear->data)) {
        rear->next = nNode;
        rear = nNode;
        return true;
    }

    node<T>* prev = front;
    node<T>* curr = front->next;

    while (curr != nullptr && !(data < curr->data)) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = nNode;
    nNode->next = curr;
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
    return dequeue(front, rear);
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
bool enqueue(PriorityQueue<T>& queue, T data) {
    if (enqueue(queue.front, queue.rear, data)) {
        queue.count++;
        return true;
    }
    return false;
}

template<typename T>
bool dequeue(PriorityQueue<T>& queue, T& data) {
    if (dequeue(queue.front, queue.rear, data)) {
        queue.count--;
        return true;
    }
    return false;
}

template<typename T>
bool dequeue(PriorityQueue<T>& queue) {
    if (dequeue(queue.front, queue.rear)) {
        queue.count--;
        return true;
    }
    return false;
}

template<typename T>
bool peek(const PriorityQueue<T>& queue, T& data) {
    return peek(queue.front, data);
}

template<typename T>
bool isEmpty(const PriorityQueue<T>& queue) {
    return isEmpty(queue.front);
}

template<typename T>
void clear(PriorityQueue<T>& queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
}
