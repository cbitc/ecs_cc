#pragma once
#include<vector>

namespace ECS
{
    using entity_t = uint32_t;
    using Entity = entity_t;

    class EntityManager final
    {
    public:
        static Entity spawn() {
            if (free_.empty()) {
                return cur_++;
            }
            Entity entity = free_.back();
            free_.pop_back();
            return entity;
        }

        static void destory(Entity entity) {
            free_.push_back(entity);
        }

    private:
        inline static Entity cur_ = 0;
        inline static std::vector<Entity> free_;
    };
}
