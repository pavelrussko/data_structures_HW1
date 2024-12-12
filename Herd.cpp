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


