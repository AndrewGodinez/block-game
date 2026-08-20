template<typename T>
bool pop(node<T> *&head) {
    if (isEmpty(head)) return false;
    node<T>* aux = head;
    head = aux->next;
    delete aux;
    return true;
}

template<typename T>
bool push(node<T> *&head, T data) {
    node<T>* nNode = new node<T> {data, head};
    head = nNode;
    return true;
}

template<typename T>
bool isEmpty(node<T> *head) {
    if (head == nullptr) {
        return true;
    }
    return false;
}
