#include "Herd.h"


bool herd::operator<=(int a) const{
    if(herd_id <= a){
        return true;
    }
    return false;
}

bool herd::operator<(const herd& other) const{
    if(herd_id < other.herd_id){
        return true;
    }
    return false;
}

bool herd::operator==(const herd& other) const{
    if(herd_id == other.herd_id){
        return true;
    }
    return false;
}

bool herd::operator!=(const herd& other) const {
    return !(*this == other);
}

shared_ptr<TreeNode<herd>> herd::make_herd_node(int herdId){
    shared_ptr<herd> toInsert = make_shared<herd>(herdId);
    shared_ptr<TreeNode<herd>> NodeToInsert = make_shared<TreeNode<herd>>(toInsert);
    return NodeToInsert;
}


