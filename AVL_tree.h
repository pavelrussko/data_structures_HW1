#include "wet1util.h"
#include <memory>

using namespace std;

template<class T>
struct TreeNode {
    shared_ptr<T> data;
    shared_ptr<TreeNode<T>> left;
    shared_ptr<TreeNode<T>> right;
    weak_ptr<TreeNode<T>> parent; // Changed from shared_ptr to weak_ptr
    int height;

    // Corrected Constructor
    TreeNode(shared_ptr<T> data,
             shared_ptr<TreeNode<T>> left = nullptr,
             shared_ptr<TreeNode<T>> right = nullptr,
             weak_ptr<TreeNode<T>> parent = weak_ptr<TreeNode<T>>(),
             int height = 0) :
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
//maybe leak
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
//maybe leak
StatusType AVL_Tree<T>::insert(shared_ptr<TreeNode<T>> node) {
    if (*(node->data) < 0) {
        return StatusType::INVALID_INPUT;
    }

    // Handle empty tree case
    if (root == nullptr) {
        root = node;
        node->parent.reset(); // There was no reset
        node->left = nullptr;
        node->right = nullptr;
        node->height = 1;
        return StatusType::SUCCESS;
    }

    shared_ptr<TreeNode<T>> parent = search(node);
    if (parent == nullptr || *(parent->data) == *(node->data)) {
        return StatusType::FAILURE;
    }

    node->parent = parent; // This is a weak pointer assignment
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
    shared_ptr<TreeNode<T>> current = parent;
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

        current = current->parent.lock();
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

    return current ? current : temp_parent;
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
        root.reset();
        return StatusType::SUCCESS;
    }

    shared_ptr<TreeNode<T>> replace = nullptr;
    // finding replacement node
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
    shared_ptr<TreeNode<T>> parent = target->parent.lock();

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

    // Balance the tree
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
        parent = parent->parent.lock();
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
    if (!child->parent.lock()) {
        root = child;  // If the parent is null, the child becomes the new root
    } else if (child->parent.lock()->left == node) {
        child->parent.lock()->left = child;
    } else {
        child->parent.lock()->right = child;
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

    if (node->parent.lock() == nullptr) {
        root = newRoot;
    } else if (node == node->parent.lock()->left) {
        node->parent.lock()->left = newRoot;
    } else {
        node->parent.lock()->right = newRoot;
    }

    newRoot->left = node;
    node->parent = newRoot;

    updateHeight(node);
    updateHeight(newRoot);
}

// RL rotation function
template<class T>
void AVL_Tree<T>::RL_rotation(shared_ptr<TreeNode<T>> node) {
    LL_rotation(node->right);
    RR_rotation(node);
}

// Get balance factor
template<class T>
int AVL_Tree<T>::get_BF(shared_ptr<TreeNode<T>> node) {
    int leftHeight = (node->left) ? node->left->height : 0;
    int rightHeight = (node->right) ? node->right->height : 0;
    return leftHeight - rightHeight;
}

template<class T>
int AVL_Tree<T>::max(int a, int b) {
    return (a > b) ? a : b;
}
