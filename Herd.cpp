#include "Herd.h"

// Destructor for herd
herd::~herd() {
    // No need for anything special.
}

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
TreeNode<herd>* herd::make_herd_node(int herdId) {
    return new TreeNode<herd>(new herd(herdId));
}

herd& herd::operator=(const herd& other) {
    if (this == &other) {
        return *this; // Handle self-assignment
    }

    // Copy the herd_id
    herd_id = other.herd_id;
    herd_horses = other.herd_horses;

    return *this;
}