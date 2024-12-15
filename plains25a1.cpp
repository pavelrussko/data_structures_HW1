// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"

//gg
Plains::Plains():horses(), herds(), empty_herds(){}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId)
{
    if(herdId <= 0){
        return StatusType::INVALID_INPUT;
    }

    return empty_herds.insert(herd::make_herd_node(herdId));

}

StatusType Plains::remove_herd(int herdId) {
    if (herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Check in empty_herds
    shared_ptr<TreeNode<herd>> emptyHerdNode = empty_herds.search(herd::make_herd_node(herdId));
    if (emptyHerdNode) {
        empty_herds.removal(emptyHerdNode);
        return StatusType::SUCCESS;
    }

    // Check in herds
    shared_ptr<TreeNode<herd> >nonEmptyHerdNode = herds.search(herd::make_herd_node(herdId));
    if (nonEmptyHerdNode) {
        if (!nonEmptyHerdNode->data->herd_horses.isEmpty()) {
            return StatusType::FAILURE; // now allowed to remove a herd with horses
        }
        herds.removal(nonEmptyHerdNode);
        return StatusType::SUCCESS;
    }

    return StatusType::FAILURE; // Herd not found at all
}

StatusType Plains::leave_herd(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Create a fake node for the horse
    horse horseToLeave(horseId);
    // Search for the horse
    shared_ptr<TreeNode<horse>> horseNode = horses.search(horse::make_horse_node(horseId));
    if (!horseNode || horseNode->data->get_herd_id() == -1) {
        return StatusType::FAILURE;
    }

    int herdId = horseNode->data->get_herd_id();

    // Find the herd the horse belongs to
    herd herdToLeave(herdId);
    shared_ptr<TreeNode<herd>> herdNode = herds.search(herd::make_herd_node(herdId));
    if (!herdNode) {
        return StatusType::FAILURE;
    }

    // Remove the horse from the herd
    horseNode->data->set_version(horseNode->data->get_version() + 1);
    horseNode->data->set_herd_id(-1);

    herdNode->data->herd_horses.removal(horseNode);

    // Move herd to empty_herds if it becomes empty
    if (herdNode->data->herd_horses.isEmpty()) {
        herds.moveToTree(herdNode,empty_herds);
    }

    return StatusType::SUCCESS;
}

StatusType Plains::add_horse(int horseId, int speed)
{
    shared_ptr<horse> horseToInsert = make_shared<horse>(horseId, speed);
    shared_ptr<TreeNode<horse>> NodeToInsert = make_shared<TreeNode<horse>>(horseToInsert);
    return horses.insert(NodeToInsert);//add node maker?
}

StatusType Plains::join_herd(int horseId, int herdId) {
    if (horseId <= 0 || herdId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    // Search for the herd in both herds and empty_herds
    shared_ptr<TreeNode<herd>> herdNode = herds.search(herd::make_herd_node(herdId));
    if (!herdNode || herdNode->data->get_id() != herdId) {  // Herd not found in non-empty herds, check empty herds
        herdNode = empty_herds.search(herd::make_herd_node(herdId));
        if (herdNode->data->get_id() != herdId) {  // Herd not found at all
            return StatusType::FAILURE;
        }
    }

    // Search for the horse
    shared_ptr<TreeNode<horse>> horseNode = horses.search(horse::make_horse_node(horseId));
    if (horseNode->data->get_id() != horseId || horseNode->data->get_herd_id() != -1) {  // Horse doesn't exist or is already in a herd
        return StatusType::FAILURE;
    }

    // Add the horse to the herd
    horseNode->data->set_herd_id(herdId);  // Update horse's herd ID
    herdNode->data->herd_horses.insert(make_shared<TreeNode<horse>>(horseNode->data));  // Insert horse into the herd

    // If the herd was empty, move it from empty_herds to herds
    if(!empty_herds.isEmpty()){
    if (empty_herds.search(herd::make_herd_node(herdId))->data->get_id() == herdId) {
        empty_herds.moveToTree(herdNode, herds);  // Move to non-empty herds
    }
}
    return StatusType::SUCCESS;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId) {
    if (horseId <= 0 || otherHorseId <= 0) {
        return false; // Invalid input check
    }

    // Search for the follower and leader in the AVL tree

    shared_ptr<TreeNode<horse>> followerNode = horses.search(horse::make_horse_node(horseId));
    shared_ptr<TreeNode<horse>> leaderNode = horses.search(horse::make_horse_node(otherHorseId));


    // If either horse doesn't exist, return false
    if (followerNode->data->get_id() != horseId || leaderNode->data->get_id() != otherHorseId) {
        return false;
    }

    // Follow the chain of horses to see if the follower eventually leads the leader
    shared_ptr<TreeNode<horse>> currentNode = followerNode;
    bool foundLeader = false;

    while (currentNode) {
        shared_ptr<horse> currentHorse = currentNode->data;

        // Check for cycles
        if (currentHorse->isVisited) {
            // Reset all visited flags and return false for cycle detection
            currentNode = followerNode;
            while (currentNode) {
                currentNode->data->isVisited = false;
                shared_ptr<horse> nextHorse = currentNode->data->get_follow();
                if (!nextHorse) { break; }
                shared_ptr<TreeNode<horse>> fakeNode = make_shared<TreeNode<horse>>(nextHorse);
                currentNode = horses.search(fakeNode);
            }
            return false; // Cycle detected
        }

        // Mark the current horse as visited
        currentHorse->isVisited = true;

        // Check if this horse is the leader
        if (currentHorse->get_horse_id() == leaderNode->data->get_horse_id() &&
            currentHorse->get_versionfollow() ==
            leaderNode->data->get_version()) {
            foundLeader = true;
            break;
        }

        // Move to the next horse in the chain
        shared_ptr<horse> nextHorse = currentHorse->get_follow();
        if (!nextHorse) {
            break; // No more horses to follow
        }

        // Create a fake node with the next horse data and find the corresponding node in the tree
        shared_ptr<TreeNode<horse>> fakeNode1 = make_shared<TreeNode<horse>>(nextHorse);
        currentNode = horses.search(fakeNode1);
    }

    // Reset all visited flags before returning
    currentNode = followerNode;
    shared_ptr<TreeNode<horse>> fakeNode = make_shared<TreeNode<horse>>(followerNode->data);

    while (currentNode->data->get_id() != fakeNode->data->get_id()) {
        currentNode->data->isVisited = false;
        shared_ptr<horse>nextHorse = currentNode->data->get_follow();
        if (!nextHorse) { break; }
        fakeNode = make_shared<TreeNode<horse>>(nextHorse);
        currentNode = horses.search(fakeNode);
    }

    return foundLeader;
}

StatusType Plains::follow(int horseId, int horseToFollowId)
{
    if(horseId <= 0 || horseToFollowId <= 0 || horseId == horseToFollowId){
        return StatusType::INVALID_INPUT;
    }
    shared_ptr<TreeNode<horse>> follower = horses.search(make_shared<TreeNode<horse>>(make_shared<horse>(horseId, 0)));
    shared_ptr<TreeNode<horse>> toFollow = horses.search(make_shared<TreeNode<horse>>(make_shared<horse>(horseToFollowId, 0)));
    if(follower->data->get_herd_id() != toFollow->data->get_herd_id()){
        return StatusType::FAILURE;
    }
    follower->data->set_follow(toFollow->data);
    follower->data->set_versionfollow(toFollow->data->get_version());
    return StatusType::SUCCESS;
}

output_t<int> Plains::get_speed(int horseId) {
    if (horseId <= 0) {
        return StatusType::INVALID_INPUT; // INVALID_INPUT for invalid IDs
    }

    shared_ptr<TreeNode<horse>> horseNode = horses.search(horse::make_horse_node(horseId));

    if (!horseNode || horseNode->data->get_horse_id() != horseId) {
        return StatusType::FAILURE; // Horse not found
    }

    return horseNode->data->get_speed();
}


output_t<bool> Plains::can_run_together(int herdId)
{
    //TODO: implement inorder traversal
    return false;
}