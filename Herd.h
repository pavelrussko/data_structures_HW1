//
// Created by Ofek on 11/12/2024.
//
#include "AVL_tree.h"
#include "Horse.h"

#ifndef DATA_STRUCTURES_HW1_HERD_H
#define DATA_STRUCTURES_HW1_HERD_H

class herd {
private:
    int herd_id;
public:
    AVL_Tree<horse> herd_horses;
    herd(int id);
    herd();
    ~herd();
    bool isEmpty(); //for pasha
    bool operator<=(int a) const;
    bool operator<(const herd &other) const;
    bool operator==(const herd &other) const;
    bool operator!=(const herd &other) const;
};

bool herd::operator<=(int a) const {
    if (herd_id <= a) {
        return true;
    }
    return false;
}

bool herd::operator<(const herd &other) const {
    if (herd_id < other.herd_id) {
        return true;
    }
    return false;
}

bool herd::operator==(const herd &other) const {
    if (herd_id == other.herd_id) {
        return true;
    }
    return false;
}

bool herd::operator!=(const herd &other) const {
    return !(*this == other);
}

#endif //DATA_STRUCTURES_HW1_HERD_H
