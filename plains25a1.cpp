// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

Plains::Plains() : horses(), herds(), empty_herds() {}

Plains::~Plains() {}

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<herd>* newHerdNode = herd::make_herd_node(herdId);
    StatusType result = empty_herds.insert(newHerdNode);

    if (result != StatusType::SUCCESS) {
        delete newHerdNode; // Clean up memory if insertion fails
    }

    return result;
}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<herd>* emptyHerdNode = empty_herds.search(herd::make_herd_node(herdId));
    if (emptyHerdNode) {
        empty_herds.removal(emptyHerdNode);
        delete emptyHerdNode->data;  // Ensure herd memory is freed
        delete emptyHerdNode;  // Free the node itself
        return StatusType::SUCCESS;
    }

    TreeNode<herd>* nonEmptyHerdNode = herds.search(herd::make_herd_node(herdId));
    if (nonEmptyHerdNode) {
        if (!nonEmptyHerdNode->data->herd_horses.isEmpty()) {
            return StatusType::FAILURE;
        }
        herds.removal(nonEmptyHerdNode);
        delete nonEmptyHerdNode->data;  // Ensure herd memory is freed
        delete nonEmptyHerdNode;  // Free the node itself
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE;
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

    // Update horse's herd ID to -1 (indicating it's not in any herd)
    horseNode->data->set_herd_id(-1);

    // Remove horse from herd
    herdNode->data->herd_horses.removal(horseNode);

    // If the herd is now empty, move it to empty herds
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

    // Create a new horse node
    TreeNode<horse>* newHorseNode = horse::make_horse_nodey(horseId, speed);

    // Check if insertion was successful (avoid memory leak if insertion fails)
    if (newHorseNode && horses.insert(newHorseNode) == StatusType::SUCCESS) {
        return StatusType::SUCCESS;
    }

    // If insertion fails, clean up the allocated memory for the new horse
    if (newHorseNode) {
        delete newHorseNode;  // Clean up allocated memory to prevent leaks
    }

    return StatusType::FAILURE; // If insertion fails
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
    TreeNode<horse>* horseNode = horses.search(horse::make_horse_nodey(horseId, 0));
    if (!horseNode || horseNode->data->get_id() != horseId || horseNode->data->get_herd_id() != -1) {  // Horse doesn't exist or is already in a herd
        return StatusType::FAILURE;
    }

    // Add the horse to the herd
    horseNode->data->set_herd_id(herdId);  // Update horse's herd ID
    herdNode->data->herd_horses.insert(horse::make_horse_node(horseNode->data));  // Insert horse into the herd

    // If the herd was empty, move it from empty_herds to herds
    TreeNode<herd>* herdNodeSearchResult = empty_herds.search(herdNode);
    if (!herdNodeSearchResult || herdNodeSearchResult->data->get_id() == herdId) {
        empty_herds.detach(herdNode);  // Detach from empty_herds
        herds.insert(herdNode);  // Insert new node with the copied data
    }

    return StatusType::SUCCESS;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<horse>* follower = horses.search(horse::make_horse_nodey(horseId, 0));
    TreeNode<horse>* toFollow = horses.search(horse::make_horse_nodey(horseToFollowId, 0));

    if (!follower || !toFollow || follower->data->get_horse_id() != horseId || toFollow->data->get_horse_id() != horseToFollowId) {
        return StatusType::FAILURE;
    }

    if (follower->data->get_herd_id() != toFollow->data->get_herd_id()) {
        return StatusType::FAILURE;  // Horses must be in the same herd
    }

    // Set the follow relationship
    follower->data->set_follow(toFollow->data);
    follower->data->set_versionfollow(toFollow->data->get_version() + 1);  // Increment version to avoid issues with stale data

    return StatusType::SUCCESS;
}


output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    TreeNode<horse>* horseNode = horses.search(horse::make_horse_nodey(horseId,0));
    if (!horseNode || horseNode->data->get_horse_id() != horseId) {
        return StatusType::FAILURE;
    }

    return horseNode->data->get_speed();
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) {
        return false;
    }

    TreeNode<horse>* followerNode = horses.search(horse::make_horse_nodey(horseId,0));
    TreeNode<horse>* leaderNode = horses.search(horse::make_horse_nodey(otherHorseId,0));

    if (!followerNode || !leaderNode || followerNode->data->get_horse_id() != horseId || leaderNode->data->get_horse_id() != otherHorseId) {
        return false;
    }

    if (followerNode->data->get_herd_id() != leaderNode->data->get_herd_id()) {
        return false;
    }

    horse* currentHorse = followerNode->data;
    while (currentHorse) {
        if (currentHorse->get_horse_id() == leaderNode->data->get_horse_id()) {
            return true;
        }
        currentHorse = currentHorse->get_follow();
    }

    return false;
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
        return true;  // Base case: no horse in this subtree
    }

    horse* current = node->data;

    // Ensure the horse is not visited
    if (current->isVisited) {
        return true;  // Prevent revisiting a horse
    }

    // Check for cycles using slow and fast pointers
    horse* slow = current, *fast = current;
    while (fast && fast->get_follow() &&
           fast->get_versionfollow() == fast->get_follow()->get_version()) {
        slow = slow->get_follow();
        fast = fast->get_follow();
        if (fast) fast = fast->get_follow();

        if (slow == fast) {
            return false;  // A cycle is detected
        }
           }

    // Now `current` should be pointing to the last horse in the follow chain (the leader)
    while (current->get_follow() &&
           current->get_versionfollow() == current->get_follow()->get_version()) {
        current = current->get_follow();
           }

    // Set or check the leader consistency
    if (!potential_leader) {
        potential_leader = current;  // Set the first leader
    } else if (current != potential_leader) {
        return false;  // Different leader, horses can't run together
    }

    // Mark the current horse as visited
    current->isVisited = true;

    // Continue the traversal for left and right subtrees
    return traversal(node->left, potential_leader) && traversal(node->right, potential_leader);
}



