#include "wet1util.h"
#include <memory>

using namespace std;

template<class T>
struct TreeNode {
    shared_ptr<T> data;
    shared_ptr<TreeNode<T>> left;
    shared_ptr<TreeNode<T>> right;
    shared_ptr<TreeNode<T>> parent;
    int height;

    TreeNode(shared_ptr<T> data, shared_ptr<TreeNode<T>> left = nullptr,
             shared_ptr<TreeNode<T>> right = nullptr,
             shared_ptr<TreeNode<T>> parent = nullptr, int height = 0) :
            data(data),
            left(left),
            right(right),
            parent(parent),
            height(height) {}
};

template<class T>
class AVL_Tree {
private:
    shared_ptr<TreeNode<T>> root;
public:
    ~AVL_Tree();
    AVL_Tree();
    StatusType insert(shared_ptr<TreeNode<T>>);
    shared_ptr<TreeNode<T>> search(shared_ptr<TreeNode<T>>);
    StatusType removal(shared_ptr<TreeNode<T>>);
    void LL_rotation(shared_ptr<TreeNode<T>>);
    void LR_rotation(shared_ptr<TreeNode<T>>);
    void RR_rotation(shared_ptr<TreeNode<T>>);
    void RL_rotation(shared_ptr<TreeNode<T>>);
    int get_BF(shared_ptr<TreeNode<T>>);
    AVL_Tree(T data);

    bool isEmpty() { return root == nullptr; }

    void moveToTree(shared_ptr<TreeNode<T>>, AVL_Tree<T> &);
    int max(int a, int b);

    shared_ptr<TreeNode<T>> getRoot() { return root; }

    void updateHeight(shared_ptr<TreeNode<T>>);
};

template<class T>
void AVL_Tree<T>::moveToTree(shared_ptr<TreeNode<T>> nodeToMove,
                             AVL_Tree<T> &treeToMoveTo) {
    shared_ptr<T> data = nodeToMove->data;
    treeToMoveTo.insert(make_shared<TreeNode<T>>(data));
    this->removal(nodeToMove);
}

template<class T>
AVL_Tree<T>::~AVL_Tree() = default;

template<class T>
AVL_Tree<T>::AVL_Tree(T data) {
    root = make_shared<TreeNode<T>>(
            make_shared<T>(data)); // Using make_shared for root node
    root->height = 1;
}

// Constructor for empty tree
template<class T>
AVL_Tree<T>::AVL_Tree() : root(nullptr) {}

template<class T>
void AVL_Tree<T>::updateHeight(shared_ptr<TreeNode<T>> node) {
    if (node == nullptr) { return; }
    int leftHeight = (node->left) ? node->left->height : 0;
    int rightHeight = (node->right) ? node->right->height : 0;
    node->height = 1 + max(leftHeight, rightHeight);
}

template<class T>
StatusType AVL_Tree<T>::insert(shared_ptr<TreeNode<T>> node) {
    if (*(node->data) < 0) {
        return StatusType::INVALID_INPUT;
    }

    // Handle empty tree case
    if (root == nullptr) {
        root = node;
        node->parent = nullptr;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        return StatusType::SUCCESS;
    }

    // Check for duplicates
    shared_ptr<TreeNode<T>> current = search(node);
    if (current != nullptr && *(current->data) == *(node->data)) {
        return StatusType::FAILURE;
    }

    // Insertion process
    current = root;
    shared_ptr<TreeNode<T>> parent = nullptr;
/*    while (current != nullptr) {
        parent = current;
        if (*(node->data) < *(current->data)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }*/

    parent = search(node);
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;

    // Insert the node
    if (*(node->data) < *(parent->data)) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    // Rebalance the tree by traversing up
    current = node->parent;
    while (current != nullptr) {
        updateHeight(current);  // Ensure height is updated before checking BF
        int BF = get_BF(current);  // Calculate balance factor

        // Check if the node is unbalanced and rotate
        if (BF > 1 || BF < -1) {
            if (BF > 1) {
                if (get_BF(current->left) >= 0) {
                    LL_rotation(current);
                    return StatusType::SUCCESS;
                } else {
                    LR_rotation(current);
                    return StatusType::SUCCESS;
                }
            } else {
                if (get_BF(current->right) <= 0) {
                    RR_rotation(current);
                    return StatusType::SUCCESS;
                } else {
                    RL_rotation(current);
                    return StatusType::SUCCESS;
                }
            }
        }

        current = current->parent;
    }

    return StatusType::SUCCESS;
}

// search function
template<class T>
shared_ptr<TreeNode<T>> AVL_Tree<T>::search(shared_ptr<TreeNode<T>> node) {
    shared_ptr<TreeNode<T>> current = root;
    shared_ptr<TreeNode<T>> temp_parent = nullptr;
    while (current != nullptr && *(current->data) != *(node->data)) {
        temp_parent = current;
        if (*(node->data) < *(current->data)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (!current) {
        return temp_parent;
    }
    return current;
}

// Removal function
template<class T>
StatusType AVL_Tree<T>::removal(shared_ptr<TreeNode<T>> node) {
    if (!root) {
        return StatusType::FAILURE;
    }
    if (*(node->data) <= 0) {
        return StatusType::INVALID_INPUT;
    }
    shared_ptr<TreeNode<T>> target = search(node);
    if (target == nullptr || *(target->data) != *(node->data)) {
        return StatusType::FAILURE;
    }
    if (target == root && !target->left && !target->right) {
        root = nullptr;
        target->parent = nullptr;
        return StatusType::SUCCESS;
    }

    shared_ptr<TreeNode<T>> replace = nullptr;
    if (target->left && target->right) {
        replace = target->right;
        while (replace->left) {
            replace = replace->left;
        }
        target->data = replace->data;
        target = replace;
    }

    shared_ptr<TreeNode<T>> child = (target->left) ? target->left
                                                   : target->right;
    shared_ptr<TreeNode<T>> parent = target->parent;

    if (child) {
        child->parent = parent;
    }

    if (!parent) {
        root = child;
    } else if (parent->left == target) {
        parent->left = child;
    } else {
        parent->right = child;
    }

    while (parent) {
        updateHeight(parent);
        int BF = get_BF(parent);
        if (BF > 1 || BF < -1) {
            if (BF > 1) {
                if (get_BF(parent->left) >= 0) {
                    LL_rotation(parent);
                } else {
                    LR_rotation(parent);
                }
            } else {
                if (get_BF(parent->right) <= 0) {
                    RR_rotation(parent);
                } else {
                    RL_rotation(parent);
                }
            }
        }
        parent = parent->parent;
    }

    return StatusType::SUCCESS;
}

// LL rotation function
template<class T>
void AVL_Tree<T>::LL_rotation(shared_ptr<TreeNode<T>> node) {
    shared_ptr<TreeNode<T>> temp = node->left->right;
    shared_ptr<TreeNode<T>> child = node->left;

    // Update links
    node->left = temp;
    if (temp != nullptr) {
        temp->parent = node;
    }
    child->parent = node->parent;
    child->right = node;
    node->parent = child;

    // Update parent-child relationships
    if (child->parent == nullptr) {
        root = child;  // If the parent is null, the child becomes the new root
    } else if (child->parent->left == node) {
        child->parent->left = child;
    } else {
        child->parent->right = child;
    }

    // Update heights
    updateHeight(node);
    updateHeight(child);
}


// LR rotation function
template<class T>
void AVL_Tree<T>::LR_rotation(shared_ptr<TreeNode<T>> node) {
    RR_rotation(node->left);
    LL_rotation(node);
}

// RR rotation function
template<class T>
void AVL_Tree<T>::RR_rotation(shared_ptr<TreeNode<T>> node) {
    shared_ptr<TreeNode<T>> newRoot = node->right;
    node->right = newRoot->left;
    if (newRoot->left != nullptr) {
        newRoot->left->parent = node;
    }
    newRoot->parent = node->parent;

    if (node->parent == nullptr) {  // If the node is the root
        root = newRoot;
    } else if (node == node->parent->left) {
        node->parent->left = newRoot;
    } else {
        node->parent->right = newRoot;
    }

    newRoot->left = node;
    node->parent = newRoot;

    // Update heights
    updateHeight(node);
    updateHeight(newRoot);
}


// RL rotation function
template<class T>
void AVL_Tree<T>::RL_rotation(shared_ptr<TreeNode<T>> node) {
    LL_rotation(node->right);
    RR_rotation(node);
}


template<class T>
int AVL_Tree<T>::get_BF(shared_ptr<TreeNode<T>> node) {
    if (node == nullptr) {
        return 0;
    }
    int leftHeight = (node->left) ? node->left->height : 0;
    int rightHeight = (node->right) ? node->right->height : 0;
    return leftHeight - rightHeight;
}


template<class T>
int AVL_Tree<T>::max(int a, int b) {
    if (a > b) { return a; }
    return b;
}
