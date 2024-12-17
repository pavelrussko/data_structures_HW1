//
// Created by Ofek on 11/12/2024.
//
#include "Horse.h"

horse::horse(int horse_id, int speed)
        : horse_id(horse_id), version(horse_id), speed(speed),
          herd_id(-1), follow_id(-1),
          versionfollow(0), follow(weak_ptr<horse>()) {
    /*if (speed <= 0 || horse_id <= 0) {
        throw StatusType::INVALID_INPUT; //change it
    }*/
}

horse::horse(int horse_id) : horse_id(horse_id), version(horse_id), speed(0),
                             herd_id(-1), follow_id(-1),
                             versionfollow(0), follow(weak_ptr<horse>()) {
    /*  if (horse_id <= 0) {
          throw StatusType::INVALID_INPUT; //change it
      }*/
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

shared_ptr<horse> horse::get_follow() {
    return follow.lock();
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

void horse::set_follow(shared_ptr<horse> follow) {
    this->follow = follow;
    set_follow_id(follow->get_horse_id());
    set_versionfollow(follow->get_version());
}

shared_ptr<TreeNode<horse>> horse::make_horse_node(int horse_id, int speed) {
    shared_ptr<horse> toInsert = make_shared<horse>(horse_id, speed);
    shared_ptr<TreeNode<horse>> NodeToInsert = make_shared<TreeNode<horse>>(
            toInsert);
    return NodeToInsert;
}
