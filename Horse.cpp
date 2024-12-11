//
// Created by Ofek on 11/12/2024.
//
#include "Horse.h"

horse::horse(int horse_id, int speed) : horse_id(horse_id), speed(speed),
                                        herd_id(-1), follow_id(-1), version(0),
                                        versionfollow(0), follow(nullptr) {
}

int horse::get_version() {
    return version;
}

int horse::get_horse_id() {
    return horse_id;
}

int horse::get_speed() {
    return speed;
}

int horse::get_herd_id() {
    return herd_id;
}

int horse::get_follow_id() {
    return follow_id;
}

int horse::get_versionfollow() {
    return versionfollow;
}

horse *horse::get_follow() {
    return follow;
}

void horse::set_version(int version) {
    this->version = version;
}

void horse::set_herd_id(int herd_id) {
    this->herd_id = herd_id;
}

void horse::set_follow_id(int follow_id) {
    this->follow_id = follow_id;
}

void horse::set_versionfollow(int versionfollow) {
    this->versionfollow = versionfollow;
}

void horse::set_follow(horse *follow) {
    this->follow = follow;
    set_follow_id(follow->get_horse_id());
    set_versionfollow(follow->get_version());
}
