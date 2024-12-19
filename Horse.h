
#include "wet1util.h"
#include <memory>
#include "AVL_tree.h"

using namespace std;

//ddd
class horse {
private:
    int horse_id;
    int version;
    int speed;
    int herd_id;
    int follow_id;
    int versionfollow;
    weak_ptr<horse> follow;
public:
    bool isVisited = false;
    bool visited_local = false;
    bool isLead = false;
    int get_version();
    int get_horse_id();
    int get_speed();
    int get_herd_id();
    int get_follow_id();
    int get_versionfollow();
    weak_ptr<horse> get_follow();
    void set_version(int version);
    void set_herd_id(int herd_id);
    void set_follow_id(int follow_id);
    void set_versionfollow(int versionfollow);
    void set_follow(shared_ptr<horse> follow);
    horse(int horse_id, int speed);
    static shared_ptr<TreeNode<horse>>
    make_horse_node(int horse_id, int speed = 0);
    horse(int horse_id);

    int get_id() const { return horse_id; }

    bool operator<(const horse &other) const {
        return horse_id < other.horse_id;
    }

    horse &operator=(const horse &other) {
        horse_id = other.horse_id;
        version = other.version;
        speed = other.speed;
        herd_id = other.herd_id;
        follow_id = other.follow_id;
        versionfollow = other.versionfollow;
        follow = other.follow;
        return *this;
    }

    bool operator==(const horse &other) const {
        return horse_id == other.horse_id;
    }

    bool operator!=(const horse &other) const {
        return !(*this == other);
    }

    bool operator<=(int a) const {
        if (horse_id <= a) {
            return true;
        }
        return false;
    }
};