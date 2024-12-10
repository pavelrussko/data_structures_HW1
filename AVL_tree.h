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
    void inorder(TreeNode<T> *);
    void LL_rotation();
    void LR_rotation();
    void RR_rotation();
    void Rl_rotation();
    int height_difference(TreeNode<T> *);
    int get_BF(TreeNode<T>*);
};

// Constructor for AVL_Tree
template<class T>
AVL_Tree<T>::AVL_Tree() : root(nullptr) {}

// Insert function
template<class T>
StatusType AVL_Tree<T>::insert(TreeNode<T> *node) {
    // Implementation of insert function
}

// Search function
template<class T>
TreeNode<T> *AVL_Tree<T>::search(TreeNode<T> *node) {
    // Implementation of search function
}

// Removal function
template<class T>
StatusType AVL_Tree<T>::removal(TreeNode<T> *node) {
    // Implementation of removal function
}

// Inorder traversal function
template<class T>
void AVL_Tree<T>::inorder(TreeNode<T> *node) {
    // Implementation of inorder traversal function
}

// LL rotation function
template<class T>
void AVL_Tree<T>::LL_rotation() {
    // Implementation of LL rotation function
}

// LR rotation function
template<class T>
void AVL_Tree<T>::LR_rotation() {
    // Implementation of LR rotation function
}

// RR rotation function
template<class T>
void AVL_Tree<T>::RR_rotation() {
    // Implementation of RR rotation function
}

// RL rotation function
template<class T>
void AVL_Tree<T>::Rl_rotation() {
    // Implementation of RL rotation function
}

// Height difference function
template<class T>
int AVL_Tree<T>::height_difference(TreeNode<T> *node) {
    // Implementation of height difference function
}

template<class T>
int AVL_Tree<T>::get_BF(TreeNode<T>* node) {
    return node->left->height - node->right->height;
}