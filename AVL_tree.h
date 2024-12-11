#include "wet1util.h"

template<class T>

struct TreeNode {
    T *data;
    TreeNode<T> *left;
    TreeNode<T> *right;
    TreeNode<T> *parent;
    int height;
};

template<class T>
class AVL_Tree {
private:
    TreeNode<T> *root;
public:
    AVL_Tree();
    StatusType insert(TreeNode<T> *);
    TreeNode<T> *search(TreeNode<T> *);
    StatusType removal(TreeNode<T> *);
    void inorder(TreeNode<T> *, T *);
    void LL_rotation(TreeNode<T> *);
    void LR_rotation(TreeNode<T> *);
    void RR_rotation(TreeNode<T> *);
    void Rl_rotation(TreeNode<T> *);
    int get_BF(TreeNode<T> *);
    int max(int a, int b);
    void updateHeight(TreeNode<T> *);
};

template<class T>
void AVL_Tree<T>::updateHeight(TreeNode<T> *node) {
    if (node == nullptr) { return; }
    int leftHeight = (node->left) ? node->left->height : 0;
    int rightHeight = (node->right) ? node->right->height : 0;
    node->height = 1 + max(leftHeight, rightHeight);
}

// Constructor for AVL_Tree
template<class T>
AVL_Tree<T>::AVL_Tree() : root(nullptr) {}

// Insert function
template<class T>
StatusType AVL_Tree<T>::insert(TreeNode<T> *node) {
    if (node->data < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (root == nullptr) {
        root = node;
        node->parent = nullptr;
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        return StatusType::SUCCESS;
    }
    TreeNode<T> *current = search(node);
    if (current != nullptr && current->data == node->data) {
        return StatusType::FAILURE;
    }
    current = root;
    TreeNode<T> *parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (node->data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    current = node->parent;
    while (current != nullptr) {
        updateHeight(current);
        int BF = get_BF(current);
        if (BF > 1 || BF < -1) {
            if (BF > 1) {
                if (get_BF(current->left) >= 0) {
                    LL_rotation(current);
                } else {
                    LR_rotation(current);
                }
            } else {
                if (get_BF(current->right) <= 0) {
                    RR_rotation(current);
                } else {
                    Rl_rotation(current);
                }
            }
        }
        current = current->parent;
    }
    return StatusType::SUCCESS;
}

// Search function
template<class T>
TreeNode<T> *AVL_Tree<T>::search(TreeNode<T> *node) {
    TreeNode<T> *current = root;
    while (current != nullptr && current->data != node->data) {
        if (node->data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return current;
}

// Removal function
template<class T>
StatusType AVL_Tree<T>::removal(TreeNode<T> *node) {
    if (node->data <= 0) {
        return StatusType::INVALID_INPUT;
    }
    TreeNode<T> *target = search(node);
    if (target == nullptr || target->data != node->data) {
        return StatusType::FAILURE;
    }

    TreeNode<T> *replace = nullptr;
    if (target->left && target->right) {
        replace = target->right;
        while (replace->left) {
            replace = replace->left;
        }
        target->data = replace->data;
        target = replace;
    }

    TreeNode<T> *child = (target->left) ? target->left : target->right;
    TreeNode<T> *parent = target->parent;

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

    delete target;

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
                    Rl_rotation(parent);
                }
            }
        }
        parent = parent->parent;
    }

    return StatusType::SUCCESS;
}

// Inorder traversal function
template<class T>
void AVL_Tree<T>::inorder(TreeNode<T> *node, T *arr) {
    //TODO
}

// LL rotation function
template<class T>
void AVL_Tree<T>::LL_rotation(TreeNode<T> *node) {
    TreeNode<T> *temp = node->left->right;
    TreeNode<T> *child = node->left;
    node->left = temp;
    child->parent = node->parent;
    child->right = node;
    node->parent = child;
    updateHeight(node);
    updateHeight(child);
}

// LR rotation function
template<class T>
void AVL_Tree<T>::LR_rotation(TreeNode<T> *node) {
    RR_rotation(node->left);
    LL_rotation(node);
}

// RR rotation function
template<class T>
void AVL_Tree<T>::RR_rotation(TreeNode<T> *node) {
    TreeNode<T> *newRoot = node->right;
    node->right = newRoot->left;
    if (newRoot->left != nullptr) {
        newRoot->left->parent = node;
    }
    newRoot->parent = node->parent;
    if (node->parent == nullptr) {
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
void AVL_Tree<T>::Rl_rotation(TreeNode<T> *node) {
    LL_rotation(node->right);
    RR_rotation(node);
}


template<class T>
int AVL_Tree<T>::get_BF(TreeNode<T> *node) {
    if (!node->left && !node->right) {
        return 0;
    }
    if (!node->left) {
        return -node->right->height;
    }
    if (!node->right) {
        return node->left->height;
    }
    return node->left->height - node->right->height;
}

template<class T>
int AVL_Tree<T>::max(int a, int b) {
    if (a > b) { return a; }
    return b;
}
