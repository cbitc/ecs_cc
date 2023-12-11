#pragma once
#include<vector>

namespace ECS
{
using entity_t = uint32_t;
using Entity = entity_t;



/**
 *@brief 
 * 
 */
class EntityManager final
{

public:



    /**
     *@brief 
     * 
     * @return Entity 
     */
    static Entity spawn() {
        if (free_.empty()) {
            return cur_++;
        }
        Entity entity = free_.back();
        free_.pop_back();
        return entity;
    }



    /**
     *@brief 
     * 
     * @param entity 
     */
    static void destory(Entity entity) {
        free_.push_back(entity);
    }


    
private:
    inline static Entity cur_ = 0;
    inline static std::vector<Entity> free_;
};





}
