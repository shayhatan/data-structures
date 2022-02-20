//
// Created by USER on 20/02/2022.
//

#ifndef DATA_STRUCTURES_LINKLIST_H
#define DATA_STRUCTURES_LINKLIST_H

template<class T>
class LinkedList {
    struct Node;
    Node *head;
    int size;
public:

    LinkedList() : head(new Node(nullptr)), size(0) {
    }

    ~LinkedList() {
        delete head;
    }

    void insertBeginning(const T &data) {
        ++size;
        T *new_data = new T(data);

        // this is the first item in the linked list
        if (!head->data) {
            head->data = new_data;
        } else {
            // inserting the new item to end of the list
            Node *new_node = new Node(new_data);
            new_node->next = head;
            head = new_node;
        }
    }

    void traverse() {
        Node *it = head;
        while (it) {
            std::cout << *it << std::endl;
            it = it->next;
        }
    }

    void remove(const T &data) {
        if (head->data == nullptr) {
            return;
        }

        if (*head->data == data) {
            --size;
            delete head->data;
        } else {
            remove(data, head, head->next);
        }
    }

    int getSize() const {
        return size;
    }

    void insertEnd(const T &data) {
        if (head->data == nullptr) {
            head->data = new T(data);
        } else {
            insertEnd(data, head);
        }
    }

private:
    void remove(const T &data, Node *prev, Node *actual) {
        while (actual != nullptr) {
            if (*actual->data == data) {
                --size;
                prev->next = actual->next;
                actual->next = nullptr;
                delete actual;
                actual = nullptr;
            } else {
                prev = actual;
                actual = actual->next;
            }
        }
    }

    void insertEnd(const T &data, Node *node) {
        // in order to it in O(1) running time complexity check out - ../doubly-linked-list
        if (node->next != nullptr) {
            insertEnd(data, node->next);
        } else {
            node->next = new Node(new T(data));
            ++size;
        }

    }
};

template<class T>
struct LinkedList<T>::Node {
    T *data;
    Node *next;

    explicit Node(T *_data = nullptr) : data(_data), next(nullptr) {

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
