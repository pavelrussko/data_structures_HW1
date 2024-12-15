// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains():horses(), herds(), empty_herds(){}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId)
{
    return empty_herds.insert(herd::make_herd_node(herdId));
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
    //TODO
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

output_t<int> Plains::get_speed(int horseId)
{
    if(horseId <= 0){
        return StatusType::FAILURE;
    }
    shared_ptr<TreeNode<horse>> horse = horses.search(horse::make_horse_node(horseId));
    if(horse->data->get_horse_id() != horseId){
        return StatusType::FAILURE;
    }
    return horse->data->get_speed();
}

output_t<bool> Plains::leads(int horseId, int otherHorseId)
{
    return false;
}

output_t<bool> Plains::can_run_together(int herdId)
{
    //TODO: implement inorder traversal
    return false;
}
