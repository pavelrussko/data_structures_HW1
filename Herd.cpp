#include "Herd.h"

// Comparison operator: herd_id <= integer
bool herd::operator<=(int a) const {
    return herd_id <= a;
}

// Comparison operator: less-than between herds
bool herd::operator<(const herd& other) const {
    return herd_id < other.herd_id;
}

// Equality operator for herds
bool herd::operator==(const herd& other) const {
    return herd_id == other.herd_id;
}

// Inequality operator for herds
bool herd::operator!=(const herd& other) const {
    return !(*this == other);
}

// Static method to create a TreeNode containing a herd
shared_ptr<TreeNode<herd>> herd::make_herd_node(int herdId) {
    auto toInsert = make_shared<herd>(herdId);
    return make_shared<TreeNode<herd>>(toInsert);
}
