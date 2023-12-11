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






    class ComponentContainer final
    {
    public:

        void insert(const ComponentInfo& info) noexcept {
            componentInfos_.push_back(info);
        }





        void insert(componentID_t id,void* data) noexcept {
            componentInfos_.emplace_back(id,data);
        }





        bool contain(componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return true;
            }
            return false;
        }





        bool remove(componentID_t id) noexcept {
            for (ComponentInfo& info : componentInfos_) {
                if (info.id == id) {
                    std::swap(info,componentInfos_.back()),componentInfos_.pop_back();
                    return true;
                }
            }
            return false;
        }





        ComponentInfo& operator[](componentID_t id) noexcept {
            for (ComponentInfo& info : componentInfos_) {
                if (info.id == id)return info;
            }
            assertm("not found!",false);

            return componentInfos_.back();
        }





        const ComponentInfo& operator[](componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return info;
            }
            assertm("not found!",false);

            return componentInfos_.back();
        }





        std::vector<ComponentInfo>::iterator begin() noexcept {
            return componentInfos_.begin();
        }





        std::vector<ComponentInfo>::const_iterator begin() const noexcept {
            return componentInfos_.begin();
        }





        std::vector<ComponentInfo>::iterator end() noexcept {
            return componentInfos_.end();
        }





        std::vector<ComponentInfo>::const_iterator end() const noexcept {
            return componentInfos_.end();
        }





    private:
        std::vector<ComponentInfo> componentInfos_;
    };
















    class ComponentMap final
    {

    public:

        CDataStruct::SparseSet<Entity>& operator[](componentID_t id) noexcept {
            if (id >= map_.size()) {
                map_.resize(id + 1);
            }
            assertm("the component is not in map",contain(id));
            return map_[id];
        }





        bool contain(componentID_t id) const noexcept {
            return id < map_.size();
        }





        std::vector<CDataStruct::SparseSet<Entity>>::iterator begin() noexcept {
            return map_.begin();
        }





        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator begin() const noexcept {
            return map_.begin();
        }






        std::vector<CDataStruct::SparseSet<Entity>>::iterator end() noexcept {
            return map_.end();
        }






        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator end() const noexcept {
            return map_.end();
        }





    private:
        std::vector<CDataStruct::SparseSet<Entity>> map_;
    };











public:
    using componentSet_t = ComponentContainer;
    using entitySet_t = CDataStruct::SparseSet<Entity>;
    using entityMap_t = CDataStruct::DenseMap<Entity,componentSet_t>;
    using componentMap_t = ComponentMap;

private:
    entityMap_t entityMap_;
    componentMap_t componentMap_;

public:
    void entitysLog()const {
        for (const auto& [entity,coms] : entityMap_) {
            printf("%d ",entity);
            printf("(");
            for (const auto& info : coms) {
                printf("%d ",info.id);
            }
            printf(")\n");
        }
    }

    void componentsMapLog()const {
        int i = 0;
        for (const auto& set : componentMap_) {
            printf("%d ",i++);
            printf("(");
            for (auto entity : set) {
                printf("%d ",entity);
            }
            printf(")\n");
        }
    }
};








class Registry final
{

    Entity _get_entity() const {
        return EntityManager::spawn();
    }






public:

    Registry(World& world) :world_(world) {}





    template<typename T,typename...Args>
    std::vector<Entity> view() noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        const World::entitySet_t& entitySet = world_.componentMap_[id];
        std::vector<Entity> entitys;
        for (Entity entity : entitySet) { entitys.push_back(entity); }
        if constexpr (sizeof...(Args) != 0) {
            doView<Args...>(entitys);
        }
        return entitys;
    }





    template<typename T,typename...Args>
    auto emplace(Entity entity,Args&&...args) noexcept ->
        std::void_t<decltype(new T{ std::forward<Args>(args)... }) > {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        components.insert(id,ComponentManager::create<T>(std::forward<Args>(args)...));
        world_.componentMap_[id].insert(entity);
    }





    template<typename T,typename...Args>
    auto replace(Entity entity,Args&&...args) noexcept ->
        std::void_t<decltype(new T{ std::forward<Args>(args)... }) > {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        *(static_cast<T*>(components[id].data)) = { std::forward<Args>(args)... };
    }





    template<typename T,typename...Args>
    auto emplace_or_replace(Entity entity,Args&&...args) noexcept ->
    std::void_t<decltype(new T{ std::forward<Args>(args)... }) > {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        if (!components.contain(id)) {
            components.insert(id,ComponentManager::create<T>(std::forward<Args>(args)...));
            world_.componentMap_[id].insert(entity);
        } else {
            *(static_cast<T*>(components[id].data)) = { std::forward<Args>(args)... };
        }
    }





    template<typename T,typename...Args>
    bool all_of(Entity entity) const noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        bool is = world_.entityMap_[entity].contain(id);
        if constexpr (sizeof...(Args) == 0) {
            return is;
        } else {
            return is && all_of<Args...>(entity);
        }
    }





    template<typename T,typename...Args>
    bool any_of(Entity entity) const noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        bool is = world_.entityMap_[entity].contain(id);
        if constexpr (sizeof...(Args) == 0) {
            return is;
        } else {
            return is || all_of<Args...>(entity);
        }
    }





    template<typename T,typename...Args>
    void remove(Entity entity) noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        world_.entityMap_[entity].remove(id);
        world_.componentMap_[id].remove(entity);
        if constexpr (sizeof...(Args) != 0) {
            remove<Args...>(entity);
        }
        return;
    }





    template<typename T,typename...Ts>
    auto get(Entity entity) noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        T* data = static_cast<T*>(world_.entityMap_[entity][id].data);
        if constexpr (sizeof...(Ts) == 0) {
            return data;
        } else {
            return std::make_tuple(data,get<Ts>(entity)...);
        }
    }





    template<typename T,typename...Ts>
    auto try_get(Entity entity) noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        const World::componentSet_t& components = world_.entityMap_[entity];
        T* data = components.contain(id) ? static_cast<T*>(components[id].data) : nullptr;
        if constexpr (sizeof...(Ts) == 0) {
            return data;
        } else {
            return std::make_tuple(data,try_get<Ts>(entity)...);
        }
    }





    template<typename T,typename...Ts>
    std::conditional_t < sizeof...(Ts) == 0,
    decltype(new T{}),std::tuple<T*,std::add_pointer_t<Ts>... >>
    get_or_emplace(Entity entity) noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        T* data = nullptr;
        if (!components.contain(id)) {
            data = ComponentManager::create<T>();
            components.insert(id,data);
            world_.componentMap_[id].insert(entity);
        } else {
            data = static_cast<T*>(components[id].data);
        }
        if constexpr (sizeof...(Ts) == 0) {
            return data;
        } else {
            return std::make_tuple(data,get_or_emplace<Ts>(entity)...);
        }
    }





    Entity create() noexcept {
        Entity entity = _get_entity();
        world_.entityMap_.insert(entity);
        return entity;
    }





    void destory(Entity entity) noexcept {
        World::componentSet_t& components = world_.entityMap_[entity];
        for (ComponentInfo& info : components) {
            world_.componentMap_[info.id].remove(entity);
            ComponentManager::destory(info);
        }
        world_.entityMap_.remove(entity);
        EntityManager::destory(entity);
    }






private:

    template<typename T,typename...Args>
    void doView(std::vector<Entity>& entitys) {
        componentID_t id = ComponentManager::componentID<T>();
        for (size_t i = 0;i < entitys.size();i++) {
            Entity& entity = entitys[i];
            if (!world_.entityMap_[entity].contain(id)) {
                std::swap(entity,entitys.back()),entitys.pop_back();
            }
        }
        if constexpr (sizeof...(Args) != 0) {
            doView<Args...>(entitys);
        }
    }


private:
    World& world_;
};




}