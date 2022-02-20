//
// Created by USER on 20/02/2022.
//

#ifndef DATA_STRUCTURES_REDBLACKTREE_H
#define DATA_STRUCTURES_REDBLACKTREE_H

#include <ostream>

enum Color {
    RED, BLACK
};

template<class T>
class RedBlackTree {

    struct Node;
    Node *root;

    // no option for copy constructor
    RedBlackTree<T> &operator=(const RedBlackTree<T> &avl) = default;

public:
    RedBlackTree() : root(new Node()) {}

    ~RedBlackTree() {
        delete root;
    }

    void insert(const T &data);

    void remove(const T &data);

    void traverse();

private:
    // should be static funcs

    bool isRed(Node *node) {
        if (node == nullptr) {
            return false;
        }
        return node->getColor() == RED;
    }

    void insert(Node *new_node, Node *node);

    void traverse(Node *node);

    void settleViolations(Node *node);

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
struct RedBlackTree<T>::Node {
    T *data;
    Node *left;
    Node *right;
    Node *parent;

    Color color;

    explicit Node(T *_data = nullptr) : data(_data), left(nullptr), right(nullptr), parent(nullptr),
                                        color(RED) {};

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

    Color getColor() const {
        return color;
    }

    void setColor(Color color) {
        Node::color = color;
    }

    friend std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << *node.data << " ";
        return os;
    }

};

template<class T>
void RedBlackTree<T>::insert(const T &data) {
    if (root->data == nullptr) {
        root->setData(new T(data));
        settleViolations(root);
    } else {
        Node *new_node = new Node(new T(data));
        insert(new_node, root);
    }
}

template<class T>
void RedBlackTree<T>::insert(RedBlackTree::Node *new_node, RedBlackTree::Node *node) {
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
void RedBlackTree<T>::traverse() {
    if (root == nullptr) {
        return;
    }
    traverse(root);
}

template<class T>
void RedBlackTree<T>::traverse(RedBlackTree::Node *node) {
    if (node->getLeft()) {
        traverse(node->getLeft());
    }
    std::cout << *node << " ";

    if (node->getRight()) {
        traverse(node->getRight());
    }
}


template<class T>
void RedBlackTree<T>::settleViolations(RedBlackTree::Node *node) {

    // document of properties and cases attached

    Node *parent_node = nullptr;
    Node *grandparent = nullptr;

    // must check violations up to the root node
    while (node != root && isRed(node) && isRed(node->getParent())) {

        parent_node = node->getParent();
        grandparent = node->getParent()->getParent();

        if (parent_node == grandparent->getLeft()) {
            Node *uncle = grandparent->getRight();

            // CASE 1 or 4 (RECOLORING)
            if (isRed(uncle)) {
                grandparent->setColor(RED);
                parent_node->setColor(BLACK);
                uncle->setColor(BLACK);

                // update references we keep going up to the root node
                node = grandparent;
            } else {
                // CASE 2
                if (node == parent_node->getRight()) {
                    leftRotation(parent_node);

                    // update references we keep going up to the root node
                    node = parent_node;
                    parent_node = grandparent;
                }

                // CASE 3: rotation on the grandparent + parent and grandparent switch color
                rightRotation(grandparent);

                // swap the color of the parent and the grandparent
                Color temp_color = parent_node->getColor();
                parent_node->setColor(grandparent->getColor());
                grandparent->setColor(temp_color);

                // update references we keep going up to the root node
                node = parent_node;
            }
        } else {
            // symmetric cases...

            // parent is a right child of it's parent
            Node *uncle = grandparent->getLeft();
            // CASE 1 and 4
            if (isRed(uncle)) {
                grandparent->setColor(RED);
                parent_node->setColor(BLACK);
                node = grandparent;
            } else {
                // CASE 2
                if (node == parent_node->getLeft()) {
                    rightRotation(parent_node);
                    node = parent_node;
                    parent_node = grandparent;
                }
                // CASE 3
                leftRotation(grandparent);


                Color temp_color = parent_node->getColor();
                parent_node->setColor(grandparent->getColor());
                grandparent->setColor(temp_color);
                node = parent_node;
            }
        }
    }
    root->setColor(BLACK);
}




template<class T>
void RedBlackTree<T>::leftRotation(RedBlackTree::Node *node) {
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
    }
}

template<class T>
void RedBlackTree<T>::rightRotation(RedBlackTree::Node *node) {
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
    }
}

template<class T>
void RedBlackTree<T>::remove(const T &data) {
    if (root == nullptr) {
        return;
    }
    const T *p_data = &data;
    remove(p_data, root);
}

template<class T>
void RedBlackTree<T>::remove(const T *data, RedBlackTree::Node *node) {

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
void RedBlackTree<T>::caseRightChild(RedBlackTree::Node *node) {
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
void RedBlackTree<T>::caseLeftChild(RedBlackTree::Node *node) {
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
void RedBlackTree<T>::removeLeaf(RedBlackTree::Node *leaf) {
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
void RedBlackTree<T>::caseTwoChildren(RedBlackTree::Node *node) {
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

#endif //DATA_STRUCTURES_REDBLACKTREE_H
