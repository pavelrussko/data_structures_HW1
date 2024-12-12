// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains() {

}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId) {
    return StatusType::FAILURE;
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    } else {
        herd herdToRemove(herdId);
        TreeNode<herd> herdNode(herdToRemove);
        TreeNode<herd> *foundNode = empty_herds.search(&herdNode);
        if (foundNode == nullptr) {
            return StatusType::FAILURE;
        } else {
            try {
                if (*(foundNode->data) == herdToRemove) {
                    empty_herds.removal(foundNode);
                } else {
                    return StatusType::FAILURE;
                }
            } catch (...) {
                return StatusType::FAILURE; // Ensure this is the correct exception handling
            }
        }
    }
    return StatusType::SUCCESS;
} //ofek

StatusType Plains::add_horse(int horseId, int speed) {
    return StatusType::FAILURE;
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    herd herdToJoin(herdId);
    TreeNode<herd> herdNode(herdToJoin);
    TreeNode<herd> *foundNode = empty_herds.search(
            &herdNode);// actual node of herd in emptyherds
    TreeNode<herd> *foundNode1 = herds.search(
            &herdNode); // actual node of herd in herds
    if (foundNode == nullptr && foundNode1 == nullptr) {
        return StatusType::FAILURE;
    } else if (foundNode == nullptr) { // herd is in herds
        horse horseToJoin(horseId); // fake horse
        TreeNode<horse> horseNode(horseToJoin); //fake node of horse
        TreeNode<horse> *foundNode2 = horses.search(
                &horseNode); // actual node of horse in horses
        if (foundNode2 == nullptr || *(foundNode2->data) != horseToJoin) {
            return StatusType::FAILURE;
        } else {
            foundNode2->data->set_herd_id(herdId); //updating herd id of horse
            horse toAdd = *foundNode2->data; //copying horse to add to herd
            TreeNode<horse> horseNodeToAdd(
                    toAdd); //creating new node of horse to add to herd
            foundNode1->data->herd_horses.insert(
                    &horseNodeToAdd); //adding horse to herd
            return StatusType::SUCCESS;
        }
    } else if (foundNode1 == nullptr) { // herd is in empty herds
        horse horseToJoin(horseId); // fake horse
        TreeNode<horse> horseNode(horseToJoin); //fake node of horse
        TreeNode<horse> *foundNode2 = horses.search(
                &horseNode); // actual node of horse in horses
        if (foundNode2 == nullptr || *(foundNode2->data) != horseToJoin) {
            return StatusType::FAILURE;
        } else {
            foundNode2->data->set_herd_id(herdId); //updating herd id of horse
            horse toAdd = *foundNode2->data; //copying horse to add to herd
            TreeNode<horse> horseNodeToAdd(
                    toAdd); //creating new node of horse to add to herd
            foundNode->data->herd_horses.insert(
                    &horseNodeToAdd); //adding horse to herd
            herds.moveToTree(foundNode,
                             &empty_herds); //moving herd from empty herds to herds
            return StatusType::SUCCESS;
        }
    }
} //ofek

StatusType Plains::follow(int horseId, int horseToFollowId) {
    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId) { // ofek
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    horse horseToLeave(horseId);
    TreeNode<horse> horseNode(horseToLeave);
    TreeNode<horse> *foundNode = horses.search(&horseNode);
    if (foundNode == nullptr || *(foundNode->data) != horseToLeave) {
        return StatusType::FAILURE;
    } else {
        int herdId = foundNode->data->get_herd_id();
        herd herdToLeave(herdId);
        TreeNode<herd> herdNode(herdToLeave);
        TreeNode<herd> *foundNode1 = herds.search(&herdNode);
        if (foundNode1 == nullptr || *(foundNode1->data) != herdToLeave) {
            return StatusType::FAILURE;
        } else {
            horse toRemove = *foundNode->data;
            foundNode->data->set_version(foundNode->data->get_version() + 1);
            TreeNode<horse> horseNodeToRemove(toRemove);
            foundNode1->data->herd_horses.removal(&horseNodeToRemove);
            foundNode->data->set_herd_id(0);
            if (foundNode1->data->herd_horses.isEmpty()) {
                herds.moveToTree(foundNode1, &empty_herds);
            }
            return StatusType::SUCCESS;
        }
    }
    return StatusType::FAILURE;
} //ofek

output_t<int> Plains::get_speed(int horseId) {
    return 0;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) {
        return false;
    }
    horse follower(horseId);
    TreeNode<horse> followerFakeNode(follower);
    horse leader(otherHorseId);
    TreeNode<horse> leaderFakeNode(leader);
    TreeNode<horse> *followerNode = horses.search(&followerFakeNode);
    TreeNode<horse> *leaderNode = horses.search(&leaderFakeNode);
    if (followerNode == nullptr || leaderNode == nullptr) {
        return false;
    }
    if (followerNode->data->get_follow_id() ==
        leaderNode->data->get_horse_id() &&
        followerNode->data->get_versionfollow() ==
        leaderNode->data->get_version()) {
        return true;
    } else {
        while (followerNode->data->get_follow() != nullptr) {
            followerNode->data = followerNode->data->get_follow();
            if (followerNode->data->get_horse_id() ==
                leaderNode->data->get_horse_id() &&
                followerNode->data->get_versionfollow() ==
                leaderNode->data->get_version()) {
                return true;
            }
        }
    }
    return false;
} //ofek

output_t<bool> Plains::can_run_together(int herdId) {
    return false;
}
