template<typename T, typename U>
bool enqueue(node<T>*& front, node<T>*& rear, U data) {
    node<T>* nNode = new node<T> {static_cast<T>(data), nullptr};
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
bool dequeue(node<T>*& front, node<T>*& rear, T& data) {
    if (isEmpty(front)) return false;
    node<T>* aux = front;
    data = aux->data;
    front = aux->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete aux;
    return true;
}

template<typename T>
bool dequeue(node<T>*& front, node<T>*& rear) {
    if (isEmpty(front)) return false;
    node<T>* aux = front;
    front = aux->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete aux;
    return true;
}

template<typename T>
bool peek(node<T>* front, T& data) {
    if (isEmpty(front)) return false;
    data = front->data;
    return true;
}

template<typename T, typename U>
bool enqueue(Queue<T>& q, U data) {
    return enqueue(q.front, q.rear, data);
}

template<typename T>
bool dequeue(Queue<T>& q, T& data) {
    return dequeue(q.front, q.rear, data);
}

template<typename T>
bool dequeue(Queue<T>& q) {
    return dequeue(q.front, q.rear);
}

template<typename T>
bool peek(const Queue<T>& q, T& data) {
    return peek(q.front, data);
}

template<typename T>
bool isEmpty(const Queue<T>& q) {
    return isEmpty(q.front);
}