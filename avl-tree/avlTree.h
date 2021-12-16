//
// Created by USER on 16/12/2021.
//

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

#include <iostream>
#include <exception>


/*
 * BALANCE FACTOR {-1, 0 , 1};
 *              which means that if the and int number is in one of those then the tree is balanced
 *              if not balance we have 4 diff types of rotations {left-left, left-right, right-right, right-left}
 *              will know how to rotate by sign -
 *                  "-" - means left
 *                  "+" - means right
 */





class EmptyTreeException : std::exception {
public:
    EmptyTreeException() = default;

    const char *what() const noexcept override {
        return "AVL is empty()";
    }
};

template<class T>
class AVL {
    struct Node;
    Node *head;

public:
    AVL();

    AVL(const AVL &other);

    AVL &operator=(const AVL &other);

    void insert(const T &data);

    void traversal() const;

    void printLevelOrder();

    // if(!head) return null else while(node->right) return node;
    const T &getMaxValue() const;

    const T &getMinValue() const;

    void remove(const T &data);

    bool contain(const T &data);

private:
    static void insertCurrent(Node *here, Node *that);

    static int getBalanceFactor(Node *node);

    static bool leftLeftUnbalance(Node *new_node, Node *r, int balance_factor);

    static bool rightRightUnbalance(Node *new_node, Node *r, int balance_factor);

    static bool rightLeftUnbalance(Node *new_node, Node *r, int balance_factor);

    static bool leftRightUnbalance(Node *new_node, Node *r, int balance_factor);

    static bool contain(const T *data, Node *r);

    static void inOrderTraversal(Node *node);

    static void printGivenLevel(Node *r, int level);

    static int height(Node *node);

    static Node *rotate(Node *new_node, Node *r) {
        // now the recursion is going back from bottom, "first iteration, parent is curr_root of new_node"
        // will go to the bottom of the insertion because we are looking for nullptr
        int balance_factor = getBalanceFactor(r);

        if (leftLeftUnbalance(new_node, r, balance_factor)) {
            return rightRotate(r);
        }
        if (rightRightUnbalance(new_node, r, balance_factor)) {
            return leftRotate(r);
        }
        if (rightLeftUnbalance(new_node, r, balance_factor)) {
            r->left = rightRotate(r);
            return leftRotate(r);
        }
        if (leftRightUnbalance(new_node, r, balance_factor)) {
            r->right = leftRotate(r);
            return rightRotate(r);
        }
        return r;
    }

    static Node *rightRotate(Node *z) {
        // we want to rotate y with z, x = inserted, z= root  - (z left is y left is x)
        Node *y = z->left;
        Node *sub_tree = y->right;
        // rotate right - y with z
        y->right = z;
        z->left = sub_tree;
        return y;
    }

    static Node *leftRotate(Node *z) {
        // we want to rotate y with z, x = inserted, z= root  - (z right is right left is x)
        Node *y = z->right;
        Node *sub_tree = y->left;
        // rotate right - y with z
        y->left = z;
        z->right = sub_tree;
        return y;
    }

    static Node *removeRotate(Node *r) {
        int balance_factor = getBalanceFactor(r);
        // six option included when deleting from the bottom while 4 of them
        if (r && balance_factor == 2 && getBalanceFactor(r->left) >= 0) {
            return rightRotate(r);
        } else if (r && balance_factor == 2 && getBalanceFactor(r->left) == -1) {
            r->left = leftRotate(r);
            return rightRotate(r);
        }

        else if (r && balance_factor == -2 && getBalanceFactor(r->right) <= 0) {
            return leftRotate(r);
        } else if (r && balance_factor == 02 && getBalanceFactor(r->right) == 1) {
            r->right = rightRotate(r);
            return leftRotate(r);
        }
        return r;
    }

    Node *insertNode(Node *new_node, Node *curr_root) {
        //if the data is smaller: we go to the left
        if (*new_node->data < *curr_root->data) {
            //let's find the location recursively where to insert
            if (curr_root->left) {
                curr_root->left = insertNode(new_node, curr_root->left);
            } else {
                //no left node: this is the location where we have to insert
                curr_root->left = new_node;
            }
        } else if (*curr_root->data < *new_node->data) {
            //else we have to go to the right subtree and find the location
            if (curr_root->right) {
                curr_root->right = insertNode(new_node, curr_root->right);
            } else {
                //no right node: so this is where to insert
                curr_root->right = new_node;
            }
        } else {
            //no duplicates
            free(new_node);
            return curr_root;
        }

        // "We have 4 options we know each case by balance factor (more info in the head of the file)
        return rotate(new_node, curr_root);

    }

    static Node *removeRightChild(Node *to_remove) {
        Node *temp = to_remove->right;
        to_remove->right = nullptr;
        delete to_remove;
        return temp;
    }

    static Node *removeLeftChild(Node *to_remove) {
        std::cout << "Removing the left child...\n";
        Node *temp = to_remove->left;
        to_remove->left = nullptr;
        delete to_remove;
        return temp;
    }

    static Node *removeBigParent(Node *node) {
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
        return predecessor;
    }

    Node *removeNode(const T *data, Node *node) {
        //first we have to find the node we want to remove
        if (*data < *node->data) {
            if (node->left) {
                node->left = removeNode(data, node->left);
            }
        } else if (*data > *node->data) {
            if (node->right)
                node->right = removeNode(data, node->right);
        } else {
            // we have found the node we want to remove !!!
            //removing a leaf node
            if (!node->left && !node->right) {
                delete node;
                return nullptr;
            } else if (!node->left) {
                removeRightChild(node);
                return node;
            } else if (!node->right) {
                removeLeftChild(node);
            } else {
                node = removeBigParent(node);
            }
        }
        //node is deleted
        return removeRotate(node);
    }
};


/*
        public methods
                                */


template<class T>
AVL<T>::AVL() : head(new Node()) {
}

template<class T>
AVL<T>::AVL(const AVL &other) : head(other.head->data == nullptr ?
                                     new Node() : new Node(
                new T(*(other.head->data)))) {
    insertCurrent(head, other.head);
}


template<class T>
AVL<T> &AVL<T>::operator=(const AVL &other) {
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
void AVL<T>::traversal() const {
    if (!head->data) {
        return;
    }
    Node* temp = head;
    inOrderTraversal(temp);
    std::cout << std::endl;
}

template<class T>
const T &AVL<T>::getMaxValue() const {
    if (head->data == nullptr) {
        throw EmptyTreeException{};
    }
    Node *temp = head;
    while (temp->right) {
        temp = temp->right;
    }
    return *temp->data;
}

template<class T>
const T &AVL<T>::getMinValue() const {
    if (head->data == nullptr) {
        throw EmptyTreeException{};
    }
    Node *temp = head;
    while (temp->left) {
        temp = temp->left;
    }
    return *temp->data;
}

template<class T>
void AVL<T>::remove(const T &data) {
    if (!head) {
        return;
    }
    const T *p_data = &data;
    head = removeNode(p_data, head);
}

template<class T>
void AVL<T>::printLevelOrder() {
    int h = height(head);
    for (int i = 0; i <= h; ++i) {
        printGivenLevel(head, i);
        std::cout << std::endl;
    }
}

template<class T>
bool AVL<T>::contain(const T &data) {
    if (!head) {
        return false;
    }
    const T *p_data = &data;
    Node *r = head;
    return contain(p_data, r);
}

template<class T>
void AVL<T>::insert(const T &data) {
    Node *new_node = new Node(new T(data));
    if (head->data) {
        //not the first node
        head = insertNode(new_node, head);
    } else {
        //inserting the first node
        head = new_node;
    }
}



/*
        private methods and structs
                                            */

template<class T>
struct AVL<T>::Node {

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
void AVL<T>::insertCurrent(AVL::Node *here, AVL::Node *that) {
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
int AVL<T>::height(AVL::Node *node) {
    if (node == nullptr) {
        return -1;
    } else {
        int left_height = height(node->left);
        int right_height = height(node->right);
        if (left_height < right_height) {
            return right_height + 1;
        } else {
            return left_height + 1;
        }
    }
}

template<class T>
int AVL<T>::getBalanceFactor(AVL::Node *node) {
    return height(node->left) - height(node->right);
}

template<class T>
bool AVL<T>::leftLeftUnbalance(AVL::Node *new_node, AVL::Node *r, int balance_factor) {
    return (balance_factor > 1 && r->left
            && *new_node->data < *(r->left->data));
}

template<class T>
bool AVL<T>::rightRightUnbalance(AVL::Node *new_node, AVL::Node *r, int balance_factor) {
    return (balance_factor < -1 && r->right
            && *(r->right->data) < *new_node->data);
}

template<class T>
bool AVL<T>::leftRightUnbalance(AVL::Node *new_node, AVL::Node *r, int balance_factor) {
    return (balance_factor > 1 && r->left
            && *(r->left->data) < *new_node->data);
}

template<class T>
bool AVL<T>::rightLeftUnbalance(AVL::Node *new_node, AVL::Node *r, int balance_factor) {
    return (balance_factor < -1 && r->right
            && *new_node->data < *(r->left->data));
}


template<class T>
void AVL<T>::inOrderTraversal(AVL::Node *node) {
    if (node->left) {
        inOrderTraversal(node->left);
    }
    std::cout << *node->data << " ";

    if (node->right) {
        inOrderTraversal(node->right);
    }
}

template<class T>
void AVL<T>::printGivenLevel(AVL::Node *r, int level) {
    if (r == NULL) {
        return;
    } else if (level == 0) {
        std::cout << *r->data << " ";
    } else {
        printGivenLevel(r->left, level - 1);
        printGivenLevel(r->right, level - 1);
    }
}


#endif //AVL_TREE_AVLTREE_H
