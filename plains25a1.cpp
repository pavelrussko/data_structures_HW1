// You can edit anything you want in this file.
// However you need to implement all public Plains function, as provided below as a template

#include "plains25a1.h"


Plains::Plains()
{

}

Plains::~Plains() = default;

StatusType Plains::add_herd(int herdId)
{
    return StatusType::FAILURE;
}

StatusType Plains::remove_herd(int herdId)
{
    if(herdId <= 0 ) {
        return StatusType::INVALID_INPUT;
    } else {
        herd *herd = new herd(herdId);
        empty_herds.search(herd) = herd;
        if(herd == nullptr) {
            return StatusType::FAILURE;
        } else {
            try {
                empty_herds.removal(herd);
            } catch(...) {
                return StatusType::FAILURE;//check if this is the correct exception
            }
        }
    }
    return StatusType::SUCCESS;
}

StatusType Plains::add_horse(int horseId, int speed)
{
    return StatusType::FAILURE;
}

StatusType Plains::join_herd(int horseId, int herdId)
{
    return StatusType::FAILURE;
}

StatusType Plains::follow(int horseId, int horseToFollowId)
{
    return StatusType::FAILURE;
}

StatusType Plains::leave_herd(int horseId)
{
    return StatusType::FAILURE;
}

output_t<int> Plains::get_speed(int horseId)
{
    return 0;
}

output_t<bool> Plains::leads(int horseId, int otherHorseId)
{
    return false;
}

output_t<bool> Plains::can_run_together(int herdId)
{
    return false;
}
