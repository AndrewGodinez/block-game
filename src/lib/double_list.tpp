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

template<typename T>
bool removeNode(doubleNode<T>*& head, doubleNode<T>*& tail, doubleNode<T>* target) {
    if (target == nullptr || isEmpty(head)) return false;
    if (target == head) {
        return removeHead(head, tail);
    }
    if (target == tail) {
        return removeTail(head, tail);
    }
    target->prev->next = target->next;
    target->next->prev = target->prev;
    delete target;
    return true;
}

template<typename T>
bool removeNode(doubleList<T>& list, doubleNode<T>* target) {
    if (removeNode(list.head, list.tail, target)) {
        list.count--;
        return true;
    }
    return false;
}

template<typename T>
doubleNode<T>* getNode(const doubleList<T>& list, int index) {
    if (index < 0 || index >= list.count) return nullptr;
    if (index < list.count / 2) {
        doubleNode<T>* curr = list.head;
        for (int i = 0; i < index; ++i) {
            curr = curr->next;
        }
        return curr;
    } else {
        doubleNode<T>* curr = list.tail;
        for (int i = list.count - 1; i > index; --i) {
            curr = curr->prev;
        }
        return curr;
    }
}

template<typename T>
bool get(const doubleList<T>& list, int index, T& data) {
    doubleNode<T>* n = getNode(list, index);
    if (n == nullptr) return false;
    data = n->data;
    return true;
}

template<typename T>
bool set(doubleList<T>& list, int index, T data) {
    doubleNode<T>* n = getNode(list, index);
    if (n == nullptr) return false;
    n->data = data;
    return true;
}

template<typename T>
bool insertAt(doubleList<T>& list, int index, T data) {
    if (index < 0 || index > list.count) return false;
    if (index == 0) return insertHead(list, data);
    if (index == list.count) return insertTail(list, data);
    doubleNode<T>* curr = getNode(list, index);
    doubleNode<T>* nNode = new doubleNode<T>{data, curr->prev, curr};
    curr->prev->next = nNode;
    curr->prev = nNode;
    list.count++;
    return true;
}

template<typename T>
bool removeAt(doubleList<T>& list, int index) {
    doubleNode<T>* n = getNode(list, index);
    if (n == nullptr) return false;
    return removeNode(list, n);
}