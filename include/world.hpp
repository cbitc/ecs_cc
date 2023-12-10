#pragma once

#include"public.hpp"
#include"entity.hpp"
#include"dataStruct.hpp"
#include"component.hpp"


namespace ECS
{
    class Registry;
    class World
    {
        friend class Registry;
        using EntityContainer = CDataStruct::DensMap<Entity, std::vector<ComponentManager::ComponentInfo>>;

        class ComponentsMap final
        {
            using componentID_t = ComponentManager::componentID_t;
        public:
            void insert(componentID_t id,Entity entity) {
                if (id >= map_.size()) {
                    map_.resize(id + 1);
                }
                map_[id].insert(entity);
            }

            
            void erase(componentID_t id,Entity entity) {
                map_[id].erase(entity);
            }

            auto begin() {
                return map_.begin();
            }

            auto begin()const {
                return map_.begin();
            }

            
            auto end() {
                return map_.end();
            }

            
            auto end()const {
                return map_.end();
            }
        private:
            std::vector<CDataStruct::SparseSet<Entity>> map_;
        };


    private:
        EntityContainer entitys_;
        ComponentsMap componentsMap_;
    public:
        void entitysLog()const {
            for (const auto& [entity,coms] : entitys_) {
                printf("%d ", entity);
                printf("(");
                for (const auto& info : coms) {
                    printf("%d ", info.id);
                }
                printf(")\n");
            }
        }

        void componentsMapLog()const {
            int i = 0;
            for (const auto& set : componentsMap_) {
                printf("%d ", i++);
                printf("(");
                for (auto entity : set) {
                    printf("%d ", entity);
                }
                printf(")\n");
            }
        }
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

        template<typename T,typename...Args>
        void emplace(Entity entity, Args&&...args) {
            auto& componentsInfo = world_.entitys_[entity];
            auto info = ComponentManager::create<T>(std::forward<Args>(args)...);
            world_.componentsMap_.insert(info.id, entity);
            componentsInfo.push_back(info);
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
            auto& componentInfos = world_.entitys_[entity];
            for (auto& info : componentInfos) {
                world_.componentsMap_.erase(info.id,entity);
                ComponentManager::destory(info);
            }
            world_.entitys_.erase(entity);
            EntityManager::destory(entity);
        }



    private:
        World& world_;
    };
}
