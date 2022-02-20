//
// Created by USER on 18/02/2022.
//

#ifndef DATA_STRUCTURES_LINKLIST_H
#define DATA_STRUCTURES_LINKLIST_H

template<class T>
class LinkedList {
    struct Node;
    Node *head;
    Node *tail;
public:

    LinkedList() : head(new Node(nullptr)), tail(nullptr) {
    }

    ~LinkedList() {
        delete head;
    }

    void insert(const T &data) {
        T *new_data = new T(data);

        // this is the first item in the linked list
        if (!tail) {
            // both of the pointer are pointing to the same node
            head->data = new_data;
            tail = head;
        } else {
            // inserting the new item to end of the list
            Node *new_node = new Node(new_data);
            new_node->setPrev(tail);
            tail->setNext(new_node);
            tail = new_node;
        }
    }

    void traverse() {
        Node *it = head;
        while (it) {
            std::cout << *it << std::endl;
            it = it->next;
        }
    }


};

template<class T>
struct LinkedList<T>::Node {
    T *data;
    Node *prev;
    Node *next;

    explicit Node(T *_data = nullptr) : data(_data), prev(nullptr), next(nullptr) {

    }

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

    Node *getPrev() const {
        return prev;
    }

    void setPrev(Node *prev) {
        Node::prev = prev;
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

#endif //DATA_STRUCTURES_LINKLIST_H
