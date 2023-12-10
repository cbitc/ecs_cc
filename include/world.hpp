#pragma once

#include"public.hpp"
#include"entity.hpp"
#include"dataStruct.hpp"
#include"component.hpp"
#include<tuple>


namespace ECS
{

    class World
    {
        friend class Registry;
        class ComponentContainer;
        using ComponentSet = ComponentContainer;
        using EntityContainer = CDataStruct::DensMap<Entity, ComponentSet>;

        class ComponentContainer final
        {

        public:
            void insert(const ComponentInfo& info) {
                componentInfos_.push_back(info);
            }

            void insert(componentID_t id, void* data) {
                componentInfos_.emplace_back(id,data);
            }

            void erase(componentID_t id) {
                std::swap(componentInfos_[id],componentInfos_.back());
                componentInfos_.pop_back();
            }

            bool container(componentID_t id)const {
                for (const ComponentInfo& info : componentInfos_) {
                    if (info.id == id)return true;
                }
                return false;
            }

            bool remove(componentID_t id) {
                for (ComponentInfo& info : componentInfos_) {
                    if (info.id == id) {
                        std::swap(info, componentInfos_.back()), componentInfos_.pop_back();
                        return true;
                    }
                }
                return false;
            }
   

            ComponentInfo& operator[](componentID_t id) {
                for (ComponentInfo& info : componentInfos_) {
                    if (info.id == id)return info;
                }
                componentInfos_.emplace_back(id, nullptr);
                return componentInfos_.back();
            }

            

            std::vector<ComponentInfo>::iterator begin() {
                return componentInfos_.begin();
            }

            std::vector<ComponentInfo>::const_iterator begin() const {
                return componentInfos_.begin();
            }

            std::vector<ComponentInfo>::iterator end() {
                return componentInfos_.end();
            }

            std::vector<ComponentInfo>::const_iterator end() const {
                return componentInfos_.end();
            }

        private:
            std::vector<ComponentInfo> componentInfos_;
        };

        class ComponentsMap final
        {
        public:
            using EntitySet = CDataStruct::SparseSet<Entity>;
        public:
            
            EntitySet& operator[](componentID_t id) {
                if (id >= map_.size()) {
                    map_.resize(id + 1);
                }
                return map_[id];
            }
            
            std::vector<EntitySet>::iterator begin() {
                return map_.begin();
            }

            std::vector<EntitySet>::const_iterator begin()const {
                return map_.begin();
            }

            
            std::vector<EntitySet>::iterator end() {
                return map_.end();
            }

            
            std::vector<EntitySet>::const_iterator end()const {
                return map_.end();
            }
        private:
            std::vector<EntitySet> map_;
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
            auto id = ComponentManager::componentID<T>();
            auto& components = world_.entitys_[entity];
            components.insert(id,ComponentManager::create<T>(std::forward<Args>(args)...));
            world_.componentsMap_[id].insert(entity);
        }

        template<typename...Args>
        void replace(Entity entity, Args&&...args) {

        }

        template<typename...Args>
        void emplace_or_replace(Entity entity, Args&&...args) {

        }

        template<typename T,typename...Args>
        bool all_of(Entity entity) {
            componentID_t id = ComponentManager::componentID<T>();
            bool is = world_.entitys_[entity].container(id);
            if constexpr (sizeof...(Args) == 0) {
                return is;
            } else {
                return is && all_of<Args...>(entity);
            }

        }

        template<typename T,typename...Args>
        bool any_of(Entity entity) {
            componentID_t id = ComponentManager::componentID<T>();
            bool is = world_.entitys_[entity].container(id);
            if constexpr (sizeof...(Args) == 0) {
                return is;
            } else {
                return is || all_of<Args...>(entity);
            }
        }

        template<typename T,typename...Args>
        void erase(Entity entity) {
            componentID_t id = ComponentManager::componentID<T>();
            world_.entitys_[entity].erase(id);
            world_.componentsMap_[id].erase(entity);
            if constexpr (sizeof...(Args) != 0) {
                erase<Args...>(entity);
            }
            return;
        }

        template<typename T,typename...Args>
        void remove(Entity entity) {
            componentID_t id = ComponentManager::componentID<T>();
            if (bool is = world_.entitys_[entity].remove(id);is) {
                world_.componentsMap_[id].erase(entity);
            }
            if constexpr (sizeof...(Args) != 0) {
                remove<Args...>(entity);
            }
            return;
        }

        template<typename T,typename...Args>
        auto get(Entity entity) {
            componentID_t id = ComponentManager::componentID<T>();
            T& data = *(static_cast<T*>(world_.entitys_[entity][id].data));
            if constexpr (sizeof...(Args) == 0) {
                return std::tie(data);
            } else {
                return std::tuple_cat(
                    std::tie(data),
                    get<Args...>(entity)
                );
            }
        }

        template<typename T,typename...Args>
        auto try_get(Entity entity) {
            
        }


        Entity create() {
            Entity entity = _get_entity();
            world_.entitys_.insert(entity);
            return entity;
        }

        void destory(Entity entity) {
            auto& components = world_.entitys_[entity];
            for (ComponentInfo& info : components) {
                world_.componentsMap_[info.id].erase(entity);
                ComponentManager::destory(info);
            }
            world_.entitys_.erase(entity);
            EntityManager::destory(entity);
        }
    private:
        


    private:
        World& world_;
    };
}
