// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

//gg
Plains::Plains() : horses(), herds(), empty_herds() {}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    return empty_herds.insert(herd::make_herd_node(herdId));

}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Check in empty_herds
    shared_ptr<TreeNode<herd>> emptyHerdNode = empty_herds.search(
            herd::make_herd_node(herdId));
    if (!emptyHerdNode || emptyHerdNode->data->get_id() != herdId) {
        return StatusType::FAILURE;
    }
    if (emptyHerdNode || emptyHerdNode->data->get_id() == herdId) {
        empty_herds.removal(emptyHerdNode);
        return StatusType::SUCCESS;
    }


    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Create a fake node for the horse
    horse horseToLeave(horseId);
    // Search for the horse
    shared_ptr<TreeNode<horse>> horseNode = horses.search(
            horse::make_horse_node(horseId));
    if (!horseNode || horseNode->data->get_herd_id() == -1) {
        return StatusType::FAILURE;
    }

    int herdId = horseNode->data->get_herd_id();

    // Find the herd the horse belongs to
    herd herdToLeave(herdId);
    shared_ptr<TreeNode<herd>> herdNode = herds.search(
            herd::make_herd_node(herdId));
    if (!herdNode) {
        return StatusType::FAILURE;
    }

    // Remove the horse from the herd
    horseNode->data->set_version(horseNode->data->get_version() + 1);
    horseNode->data->set_herd_id(-1);

    herdNode->data->herd_horses.removal(horseNode);

    // Move herd to empty_herds if it becomes empty
    if (herdNode->data->herd_horses.isEmpty()) {
        herds.moveToTree(herdNode, empty_herds);
    }

    return StatusType::SUCCESS;
}

StatusType Plains::add_horse(int horseId, int speed) {
    if (horseId <= 0 || speed <= 0) {
        return StatusType::INVALID_INPUT;
    }
    shared_ptr<horse> horseToInsert = make_shared<horse>(horseId, speed);
    shared_ptr<TreeNode<horse>> NodeToInsert = make_shared<TreeNode<horse>>(
            horseToInsert);
    return horses.insert(NodeToInsert);//add node maker?
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Search for the herd in both herds and empty_herds
    shared_ptr<TreeNode<herd>> herdNode = herds.search(
            herd::make_herd_node(herdId));
    if (!herdNode || herdNode->data->get_id() !=
                     herdId) {  // Herd not found in non-empty herds, check empty herds
        herdNode = empty_herds.search(herd::make_herd_node(herdId));
        if (!herdNode ||
            herdNode->data->get_id() != herdId) {  // Herd not found at all
            return StatusType::FAILURE;
        }
    }

    // Search for the horse
    shared_ptr<TreeNode<horse>> horseNode = horses.search(
            horse::make_horse_node(horseId));
    if (horseNode->data->get_id() != horseId ||
        horseNode->data->get_herd_id() !=
        -1) {  // Horse doesn't exist or is already in a herd
        return StatusType::FAILURE;
    }

    // Add the horse to the herd
    horseNode->data->set_herd_id(herdId);  // Update horse's herd ID
    herdNode->data->herd_horses.insert(make_shared<TreeNode<horse>>(
            horseNode->data));  // Insert horse into the herd

    // If the herd was empty, move it from empty_herds to herds
    if (!empty_herds.isEmpty()) {
        if (empty_herds.search(herd::make_herd_node(herdId))->data->get_id() ==
            herdId) {
            empty_herds.moveToTree(herdNode, herds);  // Move to non-empty herds
        }
    }
    return StatusType::SUCCESS;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId == otherHorseId) {
        return StatusType::INVALID_INPUT;
    }
    if (horseId <= 0 || otherHorseId <= 0) {
        return StatusType::INVALID_INPUT; // Invalid input check
    }
    // Search for the follower and leader in the AVL tree
    shared_ptr<TreeNode<horse>> followerNode = horses.search(
            horse::make_horse_node(horseId));
    shared_ptr<TreeNode<horse>> leaderNode = horses.search(
            horse::make_horse_node(otherHorseId));


    // If horses dont exist, return false
    if (followerNode->data->get_id() != horseId ||
        leaderNode->data->get_id() != otherHorseId) {
        return StatusType::FAILURE;
    }
    // if horses arent in the same herd
    if (followerNode->data->get_herd_id() != leaderNode->data->get_herd_id()) {
        return false;
    }
    shared_ptr<horse> starter = followerNode->data;//for isvisited reset
    // Follow the chain of horses to see if the follower eventually leads the leader
    shared_ptr<TreeNode<horse>> currentNode = followerNode;
    bool foundLeader = false;
    shared_ptr<horse> currentHorse = currentNode->data;
    while (currentHorse) {

        // Check for cycles
        if (currentHorse->isVisited) {
            // if cycle detected, reset all visited flags and return false for cycle detection
            while (starter && starter->isVisited) {
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
        if (currentHorse->get_follow() == nullptr) {//no more potential leaders
            break;
        }
        // Move to the next horse in the chain
        if (currentHorse->get_versionfollow() ==
            currentHorse->get_follow()->get_version()) {
            currentHorse = currentHorse->get_follow();
        }
    }

    // Reset all visited flags before returning
    while (starter && starter->isVisited) {
        starter->isVisited = false;
        starter = starter->get_follow();
    }
    return foundLeader;
}

StatusType Plains::follow(int horseId, int horseToFollowId) {
    //input check
    if (horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId) {
        return StatusType::INVALID_INPUT;
    }
    //find horses
    shared_ptr<TreeNode<horse>> follower = horses.search(
            horse::make_horse_node(horseId));
    shared_ptr<TreeNode<horse>> toFollow = horses.search(
            horse::make_horse_node(horseToFollowId));
    //if horses dont exist
    if (follower->data->get_horse_id() != horseId ||
        toFollow->data->get_horse_id() != horseToFollowId) {
        return StatusType::FAILURE;
    }
    //if horses arent in the same herd
    if (follower->data->get_herd_id() != toFollow->data->get_herd_id() ||
        follower->data->get_herd_id() == -1 ||
        toFollow->data->get_herd_id() == -1) {
        return StatusType::FAILURE;
    }
    //set follow
    follower->data->set_follow(toFollow->data);
    follower->data->set_versionfollow(toFollow->data->get_version());
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT; // INVALID_INPUT for invalid IDs
    }

    shared_ptr<TreeNode<horse>> horseNode = horses.search(
            horse::make_horse_node(horseId));

    if (!horseNode || horseNode->data->get_horse_id() != horseId) {
        return StatusType::FAILURE; // Horse not found
    }

    return horseNode->data->get_speed();
}


output_t<bool> Plains::can_run_together(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    //searching for herd
    shared_ptr<TreeNode<herd>> herdNode = herds.search(
            herd::make_herd_node(herdId));
    if (!herdNode || herdNode->data->get_id() != herdId) {
        return StatusType::FAILURE;
    }

    //setting up for traversal with starting point
    shared_ptr<horse> potential_leader = nullptr;
    shared_ptr<TreeNode<horse>> current_horse = herdNode->data->herd_horses.getRoot();

    // Run traversal to check if all horses lead to the same leader.
    bool result = traversal(current_horse, potential_leader);

    // Reset visited flags for future calls (optional; depends on usage).
    resetVisitedFlags(current_horse);

    return output_t<bool>(result);
}

void Plains::resetVisitedFlags(shared_ptr<TreeNode<horse>> node) {
    if (!node) {
        return;
    }
    node->data->isVisited = false;
    resetVisitedFlags(node->left);
    resetVisitedFlags(node->right);
}

void Plains::resetVisitedLocal(shared_ptr<TreeNode<horse>> node) {
    if (!node) {
        return;
    }
    node->data->visited_local = false;
    resetVisitedLocal(node->left);
    resetVisitedLocal(node->right);
}

bool Plains::traversal(shared_ptr<TreeNode<horse>> node,
                       shared_ptr<horse> &potential_leader) {
    if (!node) {
        return true;
    }
    shared_ptr<horse> current = node->data;
    shared_ptr<TreeNode<horse>> temp = node;
    while (current->get_follow() && current
           && !current->isVisited && !current->visited_local) {
        if (current->get_follow()->get_version() !=
            current->get_versionfollow()) {
            current->isVisited = true;
            resetVisitedLocal(temp);
            break;
        }
        current->visited_local = true;
        current->isVisited = true;
        current = current->get_follow();
//        current = current->get_follow();
//        current->isVisited = true;
//        current->visited_local = true;
    }
    bool valid_lead = true;
    if (current->visited_local) {
        resetVisitedLocal(temp);
        valid_lead = false;
//        return false;
    } else if (current->isVisited) {
        resetVisitedLocal(temp);
//        return true;
    } else if (!potential_leader) {
        potential_leader = current;
        current->isVisited = true;
        //current->isVisited = true; ???
        resetVisitedLocal(temp);
    } else if (current == potential_leader) {
        resetVisitedLocal(temp);
//        return true;
    } else if (current != potential_leader) {
        resetVisitedLocal(temp);
        valid_lead = false;
//        return false;
    }

    bool left_valid = traversal(node->left, potential_leader);
    bool right_valid = traversal(node->right, potential_leader);
//    return traversal(node->left, potential_leader) &&
//           traversal(node->right, potential_leader);
//}
    return valid_lead && left_valid && right_valid;
}
/*bool Plains::traversal(shared_ptr<TreeNode<horse>> node,
                       shared_ptr<horse> &potential_leader) {
    if (!node) {
        return true; // Base case: no horse in this subtree.
    }
    


    // Recur for left and right children of the current node.
    return traversal(node->left, potential_leader) &&
           traversal(node->right, potential_leader);
}*/
