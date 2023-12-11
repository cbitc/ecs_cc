#pragma once

#include"public.hpp"
#include"entity.hpp"
#include"dataStruct.hpp"
#include"component.hpp"
#include<tuple>





namespace ECS
{

/**
 * @brief 
 * 
 */
class World
{
    friend class Registry;


    
    /**
     * @brief 
     * 
     */
    class ComponentContainer final
    {
    public:
        
        /**
         * @brief 
         * 
         * @param info 
         */
        void insert(const ComponentInfo& info) noexcept {
            componentInfos_.push_back(info);
        }



        /**
         *@brief 
         * 
         * @param id 
         * @param data 
         */
        void insert(componentID_t id,void* data) noexcept {
            componentInfos_.emplace_back(id,data);
        }



        /**
         *@brief 
         * 
         * @param id 
         * @return true 
         * @return false 
         */
        bool contain(componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return true;
            }
            return false;
        }



        /**
         *@brief 
         * 
         * @param id 
         * @return true 
         * @return false 
         */
        bool remove(componentID_t id) noexcept {
            for (ComponentInfo& info : componentInfos_) {
                if (info.id == id) {
                    std::swap(info,componentInfos_.back()),componentInfos_.pop_back();
                    return true;
                }
            }
            return false;
        }



        /**
         *@brief 
         * 
         * @param id 
         * @return ComponentInfo& 
         */
        ComponentInfo& operator[](componentID_t id) noexcept {
            for (ComponentInfo& info : componentInfos_) {
                if (info.id == id)return info;
            }
            assertm("not found!",false);

            return componentInfos_.back();
        }



        /**
         *@brief 
         * 
         * @param id 
         * @return const ComponentInfo& 
         */
        const ComponentInfo& operator[](componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return info;
            }
            assertm("not found!",false);

            return componentInfos_.back();
        }



        /**
         * @brief 
         * 
         * @return std::vector<ComponentInfo>::iterator 
         */
        std::vector<ComponentInfo>::iterator begin() noexcept {
            return componentInfos_.begin();
        }



        /**
         *@brief 
         * 
         * @return std::vector<ComponentInfo>::const_iterator 
         */
        std::vector<ComponentInfo>::const_iterator begin() const noexcept {
            return componentInfos_.begin();
        }



        /**
         *@brief 
         * 
         * @return std::vector<ComponentInfo>::iterator 
         */
        std::vector<ComponentInfo>::iterator end() noexcept {
            return componentInfos_.end();
        }



        /**
         *@brief 
         * 
         * @return std::vector<ComponentInfo>::const_iterator 
         */
        std::vector<ComponentInfo>::const_iterator end() const noexcept {
            return componentInfos_.end();
        }



    private:
        std::vector<ComponentInfo> componentInfos_;
    };




    
    /**
     *@brief 
     * 
     */
    class ComponentMap final
    {

    public:


        
        /**
         *@brief 
         * 
         * @param id 
         * @return CDataStruct::SparseSet<Entity>& 
         */
        CDataStruct::SparseSet<Entity>& operator[](componentID_t id) noexcept {
            if (id >= map_.size()) {
                map_.resize(id + 1);
            }
            assertm("the component is not in map",contain(id));
            return map_[id];
        }


        
        /**
         *@brief 
         * 
         * @param id 
         * @return true 
         * @return false 
         */
        bool contain(componentID_t id) const noexcept {
            return id < map_.size();
        }


        
        /**
         *@brief 
         * 
         * @return std::vector<CDataStruct::SparseSet<Entity>>::iterator 
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator begin() noexcept {
            return map_.begin();
        }



        /**
         *@brief 
         * 
         * @return std::vector<CDataStruct::SparseSet<Entity>>::const_iterator 
         */
        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator begin() const noexcept {
            return map_.begin();
        }




        /**
         *@brief 
         * 
         * @return std::vector<CDataStruct::SparseSet<Entity>>::iterator 
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator end() noexcept {
            return map_.end();
        }




        /**
         *@brief 
         * 
         * @return std::vector<CDataStruct::SparseSet<Entity>>::const_iterator 
         */
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

    /**
     *@brief 
     * 
     * @return Entity 
     */
    Entity _get_entity() const {
        return EntityManager::spawn();
    }


    
public:

    /**
     *@brief Construct a new Registry object
     * 
     * @param world 
     */
    Registry(World& world) :world_(world) {}



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @return std::vector<Entity> 
     */
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


    
    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @param args 
     * @return std::void_t<decltype(new T{ std::forward<Args>(args)... }) > 
     */
    template<typename T,typename...Args>
    auto emplace(Entity entity,Args&&...args) noexcept ->
        std::void_t<decltype(new T{ std::forward<Args>(args)... }) > {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        components.insert(id,ComponentManager::create<T>(std::forward<Args>(args)...));
        world_.componentMap_[id].insert(entity);
    }



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @param args 
     * @return std::void_t<decltype(new T{ std::forward<Args>(args)... }) > 
     */
    template<typename T,typename...Args>
    auto replace(Entity entity,Args&&...args) noexcept ->
        std::void_t<decltype(new T{ std::forward<Args>(args)... }) > {
        componentID_t id = ComponentManager::componentID<T>();
        World::componentSet_t& components = world_.entityMap_[entity];
        *(static_cast<T*>(components[id].data)) = { std::forward<Args>(args)... };
    }



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @param args 
     * @return std::void_t<decltype(new T{ std::forward<Args>(args)... }) > 
     */
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



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @return true 
     * @return false 
     */
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



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @return true 
     * @return false 
     */
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



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     */
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



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Ts 
     * @param entity 
     * @return auto 
     */
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



    /**
     *@brief 
     * 
     * @tparam T 
     * @tparam Ts 
     * @param entity 
     * @return auto 
     */
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



    /**
     *@brief Get the or emplace object
     * 
     * @tparam T 
     * @tparam Ts 
     * @param entity 
     * @return std::conditional_t < sizeof...(Ts) == 0,
     * decltype(new T{}),std::tuple<T*,std::add_pointer_t<Ts>... >> 
     */
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



    /**
     *@brief 
     * 
     * @return Entity 
     */
    Entity create() noexcept {
        Entity entity = _get_entity();
        world_.entityMap_.insert(entity);
        return entity;
    }



    /**
     *@brief 
     * 
     * @param entity 
     */
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


    
    /**
     * @brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entitys 
     */
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