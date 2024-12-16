#include "wet1util.h"

template<class T>
struct TreeNode {
    T* data;
    TreeNode<T>* left;
    TreeNode<T>* right;
    TreeNode<T>* parent;
    int height;

    TreeNode(T* data, TreeNode<T>* left = nullptr,
             TreeNode<T>* right = nullptr,
             TreeNode<T>* parent = nullptr, int height = 0) :
            data(data),
            left(left),
            right(right),
            parent(parent),
            height(height) {}

    ~TreeNode() {
        delete data;
    }
};



template<class T>
class AVL_Tree {
private:
    TreeNode<T>* root;

    void deleteTree(TreeNode<T>* node);

public:
    ~AVL_Tree();
    AVL_Tree();
    StatusType insert(TreeNode<T>*);
    TreeNode<T>* search(TreeNode<T>*);
    StatusType removal(TreeNode<T>*);
    void LL_rotation(TreeNode<T>*);
    void LR_rotation(TreeNode<T>*);
    void RR_rotation(TreeNode<T>*);
    void RL_rotation(TreeNode<T>*);
    TreeNode<T>* detach(TreeNode<T>* node);
    int get_BF(TreeNode<T>*);
    AVL_Tree(T* data);

    bool isEmpty() { return root == nullptr; }

    void moveToTree(TreeNode<T>*, AVL_Tree<T>&);
    int max(int a, int b);

    TreeNode<T>* getRoot() { return root; }

    void updateHeight(TreeNode<T>*);
};

template<class T>
TreeNode<T>* AVL_Tree<T>::detach(TreeNode<T>* node) {
    if (!root || !node) {
        return nullptr;
    }

    TreeNode<T>* target = search(node);
    if (!target || *(target->data) != *(node->data)) {
        return nullptr;
    }

    if (target == root && !target->left && !target->right) {
        root = nullptr;
        target->parent = nullptr;
        return target;
    }

    TreeNode<T>* replace = nullptr;
    if (target->left && target->right) {
        replace = target->right;
        while (replace->left) {
            replace = replace->left;
        }
        target->data = replace->data;
        target = replace;
    }

    TreeNode<T>* child = (target->left) ? target->left : target->right;
    TreeNode<T>* parent = target->parent;

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

    target->left = target->right = target->parent = nullptr;
    return target;
}

template<class T>
void AVL_Tree<T>::deleteTree(TreeNode<T>* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

template<class T>
AVL_Tree<T>::~AVL_Tree() {
    deleteTree(root);
}

template<class T>
AVL_Tree<T>::AVL_Tree(T* data) {
    root = new TreeNode<T>(data);
    root->height = 1;
}

// Constructor for empty tree
template<class T>
AVL_Tree<T>::AVL_Tree() : root(nullptr) {}

template<class T>
void AVL_Tree<T>::updateHeight(TreeNode<T>* node) {
    if (node == nullptr) { return; }
    int leftHeight = (node->left) ? node->left->height : 0;
    int rightHeight = (node->right) ? node->right->height : 0;
    node->height = 1 + max(leftHeight, rightHeight);
}

template<class T>
StatusType AVL_Tree<T>::insert(TreeNode<T>* node) {
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
    TreeNode<T>* current = search(node);
    if (current != nullptr && *(current->data) == *(node->data)) {
        return StatusType::FAILURE;
    }

    // Insertion process
    current = root;
    TreeNode<T>* parent = nullptr;

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
TreeNode<T>* AVL_Tree<T>::search(TreeNode<T>* node) {
    TreeNode<T>* current = root;
    TreeNode<T>* temp_parent = nullptr;
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
StatusType AVL_Tree<T>::removal(TreeNode<T>* node) {
    if (!root) {
        return StatusType::FAILURE;
    }
    if (*(node->data) <= 0) {
        return StatusType::INVALID_INPUT;
    }
    TreeNode<T>* target = search(node);
    if (target == nullptr || *(target->data) != *(node->data)) {
        return StatusType::FAILURE;
    }
    if (target == root && !target->left && !target->right) {
        root = nullptr;
        target->parent = nullptr;
        delete target;
        return StatusType::SUCCESS;
    }

    TreeNode<T>* replace = nullptr;
    if (target->left && target->right) {
        replace = target->right;
        while (replace->left) {
            replace = replace->left;
        }
        target->data = replace->data;
        target = replace;
    }

    TreeNode<T>* child = (target->left) ? target->left : target->right;
    TreeNode<T>* parent = target->parent;

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

    delete target;
    return StatusType::SUCCESS;
}

// LL rotation function
template<class T>
void AVL_Tree<T>::LL_rotation(TreeNode<T>* node) {
    TreeNode<T>* temp = node->left->right;
    TreeNode<T>* child = node->left;

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
void AVL_Tree<T>::LR_rotation(TreeNode<T>* node) {
    RR_rotation(node->left);
    LL_rotation(node);
}

// RR rotation function
template<class T>
void AVL_Tree<T>::RR_rotation(TreeNode<T>* node) {
    TreeNode<T>* newRoot = node->right;
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
void AVL_Tree<T>::RL_rotation(TreeNode<T>* node) {
    LL_rotation(node->right);
    RR_rotation(node);
}

template<class T>
int AVL_Tree<T>::get_BF(TreeNode<T>* node) {
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