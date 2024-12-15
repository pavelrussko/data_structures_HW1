//
// Created by Ofek on 11/12/2024.
//

#include "Horse.h"

#ifndef DATA_STRUCTURES_HW1_HERD_H
#define DATA_STRUCTURES_HW1_HERD_H

class herd {
private:
    int herd_id;
public:
    AVL_Tree<horse> herd_horses;
    bool operator<=(int) const;
    bool operator<(const herd &) const;
    bool operator==(const herd &) const;
    bool operator!=(const herd &) const;

    herd(int herdId) : herd_id(herdId), herd_horses() {}

    static shared_ptr<TreeNode<herd>> make_herd_node(int herdId);

    int get_id() const { return herd_id; }
};

#endif //DATA_STRUCTURES_HW1_HERD_H
