//
// Created by USER on 18/02/2022.
//

#ifndef DATA_STRUCTURES_QUEUE_H
#define DATA_STRUCTURES_QUEUE_H
template<class T>
class Queue {
    struct Node;
    Node *head;
    Node *tail;
    int count;

public:

    explicit Queue() : head(nullptr), tail(nullptr), count(0) {}

    bool isEmpty() {
        return count == 0;
    }

    void enqueue(const T &data) {
        Node *last = tail;
        tail = new Node(new T(data));

        if (isEmpty()) {
            head = tail;
        } else {
            last->setNext(tail);
        }

        ++count;
    }

    T dequeue() {
        // could throw exception  if(count == null)
        count--;

        T to_dequeue = *head->getData();
        delete head->data;
        head = head->next;

        if (isEmpty()) {
            tail = nullptr;
        }
        return to_dequeue;
    }

};

template<class T>
struct Queue<T>::Node {
    T *data;
    Node *next;

    explicit Node(T *data, Node *next = nullptr) : data(data), next(next) {}

    ~Node() {
        delete data;
        delete next;
    }

    T *getData() const {
        return data;
    }

    void setData(T *data) {
        Node::data = data;
    }

    Node *getNext() const {
        return next;
    }

    void setNext(Node *next) {
        Node::next = next;
    }

    friend std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << "data: " << *node.data;
        return os;
    }
};

#endif //DATA_STRUCTURES_QUEUE_H
