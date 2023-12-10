#pragma once

#include"entity.hpp"
#include"DataStruct.hpp"
#include"component.hpp"


namespace ECS
{
    class Registry;
    class World
    {
        friend class Registry;
        using EntityContainer = CDataStruct::DensMap<Entity, std::vector<ComponentManager::ComponentInfo>>;
    private:
        EntityContainer entitys_;
        std::vector<CDataStruct::SparseSet<Entity>> componentMap_;
    };

    class Registry final
    {
        Entity _get_entity() {
            return EntityManager::spawn();
        }


    public:
        Registry(World& world) :world_(world) {}

        template<typename...Args>
        auto view() {

        }

        template<typename...Args>
        void emplace(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        void replace(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        void emplace_or_replace(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        bool all_of(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        bool any_of(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        void erase(Entity entity) {

        }

        template<typename...Args>
        bool remove(Entity entity) {

        }

        template<typename...Args>
        auto get(Entity entity) {
            std::tuple<Args...> tuple;
        }

        template<typename...Args>
        auto try_get(Entity entity) {

        }


        Entity create() {
            Entity entity = _get_entity();
            world_.entitys_.insert(entity);
            return entity;
        }

        void destory(Entity entity) {

        }



    private:
        World& world_;
    };
}
