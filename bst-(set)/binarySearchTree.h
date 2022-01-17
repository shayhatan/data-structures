#ifndef SORTED_BST_BINARYSEARCHTREE_H
#define SORTED_BST_BINARYSEARCHTREE_H

#include <iostream>
#include <vector>
#include <exception>
#include <cassert>


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

    class const_iterator;


    const_iterator begin() const {
        return head;
    }

    static const_iterator end() {
        return nullptr;
    }

    BinarySearchTree(const BinarySearchTree &other);

    BinarySearchTree &operator=(const BinarySearchTree &other);

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

    void remove(const T &data, Node *&node);

    void traversal() const {

        if (!head->data) {
            return;
        }
        inOrderTraversal(head);
        std::cout << std::endl;
    }


private:
    static void insertCurrent(Node *here, Node *that);

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
class BinarySearchTree<T>::const_iterator {
    friend class BinarySearchTree<T>;

    Node *p;
    Node *iterator;
    bool isHead = false;
    // std::vector<Node *> in_order; much nicer to use vector.

private:
    const_iterator(Node *pt = nullptr) : p(pt) {}


// more elegant but O(n)
/*    void searchNext(Node *iter, Node *curr) {
        if (iter->left) {
            searchNext(iter->left, curr);
        }
        if (*curr->data < *iter->data) {
            if (p->data == curr->data) {
                p = iter;
                return;
            }
            return;
        }
        if (iter->right) {
            searchNext(iter->right, curr);
        }
    }*/

    // (log(N))
    void searchNext(Node *iter, Node *curr) {
        if (*curr->data < *iter->data) {
            if (iter->left) {
                searchNext(iter->left, curr);
            }
            if (p->data == curr->data && *iter->data > *curr->data) {
                p = iter;
            }
        } else if (*iter->data < *curr->data) {
            if (iter->right) {
                searchNext(iter->right, curr);
            }
        }
        // equal!!!
        if (iter->right) {
            searchNext(iter->right, curr);
            if (*curr->data < *iter->data) {
                if (p->data == curr->data) {
                    p = iter;
                }
            }
        }
    }


    static T *getMaxValue(Node *iter) {
        while (iter->right) {
            iter = iter->right;
        }
        return iter->data;
    }

public:
    const_iterator(const const_iterator &iter) = default;

    const_iterator &operator=(const const_iterator &iter) = default;

    int operator!=(const const_iterator itr) const {
        return (p != itr.p && p->data != nullptr);
    }

    int operator==(const const_iterator itr) const {
        return (p == itr.p);
    }

    const T &operator*() {
        if (p == nullptr || p->data == nullptr) {
            throw std::out_of_range("");
        }
        // want to set iterator as a root
        if (!isHead) {
            iterator = p;
            // set begin to min value
            while (p->left) {
                p = p->left;
            }
            isHead = true;
        }
        return *(p->data);
    }


    const_iterator operator++() {
        if (p == nullptr) {
            throw std::out_of_range("");
        }
        if (p->data == getMaxValue(iterator)) {
            p = nullptr;
            return p;
        }
        searchNext(iterator, p);
        return p;
    }

};


template<class T>
void BinarySearchTree<T>::insertCurrent(BinarySearchTree::Node *here, BinarySearchTree::Node *that) {
    if (that->left && that->left->data) {
        here->left = new Node(new T(*(that->left->data)));
        insertCurrent(here->left, that->left);
    }
    if (that->right && that->right->data) {
        here->right = new Node(new T(*(that->right->data)));
        insertCurrent(here->right, that->right);
    }
}

template<class T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree &other):head(other.head->data == nullptr ?
                                                                          new Node() : new Node(
                new T(*(other.head->data)))) {
    insertCurrent(head, other.head);
}

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

//the only function with O(N) every single time.
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
BinarySearchTree<T> &BinarySearchTree<T>::operator=(const BinarySearchTree &other) {
    if (&other == this) {
        return *this;
    }
    Node *temp;
    // must allocate first to avoid leak encase of failure allocation
    other.head->data == nullptr ? temp = new Node() : temp = new Node(new T(*other.head->data));
    insertCurrent(temp, other.head);
    // now it safe to deallocate
    delete head;
    head = temp;
    return *this;
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

#endif //SORTED_BST_BINARYSEARCHTREE_H