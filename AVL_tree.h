
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
    void destroyTree(shared_ptr<TreeNode<T>> node);
public:
    ~AVL_Tree();
    AVL_Tree();
    StatusType insert(shared_ptr<TreeNode<T>>);
    shared_ptr<TreeNode<T>> search(shared_ptr<TreeNode<T>>);
    StatusType removal(shared_ptr<TreeNode<T>>);
    void inorder(shared_ptr<TreeNode<T>>, T *);
    void LL_rotation(shared_ptr<TreeNode<T>>);
    void LR_rotation(shared_ptr<TreeNode<T>>);
    void RR_rotation(shared_ptr<TreeNode<T>>);
    void Rl_rotation(shared_ptr<TreeNode<T>>);
    int get_BF(shared_ptr<TreeNode<T>>);
    AVL_Tree(T data);

    bool isEmpty() { return root == nullptr; }

    void moveToTree(shared_ptr<TreeNode<T>>, shared_ptr<AVL_Tree<T>>);
    int max(int a, int b);

    shared_ptr<TreeNode<T>> getRoot() { return root; }

    void updateHeight(shared_ptr<TreeNode<T>>);
};

template<class T>
void moveToTree(shared_ptr<TreeNode<T>> nodeToMove,
                shared_ptr<AVL_Tree<T>> treeToMoveTo) {
    removal(nodeToMove);
    treeToMoveTo->insert(nodeToMove);
}

template<class T>
AVL_Tree<T>::~AVL_Tree() {
    destroyTree(root);
}

template<class T>
AVL_Tree<T>::AVL_Tree(T data) {
    root = new TreeNode<T>();
    root->data = new T(data);
    root->left = nullptr;
    root->right = nullptr;
    root->parent = nullptr;
    root->height = 1;
}

template<class T>
void AVL_Tree<T>::moveToTree(shared_ptr<TreeNode<T>> node,
                             shared_ptr<AVL_Tree<T>> targetTree) {
    if (node == nullptr || targetTree == nullptr) {
        return;
    }

    // Detach the node from the current tree
    shared_ptr<TreeNode<T>> parent = node->parent;
    if (parent) {
        if (parent->left == node) {
            parent->left = nullptr;
        } else {
            parent->right = nullptr;
        }
    } else {
        root = nullptr;
    }

    // Update heights and balance the original tree
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

    // Reset node pointers
    node->parent = nullptr;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;

    // Insert the node into the target tree
    targetTree->insert(node);
}

template<class T>
void AVL_Tree<T>::destroyTree(shared_ptr<TreeNode<T>> node) {
    if (node == nullptr) {
        return;
    }
    destroyTree(node->left);
    destroyTree(node->right);
}

template<class T>
void AVL_Tree<T>::updateHeight(shared_ptr<TreeNode<T>> node) {
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
StatusType AVL_Tree<T>::insert(shared_ptr<TreeNode<T>> node) {
    if (*(node->data) < 0) {
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
    shared_ptr<TreeNode<T>> current = search(node);
    if (current != nullptr && *(current->data) == *(node->data)) {
        return StatusType::FAILURE;
    }
    current = root;
    shared_ptr<TreeNode<T>> parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (*(node->data) < *(current->data)) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    if (*(node->data) < *(parent->data)) {
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
//oogabooga

// Search function
template<class T>
shared_ptr<TreeNode<T>> AVL_Tree<T>::search(shared_ptr<TreeNode<T>> node) {
    shared_ptr<TreeNode<T>> current = root;
    while (current != nullptr && *(current->data) != *(node->data)) {
        if (*(node->data) < *(current->data)) {
            current = current->left;
        } else {
            current = current->right;
        }
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
void AVL_Tree<T>::inorder(shared_ptr<TreeNode<T>> node, T *arr) {
    //TODO
    return;
}

// LL rotation function
template<class T>
void AVL_Tree<T>::LL_rotation(shared_ptr<TreeNode<T>> node) {
    shared_ptr<TreeNode<T>> temp = node->left->right;
    shared_ptr<TreeNode<T>> child = node->left;
    node->left = temp;
    child->parent = node->parent;
    child->right = node;
    node->parent = child;
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
void AVL_Tree<T>::Rl_rotation(shared_ptr<TreeNode<T>> node) {
    LL_rotation(node->right);
    RR_rotation(node);
}


template<class T>
int AVL_Tree<T>::get_BF(shared_ptr<TreeNode<T>> node) {
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
