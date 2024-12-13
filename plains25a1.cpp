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
    }

    herd herdToRemove(herdId);
    TreeNode<herd> herdNode(&herdToRemove);

    // Check in empty_herds
    TreeNode<herd> *emptyHerdNode = empty_herds.search(&herdNode);
    if (emptyHerdNode) {
        empty_herds.removal(emptyHerdNode);
        return StatusType::SUCCESS;
    }

    // Check in herds
    TreeNode<herd> *nonEmptyHerdNode = herds.search(&herdNode);
    if (nonEmptyHerdNode) {
        if (!nonEmptyHerdNode->data->herd_horses.isEmpty()) {
            return StatusType::FAILURE; // now allowed to remove a herd with horses
        }
        herds.removal(nonEmptyHerdNode);
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE; // Herd not found at all
}
//ofek

StatusType Plains::add_horse(int horseId, int speed) {
    return StatusType::FAILURE;
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Create a fake node for the herd
    herd herdToJoin(herdId);
    TreeNode<herd> fakeHerdNode(&herdToJoin);

    // Search for the herd in either tree
    TreeNode<herd> *herdNode = herds.search(&fakeHerdNode);
    if (!herdNode) {
        herdNode = empty_herds.search(&fakeHerdNode);
        if (!herdNode) {
            return StatusType::FAILURE;
        }
    }

    // Create a fake node for the horse
    horse horseToJoin(horseId);
    TreeNode<horse> fakeHorseNode(&horseToJoin);

    // Check if the horse exists and is not already in a herd
    TreeNode<horse> *horseNode = horses.search(&fakeHorseNode);
    if (!horseNode || horseNode->data->get_herd_id() != 0) {
        return StatusType::FAILURE;
    }

    // Add the horse to the herd
    horseNode->data->set_herd_id(herdId);
    herdNode->data->herd_horses.insert(new TreeNode<horse>(horseNode->data));

    // Move herd from empty_herds to herds if necessary
    if (empty_herds.search(&fakeHerdNode)) {
        herds.moveToTree(herdNode, &empty_herds);
    }

    return StatusType::SUCCESS;
}

//ofek

StatusType Plains::follow(int horseId, int horseToFollowId) {
    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Create a fake node for the horse
    horse horseToLeave(horseId);
    TreeNode<horse> fakeHorseNode(&horseToLeave);

    // Search for the horse
    TreeNode<horse> *horseNode = horses.search(&fakeHorseNode);
    if (!horseNode || horseNode->data->get_herd_id() == 0) {
        return StatusType::FAILURE;
    }

    int herdId = horseNode->data->get_herd_id();

    // Create a fake node for the herd
    herd herdToLeave(herdId);
    TreeNode<herd> fakeHerdNode(&herdToLeave);

    // Find the herd the horse belongs to
    TreeNode<herd> *herdNode = herds.search(&fakeHerdNode);
    if (!herdNode) {
        return StatusType::FAILURE;
    }

    // Remove the horse from the herd
    horseNode->data->set_version(horseNode->data->get_version() + 1);
    horseNode->data->set_herd_id(0);

    TreeNode<horse> horseNodeToRemove(horseNode->data);
    herdNode->data->herd_horses.removal(&horseNodeToRemove);

    // Move herd to empty_herds if it becomes empty
    if (herdNode->data->herd_horses.isEmpty()) {
        empty_herds.moveToTree(herdNode, &herds);
    }

    return StatusType::SUCCESS;
}

//ofek

output_t<int> Plains::get_speed(int horseId) {
    return 0;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) {
        return output_t<bool>(false); // Invalid input check
    }

    // Create fake nodes for both horses
    horse follower(horseId);
    TreeNode<horse> followerFakeNode(&follower);
    horse leader(otherHorseId);
    TreeNode<horse> leaderFakeNode(&leader);

    // Search for the follower and leader in the AVL tree
    TreeNode<horse> *followerNode = horses.search(&followerFakeNode);
    TreeNode<horse> *leaderNode = horses.search(&leaderFakeNode);

    // If either horse doesn't exist, return false
    if (followerNode == nullptr || leaderNode == nullptr) {
        return output_t<bool>(false);
    }

    // Check if the follower is directly following the leader
    if (followerNode->data->get_follow_id() == leaderNode->data->get_horse_id() &&
        followerNode->data->get_versionfollow() == leaderNode->data->get_version()) {
        return output_t<bool>(true);
    }

    // Follow the chain of horses to see if the follower eventually leads the leader
    TreeNode<horse>* currentNode = followerNode;
    while (currentNode) {
        // Get the horse to follow
        horse* nextHorse = currentNode->data->get_follow();

        // If there's a next horse, continue the search
        if (nextHorse) {
            TreeNode<horse> fakeNode(nextHorse); // Create a fake node with the next horse data
            currentNode = horses.search(&fakeNode); // Find the corresponding node in the tree

            // Check if this horse is the leader
            if (currentNode && currentNode->data->get_horse_id() == leaderNode->data->get_horse_id() &&
                currentNode->data->get_versionfollow() == leaderNode->data->get_version()) {
                return output_t<bool>(true);
            }
        } else {
            // No more horses to follow
            currentNode = nullptr;
        }
    }

    // If the loop ends without finding the leader
    return output_t<bool>(false);
}

//ofek

output_t<bool> Plains::can_run_together(int herdId) {
    return false;
}
