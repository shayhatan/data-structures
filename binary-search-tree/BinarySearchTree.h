//
// Created by USER on 20/02/2022.
//

#ifndef DATA_STRUCTURES_BINARYSEARCHTREE_H
#define DATA_STRUCTURES_BINARYSEARCHTREE_H
#include <iostream>
#include <exception>

class EmptyTreeException : std::exception {
public:
    EmptyTreeException() = default;

    const char *what() const noexcept override {
        return "BinarySearchTree is empty()";
    }

};

template<class T>
class BinarySearchTree {
    struct Node;
    Node *head;


public:
    BinarySearchTree() : head(new Node()) {
    }

    ~BinarySearchTree() {
        delete head;
    }


    void insert(const T &data) {
        if (head->data) {
            //not the first node
            insertNode(data, head);
        } else {
            //inserting the first node
            head = new Node(new T(data));
        }
    }


    const T &getMaxValue() const {

        if (!head) {
            throw EmptyTreeException{};
        }
        return getMax(head);
    }

    const T &getMinValue() const {
        if (!head) {
            throw EmptyTreeException{};
        }

        return getMin(head);
    }

    void remove(const T &data);

    void traversal() const {

        if (!head->data) {
            return;
        }
        inOrderTraversal(head);
        std::cout << std::endl;
    }


private:
    void insertNode(const T &data, Node *node);

    void inOrderTraversal(Node *node) const;

    const T &getMax(Node *node) const {

        if (node->right) {
            return getMax(node->right);
        }

        return *node->data;
    }

    const T &getMin(Node *node) const {

        if (node->left) {
            return getMin(node->left);
        }

        return *node->data;
    }

    void remove(const T &data, Node *&node);
};


template<class T>
struct BinarySearchTree<T>::Node {

    T *data;
    Node *left;
    Node *right;

    explicit Node(T *_data = nullptr) : data(_data), left(nullptr), right(nullptr) {};

    Node(const Node &other) = default;

    Node &operator=(const Node &other) = default;

    ~Node() {
        delete left;
        delete right;
        delete data;
    }

};



template<class T>
void BinarySearchTree<T>::insertNode(const T &data, BinarySearchTree::Node *node) {
    //if the data is smaller: we go to the left
    if (data < *node->data) {
        //let's find the location recursively where to insert
        if (node->left) {
            insertNode(data, node->left);
        } else {
            //no left node: this is the location where we have to insert
            node->left = new Node(new T(data));
        }
    } else if (*node->data < data) {
        //else we have to go to the right subtree and find the location
        if (node->right) {
            insertNode(data, node->right);
        } else {
            //no right node: so this is where to insert
            node->right = new Node(new T(data));
        }
    }
}

// the only function with O(N) every single time.
template<class T>
void BinarySearchTree<T>::inOrderTraversal(BinarySearchTree::Node *node) const {
    if (node->left) {
        inOrderTraversal(node->left);
    }
    std::cout << *node->data << " ";

    if (node->right) {
        inOrderTraversal(node->right);
    }

}

template<class T>
void BinarySearchTree<T>::remove(const T &data, BinarySearchTree::Node *&node) {
    //first we have to find the node we want to remove
    if (data < *node->data) {
        if (node->left)
            remove(data, node->left);
    } else if (data > *node->data) {
        if (node->right)
            remove(data, node->right);
    } else {

        // we have found the node we want to remove !!!
        //removing a leaf node
        if (!node->left && !node->right) {
            std::cout << "Removing a leaf node...\n";
            delete node;
            node = nullptr;
            return;
        }
        //removing a right child
        if (!node->left) {
            std::cout << "Removing the right child...\n";
            Node *temp = node->right;
            node->right = nullptr;
            delete node;
            node = temp;
            return;
            //removing a left child
        } else if (!node->right) {
            std::cout << "Removing the left child...\n";
            Node *temp = node->left;
            node->left = nullptr;
            delete node;
            node = temp;
            return;
        }

        // we remove a node with two children case !!!
        std::cout << "Removing item with two children...\n";
        //we find the largest item in the left subtree (or the smallest in the right subtree is valid as well)
        //this is the PREDECESSOR
        Node *temp = node->left;
        Node *predecessor;
        // means the predecessor is left leaf
        if (!temp->right) {
            predecessor = node->left;
            predecessor->right = node->right;
        } else {
            while (temp->right && temp->right->right) {
                temp = temp->right;
            }
            predecessor = temp->right;
            temp->right = nullptr;
            predecessor->right = node->right;
            predecessor->left = node->left;
        }

        node->right = nullptr;
        node->left = nullptr;
        delete node;
        node = predecessor;
    }
}

template<class T>
void BinarySearchTree<T>::remove(const T &data) {
    if (!head) {
        return;
    }
    remove(data, head);
}

#endif //DATA_STRUCTURES_BINARYSEARCHTREE_H
