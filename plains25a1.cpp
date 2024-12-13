// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains():horses(), herds(), empty_herds(){}

Plains::~Plains()
{

}

StatusType Plains::add_herd(int herdId)
{
    return empty_herds.insert(herd::make_herd_node(herdId));
}

StatusType Plains::remove_herd(int herdId)
{
    return StatusType::FAILURE;
}

StatusType Plains::add_horse(int horseId, int speed)
{
    shared_ptr<horse> horseToInsert = make_shared<horse>(horseId, speed);
    shared_ptr<TreeNode<horse>> NodeToInsert = make_shared<TreeNode<horse>>(horseToInsert);
    return horses.insert(NodeToInsert);//add node maker?
}

StatusType Plains::join_herd(int horseId, int herdId)
{
    return StatusType::FAILURE;
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

StatusType Plains::leave_herd(int horseId)
{
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
