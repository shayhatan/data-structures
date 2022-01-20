//
// Created by USER on 20/01/2022.
//

#ifndef DATA_STRUCTURES_AVLTREE_H
#define DATA_STRUCTURES_AVLTREE_H
#ifndef AVLTREE_AVLTREE_H
#define AVLTREE_AVLTREE_H

#include "cmath"

template<class T>
class AVLTree {

    struct Node;
    Node *root;

    // no option for copy constructor
    AVLTree<T> &operator=(const AVLTree<T> &avl) = default;

public:
    AVLTree() : root(new Node()) {}

    ~AVLTree() {
        delete root;
    }

    void insert(const T &data);

    void remove(const T &data);

    void traverse();

private:
    // should be static funcs

    void insert(Node *new_node, Node *node);

    void traverse(Node *node);

    int height(Node *node);

    void updateHeight(Node *node);

    void settleViolations(Node *node);

    void violation(Node *node);

    int getBalance(Node *node);

    void leftRotation(Node *node);

    void rightRotation(Node *node);

    void remove(const T *data, Node *node);

    void caseRightChild(Node *node);

    void caseLeftChild(Node *node);

    void removeLeaf(Node *leaf);

    void caseTwoChildren(Node *node);

    Node *getPredecessor(Node *node) {
        while (node->getRight()) {
            node = node->getRight();
        }
        return node;
    }

};


template<class T>
struct AVLTree<T>::Node {
    T *data;
    Node *left;
    Node *right;
    Node *parent;
    int height;

    explicit Node(T *_data = nullptr) : data(_data), left(nullptr), right(nullptr), parent(nullptr), height(0) {};

    Node(const Node &other) = default;

    Node &operator=(const Node &other) = default;

    // parent deleted by previous calls
    ~Node() {
        delete left;
        delete right;
        delete data;
    }


    T getData() const {
        return *data;
    }

    void setData(T *_data) {
        data = _data;
    }

    Node *getLeft() const {
        return left;
    }

    void setLeft(Node *_left) {
        left = _left;
    }

    Node *getRight() const {
        return right;
    }

    void setRight(Node *_right) {
        right = _right;
    }

    Node *getParent() const {
        return parent;
    }

    void setParent(Node *_parent) {
        parent = _parent;
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int _height) {
        height = _height;
    }

    friend std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << *node.data << " ";
        return os;
    }

};

template<class T>
void AVLTree<T>::insert(const T &data) {
    if (root->data == nullptr) {
        root->setData(new T(data));
    } else {
        Node *new_node = new Node(new T(data));
        insert(new_node, root);
    }
}

template<class T>
void AVLTree<T>::insert(AVLTree::Node *new_node, AVLTree::Node *node) {
    if (new_node->getData() < node->getData()) {
        if (node->getLeft() == nullptr) {
            node->setLeft(new_node);
            new_node->setParent(node);
            settleViolations(node);
        } else {
            insert(new_node, node->getLeft());
        }
    } else {
        if (node->getRight() == nullptr) {
            node->setRight(new_node);
            new_node->setParent(node);
            settleViolations(node);
        } else {
            insert(new_node, node->getRight());
        }
    }

}

template<class T>
void AVLTree<T>::traverse() {
    if (root == nullptr) {
        return;
    }
    traverse(root);
}

template<class T>
void AVLTree<T>::traverse(AVLTree::Node *node) {
    if (node->getLeft()) {
        traverse(node->getLeft());
    }
    std::cout << *node << " ";

    if (node->getRight()) {
        traverse(node->getRight());
    }
}

template<class T>
void AVLTree<T>::updateHeight(AVLTree::Node *node) {
    node->setHeight(std::max(height(node->getLeft()), height(node->getRight())) + 1);
}

template<class T>
int AVLTree<T>::height(AVLTree::Node *node) {
    if (node == nullptr) {
        return -1;
    }
    return node->height;
}

template<class T>
int AVLTree<T>::getBalance(AVLTree::Node *node) {
    return height(node->getLeft()) - height(node->getRight());
}


template<class T>
void AVLTree<T>::settleViolations(AVLTree::Node *node) {
    while (node) {
        updateHeight(node);
        violation(node);
        node = node->getParent();
    }
}

template<class T>
void AVLTree<T>::violation(AVLTree::Node *node) {
    int balance = getBalance(node);

    // we must do right rotation
    if (balance > 1) {
        // indicates that we should do left rotation as well
        if (node->getLeft() != nullptr && getBalance(node->getLeft()) < 0) {
            leftRotation(node->getLeft());
        }
        rightRotation(node);
    }
    // we must do left rotation*/
    if (balance < -1) {
        if (node->getRight() != nullptr && getBalance(node->getRight()) > 0) {
            rightRotation(node->getRight());
        }
        leftRotation(node);
    }
}


template<class T>
void AVLTree<T>::leftRotation(AVLTree::Node *node) {
    Node *right = node->getRight();
    Node *parent = node->getParent();
    Node *grand_child = right->getLeft();

    // rotate
    right->setLeft(node);
    node->setRight(grand_child);
    if (grand_child) {
        grand_child->setParent(node);
    }

    // handle parents
    node->setParent(right);
    right->setParent(parent);
    if (parent == nullptr) {
        if (root == node) {
            root = right;
        }
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(right);
        } else {
            parent->setRight(right);
        }
        updateHeight(parent);
    }
    updateHeight(node);
    updateHeight(right);
}

template<class T>
void AVLTree<T>::rightRotation(AVLTree::Node *node) {
    Node *left = node->getLeft();
    Node *parent = node->getParent();
    Node *grand_child = left->getRight();

    // rotate
    left->setRight(node);
    node->setLeft(grand_child);
    if (grand_child) {
        grand_child->setParent(node);
    }

    // handle parents
    node->setParent(left);
    left->setParent(parent);
    if (parent == nullptr) {
        if (root == node) {
            root = left;
        }
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(left);
        } else {
            parent->setRight(left);
        }
        updateHeight(parent);
    }
    updateHeight(node);
}

template<class T>
void AVLTree<T>::remove(const T &data) {
    if (root == nullptr) {
        return;
    }
    const T *p_data = &data;
    remove(p_data, root);
}

template<class T>
void AVLTree<T>::remove(const T *data, AVLTree::Node *node) {

    if (node != nullptr) {
        // first we must find the target node
        if (*data < node->getData()) {
            remove(data, node->getLeft());
        } else if (node->getData() < *data) {
            remove(data, node->getRight());
        } else {
            // CASE 1: no children
            if (!node->getLeft() && !node->getRight()) {
                removeLeaf(node);
            }
                // CASE 2: only left child
            else if (!node->getRight() && node->getLeft()) {
                caseLeftChild(node);
            }
                // CASE 3: only right child
            else if (!node->getLeft() && node->getRight()) {
                caseRightChild(node);
            }
                // CASE 4: two children
            else {
                caseTwoChildren(node);
            }
        }
    }
}

template<class T>
void AVLTree<T>::caseRightChild(AVLTree::Node *node) {
    Node *parent = node->getParent();
    Node *right = node->getRight();
    right->setParent(parent);
    if (root == node) {
        root = right;
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(right);
        } else {
            parent->setRight(right);
        }
    }
    node->right = nullptr;
    delete node;
    settleViolations(right);
}

template<class T>
void AVLTree<T>::caseLeftChild(AVLTree::Node *node) {
    Node *parent = node->getParent();
    Node *left = node->getLeft();
    left->setParent(parent);
    if (root == node) {
        root = left;
    } else {
        if (parent->getLeft() == node) {
            parent->setLeft(left);
        } else {
            parent->setRight(left);
        }
    }
    node->left = nullptr;
    delete node;
    settleViolations(left);
}

template<class T>
void AVLTree<T>::removeLeaf(AVLTree::Node *leaf) {
    Node *parent = leaf->getParent();
    if (root == leaf) {
        delete root;
        root = new Node();
    } else {
        if (parent->getLeft() == leaf) {
            parent->setLeft(nullptr);
        } else {
            parent->setRight(nullptr);
        }
        delete leaf;
        settleViolations(parent);
    }
}

template<class T>
void AVLTree<T>::caseTwoChildren(AVLTree::Node *node) {
    Node *left = node->getLeft();
    Node *right = node->getRight();
    Node *parent = node->getParent();
    Node *predecessor = getPredecessor(left);
    Node *predecessor_parent = predecessor->getParent();

    predecessor->setParent(parent);
    predecessor->setRight(right);

    if (parent != nullptr) {
        if (parent->getRight() == node) {
            parent->setRight(predecessor);
        } else {
            parent->setLeft(predecessor);
        }
    }

    node->setLeft(nullptr);
    node->setRight(nullptr);

    if (left == predecessor) {
        if (root == node) {
            root = predecessor;
        }
        predecessor->setLeft(nullptr);
        delete node;
        settleViolations(predecessor);
    } else {
        predecessor->setLeft(left);
        delete node;
        predecessor_parent->setRight(nullptr);
        settleViolations(predecessor_parent);
    }
}

#endif //AVLTREE_AVLTREE_H

#endif //DATA_STRUCTURES_AVLTREE_H
