// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

Plains::Plains() : horses(), herds(), empty_herds() {}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<herd>* newHerdNode = herd::make_herd_node(herdId);
    return empty_herds.insert(newHerdNode);
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<herd>* emptyHerdNode = empty_herds.search(herd::make_herd_node(herdId));
    if (emptyHerdNode) {
        empty_herds.removal(emptyHerdNode);
        return StatusType::SUCCESS;
    }

    TreeNode<herd>* nonEmptyHerdNode = herds.search(herd::make_herd_node(herdId));
    if (nonEmptyHerdNode) {
        if (!nonEmptyHerdNode->data->herd_horses.isEmpty()) {
            return StatusType::FAILURE; // Not allowed to remove a herd with horses
        }
        herds.removal(nonEmptyHerdNode);
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE; // Herd not found
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<horse>* horseNode = horses.search(horse::make_horse_nodey(horseId, 0));
    if (!horseNode || horseNode->data->get_herd_id() == -1) {
        return StatusType::FAILURE; // Horse doesn't exist or is not in a herd
    }

    int herdId = horseNode->data->get_herd_id();
    TreeNode<herd>* herdNode = herds.search(herd::make_herd_node(herdId));
    if (!herdNode) {
        return StatusType::FAILURE; // Herd not found
    }

    horseNode->data->set_herd_id(-1);
    herdNode->data->herd_horses.removal(horseNode);

    if (herdNode->data->herd_horses.isEmpty()) {
        herds.removal(herdNode);
        empty_herds.insert(herdNode);
    }

    return StatusType::SUCCESS;
}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed < 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<horse>* newHorseNode = horse::make_horse_nodey(horseId, speed);
    return horses.insert(newHorseNode);
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Search for the herd in both herds and empty_herds
    TreeNode<herd>* herdNode = herds.search(herd::make_herd_node(herdId));
    if (!herdNode || herdNode->data->get_id() != herdId) {  // Herd not found in non-empty herds, check empty herds
        herdNode = empty_herds.search(herd::make_herd_node(herdId));
        if (!herdNode || herdNode->data->get_id() != herdId) {  // Herd not found at all
            return StatusType::FAILURE;
        }
    }

    // Search for the horse
    TreeNode<horse>* horseNode = horses.search(horse::make_horse_nodey(horseId,0));
    if (!horseNode || horseNode->data->get_id() != horseId || horseNode->data->get_herd_id() != -1) {  // Horse doesn't exist or is already in a herd
        return StatusType::FAILURE;
    }

    // Add the horse to the herd
    horseNode->data->set_herd_id(herdId);  // Update horse's herd ID
    herdNode->data->herd_horses.insert(horse::make_horse_node(horseNode->data));  // Insert horse into the herd

    // If the herd was empty, move it from empty_herds to herds
    TreeNode<herd>* herdNodeSearchResult = empty_herds.search(herdNode);
    if (!herdNodeSearchResult || herdNodeSearchResult->data->get_id() == herdId) {
        empty_herds.detach(herdNode);// Safely remove and delete the node
        herds.insert(herdNode);  // Insert new node with the copied data
    }
    return StatusType::SUCCESS;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    // Input check
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }

    // Find the horses in the global horses tree
    TreeNode<horse>* follower = horses.search(horse::make_horse_nodey(horseId, 0));
    TreeNode<horse>* toFollow = horses.search(horse::make_horse_nodey(horseToFollowId, 0));

    // If either horse doesn't exist in the global horses tree, return failure
    if (!follower || !toFollow || follower->data->get_horse_id() != horseId || toFollow->data->get_horse_id() != horseToFollowId) {
        return StatusType::FAILURE;
    }

    // Ensure the horses belong to the same herd
    if (follower->data->get_herd_id() != toFollow->data->get_herd_id()) {
        return StatusType::FAILURE;
    }

    // Find the herd node in the herds tree
    TreeNode<herd>* herdNode = herds.search(herd::make_herd_node(follower->data->get_herd_id()));
    if (!herdNode) {
        return StatusType::FAILURE;
    }

    // Find the follower and toFollow horses within the herd's horses tree
    TreeNode<horse>* herdFollower = herdNode->data->herd_horses.search(horse::make_horse_nodey(horseId, 0));
    TreeNode<horse>* herdToFollow = herdNode->data->herd_horses.search(horse::make_horse_nodey(horseToFollowId, 0));

    // If horses aren't found in the herd's horses tree, return failure
    if (!herdFollower || !herdToFollow || herdFollower->data->get_horse_id() != horseId || herdToFollow->data->get_horse_id() != horseToFollowId) {
        return StatusType::FAILURE;
    }

    // Set the follow relationship in both the global tree and the herd's tree
    // Set follower to follow toFollow in both the global horses tree and the herd_horses tree

    // Set global follow relationship
    follower->data->set_follow(toFollow->data);
    follower->data->set_versionfollow(toFollow->data->get_version());

    // Set herd-level follow relationship
    herdFollower->data->set_follow(herdToFollow->data);
    herdFollower->data->set_versionfollow(herdToFollow->data->get_version());

    // If both horses are trying to follow each other, we need to ensure the follow relationship works both ways.
    if (follower->data->get_horse_id() == toFollow->data->get_horse_id()) {
        toFollow->data->set_follow(follower->data);
        toFollow->data->set_versionfollow(follower->data->get_version());

        // Update the version for herd-level horses
        herdToFollow->data->set_follow(herdFollower->data);
        herdToFollow->data->set_versionfollow(herdFollower->data->get_version());
    }

    return StatusType::SUCCESS;
}



output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT; // INVALID_INPUT for invalid IDs
    }

    TreeNode<horse>* horseNode = horses.search(horse::make_horse_nodey(horseId,0));

    if (!horseNode || horseNode->data->get_horse_id() != horseId) {
        return StatusType::FAILURE; // Horse not found
    }

    return horseNode->data->get_speed();
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) {
        return false; // Invalid input check
    }
    // Search for the follower and leader in the AVL tree
    TreeNode<horse>* followerNode = horses.search(horse::make_horse_nodey(horseId,0));
    TreeNode<horse>* leaderNode = horses.search(horse::make_horse_nodey(otherHorseId,0));

    // If horses don't exist, return false
    if (followerNode->data->get_id() != horseId || leaderNode->data->get_id() != otherHorseId) {
        return false;
    }
    // If horses aren't in the same herd
    if (followerNode->data->get_herd_id() != leaderNode->data->get_herd_id()) {
        return false;
    }
    horse* starter = followerNode->data; // For isVisited reset
    // Follow the chain of horses to see if the follower eventually leads the leader
    horse* currentHorse = followerNode->data;
    bool foundLeader = false;
    while (currentHorse) {
        // Check for cycles
        if (currentHorse->isVisited) {
            // If cycle detected, reset all visited flags and return false for cycle detection
            while (starter) {
                starter->isVisited = false;
                starter = starter->get_follow();
            }
            return false; // Cycle detected, return false
        }

        // Mark the current horse as visited
        currentHorse->isVisited = true;

        // Check if this horse is the leader
        if (currentHorse->get_horse_id() == leaderNode->data->get_horse_id()) {
            foundLeader = true;
            break;
        }
        if (currentHorse->get_follow() == nullptr) { // No more potential leaders
            break;
        }
        // Move to the next horse in the chain
        if (currentHorse->get_versionfollow() == currentHorse->get_follow()->get_version()) {
            currentHorse = currentHorse->get_follow();
        }
    }

    // Reset all visited flags before returning
    while (starter) {
        starter->isVisited = false;
        starter = starter->get_follow();
    }
    return foundLeader;
}

output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<herd>* herdNode = herds.search(herd::make_herd_node(herdId));
    if (!herdNode || herdNode->data->get_id() != herdId) {
        return StatusType::FAILURE;
    }

    horse* potential_leader = nullptr;
    TreeNode<horse>* current_horse = herdNode->data->herd_horses.getRoot();

    // Run traversal to check if all horses lead to the same leader.
    bool result = traversal(current_horse, potential_leader);

    // Reset visited flags for future calls
    resetVisitedFlags(current_horse);

    return output_t<bool>(result);
}



void Plains::resetVisitedFlags(TreeNode<horse>* node) {
    if (!node) {
        return;
    }
    node->data->isVisited = false;
    resetVisitedFlags(node->left);
    resetVisitedFlags(node->right);
}

bool Plains::traversal(TreeNode<horse>* node, horse*& potential_leader) {
    if (!node) {
        return true; // Base case: no horse in this subtree.
    }

    horse* current = node->data;

    // Ensure the horse is not visited
    if (current->isVisited) {
        return true; // Prevent revisiting a horse
    }

    // Check for cycles and identify the leader
    horse* slow = current, *fast = current;

    // Traverse the follow chain to detect a cycle
    while (fast && fast->get_follow() &&
           fast->get_versionfollow() == fast->get_follow()->get_version()) {
        slow = slow->get_follow();
        fast = fast->get_follow();
        if (fast) fast = fast->get_follow();

        // If a cycle is detected, return false
        if (slow == fast) {
            return false; // A cycle is detected.
        }
           }

    // Now `current` should be pointing to the last horse in the follow chain (the leader).
    while (current->get_follow() &&
           current->get_versionfollow() == current->get_follow()->get_version()) {
        current = current->get_follow();
           }

    // Set or check the leader consistency
    if (!potential_leader) {
        potential_leader = current; // Set the first leader
    } else if (current != potential_leader) {
        return false; // Different leader, horses can't run together
    }

    // Mark the current horse as visited
    current->isVisited = true;

    // Continue the traversal for left and right subtrees
    return traversal(node->left, potential_leader) && traversal(node->right, potential_leader);
}


