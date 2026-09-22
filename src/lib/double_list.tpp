template<typename T>
bool insertTail(doubleNode<T>*& head, doubleNode<T>*& tail, T data) {
    doubleNode<T>* nNode = new doubleNode<T>{data, tail, nullptr};
    if (isEmpty(head)) {
        head = nNode;
        tail = nNode;
    } else {
        tail->next = nNode;
        tail = nNode;
    }
    return true;
}

template<typename T>
bool insertHead(doubleNode<T>*& head, doubleNode<T>*& tail, T data) {
    doubleNode<T>* nNode = new doubleNode<T>{data, nullptr, head};
    if (isEmpty(head)) {
        head = nNode;
        tail = nNode;
    } else {
        head->prev = nNode;
        head = nNode;
    }
    return true;
}

template<typename T>
bool removeTail(doubleNode<T>*& head, doubleNode<T>*& tail) {
    if (isEmpty(head)) return false;
    doubleNode<T>* aux = tail;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        tail = tail->prev;
        tail->next = nullptr;
    }
    delete aux;
    return true;
}

template<typename T>
bool removeHead(doubleNode<T>*& head, doubleNode<T>*& tail) {
    if (isEmpty(head)) return false;
    doubleNode<T>* aux = head;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
        head->prev = nullptr;
    }
    delete aux;
    return true;
}

template<typename T>
bool removeTail(doubleNode<T>*& head, doubleNode<T>*& tail, T& data) {
    if (isEmpty(head)) return false;
    data = tail->data;
    return removeTail(head,tail);
}

template<typename T>
bool removeHead(doubleNode<T>*& head, doubleNode<T>*& tail, T& data) {
    if (isEmpty(head)) return false;
    data = head->data;              
    return removeHead(head, tail);  
}

template<typename T>
void clear(doubleNode<T>*& head, doubleNode<T>*& tail) {
    while (!isEmpty(head)) {
        removeHead(head, tail);
    }
}

template<typename T>
bool insertHead(doubleList<T>& list, T data) {
    if (insertHead(list.head, list.tail, data)) {
        list.count++;
        return true;
    }
    return false;
}
template<typename T>
bool insertTail(doubleList<T>& list, T data) {
    if (insertTail(list.head, list.tail, data)) {
        list.count++;
        return true;
    }
    return false;
}
template<typename T>
bool removeHead(doubleList<T>& list, T& data) {
    if (removeHead(list.head, list.tail, data)) {
        list.count--;
        return true;
    }
    return false;
}
template<typename T>
bool removeHead(doubleList<T>& list) {
    if (removeHead(list.head, list.tail)) {
        list.count--;
        return true;
    }
    return false;
}
template<typename T>
bool removeTail(doubleList<T>& list, T& data) {
    if (removeTail(list.head, list.tail, data)) {
        list.count--;
        return true;
    }
    return false;
}
template<typename T>
bool removeTail(doubleList<T>& list) {
    if (removeTail(list.head, list.tail)) {
        list.count--;
        return true;
    }
    return false;
}
template<typename T>
bool isEmpty(const doubleList<T>& list) {
    return isEmpty(list.head);
}
template<typename T>
void clear(doubleList<T>& list) {
    clear(list.head, list.tail);
    list.count = 0;
}