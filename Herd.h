//
// Created by Ofek on 11/12/2024.
//
#include "AVL_tree.h"
#include "Horse.h"

#ifndef DATA_STRUCTURES_HW1_HERD_H
#define DATA_STRUCTURES_HW1_HERD_H

class herd {
private:
    AVL_Tree<horse> herd_horses;
    int herd_id;
};

#endif //DATA_STRUCTURES_HW1_HERD_H
