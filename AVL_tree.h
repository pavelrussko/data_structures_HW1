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
    void LL_rotation(TreeNode<T>*);
    void LR_rotation(TreeNode<T>*);
    void RR_rotation(TreeNode<T>*);
    void Rl_rotation(TreeNode<T>*);
    int get_BF(TreeNode<T>*);
    void (*get_rotation(TreeNode<T>*))();
};

// Constructor for AVL_Tree
template<class T>
AVL_Tree<T>::AVL_Tree() : root(nullptr) {}

// Insert function
template<class T>
StatusType AVL_Tree<T>::insert(TreeNode<T> *node) {
    if(node->data < 0){
        return StatusType::INVALID_INPUT;
    }
    TreeNode<T>* current = search(node);
    if(current == node){
        return StatusType::FAILURE;
    }
    int BF;
    node->parent = current;
    if(node->data > current){
        current->right = node;
    } else {
        current->left = current;
    }
    while(current) {
        BF = get_BF(current);
        if (BF > 1 || BF < -1) {
            int child_BF;
            if (BF > 1) {
                child_BF = get_BF(current->left);
                if (child_BF >= 0) {
                    LL_rotation(node);
                    return StatusType::SUCCESS;
                } else {
                    LR_rotation(node);
                    return StatusType::SUCCESS;
                }
            } else {
                child_BF = get_BF(current->right);
                if (child_BF <= 0) {
                    RR_rotation(node);
                    return StatusType::SUCCESS;
                } else {
                    Rl_rotation(node);
                    return StatusType::SUCCESS;
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
void AVL_Tree<T>::LL_rotation(TreeNode<T>* node) {
    // Implementation of LL rotation function
}

// LR rotation function
template<class T>
void AVL_Tree<T>::LR_rotation(TreeNode<T>* node) {
    // Implementation of LR rotation function
}

// RR rotation function
template<class T>
void AVL_Tree<T>::RR_rotation(TreeNode<T>* node) {
    // Implementation of RR rotation function
}

// RL rotation function
template<class T>
void AVL_Tree<T>::Rl_rotation(TreeNode<T>* node) {
    // Implementation of RL rotation function
}


template<class T>
int AVL_Tree<T>::get_BF(TreeNode<T>* node) {
    return node->left->height - node->right->height;
}

