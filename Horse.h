#include "wet1util.h"

class horse {
private:
    int horse_id;
    int version;
    int speed;
    int herd_id;
    int follow_id;
    int versionfollow;
    horse* follow;
public:
    horse(int horse_id, int speed);
};