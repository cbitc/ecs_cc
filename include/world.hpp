#pragma once

#include"public.hpp"
#include"entity.hpp"
#include"dataStruct.hpp"
#include"component.hpp"
#include"resource.hpp"
#include<tuple>





namespace ECS
{

/**
 * @brief 
 * 管理实体和组件之间关系的容器
 */
class World
{
    friend class Registry;


    
    /**
     * @brief 
     * 负责存放单个实体关系到的所有组件
     */
    class ComponentContainer final
    {
    public:


        
        /**
         * @brief 
         * 向组件集合里添加一个组件
         * @param info 
         */
        void insert(const ComponentInfo& info) noexcept {
            componentInfos_.push_back(info);
        }



        /**
         * @brief
         * 根据传入的组件ID,和组件数据向在集合中就地构造一个组件信息
         * @param[in] id 组件ID
         * @param[in] data 组件的指针
         */
        void insert(componentID_t id,void* data) noexcept {
            componentInfos_.emplace_back(id,data);
        }



        /**
         * @brief 
         * 根据传入的组件ID,判断某一组件是否在集合里
         * @param[in] id 组件ID
         * @return true 存在
         * @return false 不存在
         */
        bool contain(componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return true;
            }
            return false;
        }



        /**
         * @brief 
         * 根据传入的组件ID,从集合里移除某个组件
         * @param[in] id 组件ID
         * @return true 表示移除成功;
         * @return false 移除失败;
         * @note 
         * 若该组件不在集合里,该方法不做任何事,返回值为false;
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
         * @brief 
         * 根据传入的组件ID,返回组件信息的引用
         * @param[in] id 组件ID
         * @retval ComponentInfo&
         * @return 如果组件存在则返回对应的Info,否则返回值未定义
         * @warning
         * 使用该方法前要确保组件在集合中,否则将产生未定义行为
         */
        ComponentInfo& operator[](componentID_t id) noexcept {
            for (ComponentInfo& info : componentInfos_) {
                if (info.id == id)return info;
            }
            assertm("not found!",false);

            return componentInfos_.back();
        }



        /**
         * @brief 
         * 根据传入的组件ID,返回组件信息的常引用
         * @param[in] id 组件ID
         * @retval const ComponentInfo&
         * @return 如果组件存在则返回对应的Info,否则返回值未定义
         * @warning
         * 使用该方法前要确保组件在集合中,否则将产生未定义行为
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
         * 获取组件集合的起始迭代器
         * @retval std::vector<ComponentInfo>::iterator
         * @return 组件集合的起始迭代器
         */
        std::vector<ComponentInfo>::iterator begin() noexcept {
            return componentInfos_.begin();
        }



        /**
         * @brief 
         * 获取组件集合的const起始迭代器
         * @retval std::vector<ComponentInfo>::const_iterator
         * @return 组件集合的const起始迭代器
         */
        std::vector<ComponentInfo>::const_iterator begin() const noexcept {
            return componentInfos_.begin();
        }



        /**
         * @brief 
         * 获取组件集合的终止迭代器
         * @retval std::vector<ComponentInfo>::iterator
         * @return 组件集合的终止迭代器
         */
        std::vector<ComponentInfo>::iterator end() noexcept {
            return componentInfos_.end();
        }



        /**
         * @brief 
         * 获取组件集合的const终止迭代器
         * @retval std::vector<ComponentInfo>::const_iterator
         * @return 组件集合的const终止迭代器
         */
        std::vector<ComponentInfo>::const_iterator end() const noexcept {
            return componentInfos_.end();
        }



    private:
        std::vector<ComponentInfo> componentInfos_; 
    };




    
    /**
     * @brief 
     * 某一类型的组件与所有拥有该组件的实体的对应关系的集合
     */
    class ComponentMap final
    {

    public:


        
        /**
         * @brief 
         * 根据传入的组件ID,获取所有拥有该类型组件的所有实体的集合
         * @param[in] id 组件ID
         * @retval CDataStruct::SparseSet<Entity>&
         * @return 所有拥有该类型组件的所有实体的集合
         * @note
         * 如果该类型的组件不在容器内,则方法将创建该类型组件与实体的对应关系,并将其返回
         */
        CDataStruct::SparseSet<Entity>& operator[](componentID_t id) noexcept {
            if (id >= map_.size()) {
                map_.resize(id + 1);
            }
            assertm("the component is not in map",contain(id));
            return map_[id];
        }


        
        /**
         * @brief 
         * 根据传入的组件ID,判断该类型组件是否在容器内
         * @param[in] id 组件ID 
         * @return true 表示存在;
         * @return false 表示不存在;
         */
        bool contain(componentID_t id) const noexcept {
            return id < map_.size();
        }


        
        /**
         * @brief 
         * 获取容器的起始迭代器
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::iterator
         * @return 容器的起始迭代器
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator begin() noexcept {
            return map_.begin();
        }



        /**
         * @brief 
         * 获取容器的const起始迭代器
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::const_iterator
         * @return 容器的const起始迭代器
         */
        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator begin() const noexcept {
            return map_.begin();
        }




        /**
         * @brief 
         * 获取容器的终止迭代器
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::iterator
         * @return 容器的终止迭代器
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator end() noexcept {
            return map_.end();
        }




        /**
         * @brief 
         * 获取容器的const终止迭代器
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::const_iterator
         * @return 容器的const终止迭代器
         */
        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator end() const noexcept {
            return map_.end();
        }


        
    private:
        /// @brief 存储组件到实体集合映射关系的数据结构
        std::vector<CDataStruct::SparseSet<Entity>> map_;
    };





    class ResourceContainer final
    {

    public:
        
        void insert(resourceID_t id,void* data) noexcept {
            if (id >= resources_.size()) {
                resources_.resize(id + 1,nullptr);
            }
            resources_[id] = data;
        }


        
        bool contain(resourceID_t id) const noexcept {
            return id < resources_.size() && resources_[id];
        }


        
        bool remove(resourceID_t id) noexcept {
            if (!contain(id)) {
                return false;
            }
            resources_[id] = nullptr;
        }


        
        void* operator[](resourceID_t id) noexcept {
            return resources_[id];
        }

    private:
        std::vector<void*> resources_;
    };

    
public:

    /// @brief 表示实体到组件集合映射的组件集合的类型
    using componentSet_t = ComponentContainer;

    /// @brief 表示组件到实体集合映射的实体集合类型
    using entitySet_t = CDataStruct::SparseSet<Entity>;

    /// @brief 表示实体到组件集合映射的类型
    using entityMap_t = CDataStruct::DenseMap<Entity,componentSet_t>;

    /// @brief 表示组件到实体集合映射的类型
    using componentMap_t = ComponentMap;

    /// @brief 存储资源的集合
    using resourceSet_t = ResourceContainer;

private:
    
    /// @brief 实体到组件集合映射的容器
    entityMap_t entityMap_;

    /// @brief 组件到实体集合映射的容器
    componentMap_t componentMap_;

    /// @brief 资源容器
    resourceSet_t resources_;

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







/**
 * @brief 
 * world的所有操作均通过该类进行
 */
class Registry final
{

    /**
     * @brief 
     * 产生一个实体ID
     * @retval Entity
     * @return 实体ID
     */
    Entity _get_entity() const {
        return EntityManager::spawn();
    }


    
public:


    
    /**
     * @brief 
     * 根据传入的world构造一个依赖于该world的Registry
     * @param[in] world 
     */
    Registry(World& world) :world_(world) {}



    /**
     * @brief 
     * 根据传入的组件类型参数,返回一个包含所有组件的实体集合
     * @tparam T 当前的组件类型
     * @tparam Ts 剩余的组件类型
     * @retval std::vector<Entity>
     * @return 包含所有组件的实体集合
     */
    template<typename T,typename...Ts>
    std::vector<Entity> view() noexcept {
        componentID_t id = ComponentManager::componentID<T>();
        const World::entitySet_t& entitySet = world_.componentMap_[id];
        std::vector<Entity> entitys;
        for (Entity entity : entitySet) { entitys.push_back(entity); }
        if constexpr (sizeof...(Ts) != 0) {
            doView<Ts...>(entitys);
        }
        return entitys;
    }


    
    /**
     * @brief 
     * 给传入的实体构造类型为T的组件
     * @tparam T 构造的组件类型
     * @tparam Args 用于构造该组件的参数类型
     * @param entity 目标实体
     * @param args 用于构造该组件的参数
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
     * @brief 
     * 
     * @tparam T 
     * @tparam Args 
     * @param entity 
     * @param args 
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



    template<typename T>
    T* getResource() noexcept {
        resourceID_t id = ResourceManager::resourceID<T>();
        T* res = nullptr;
        if (world_.resources_.contain(id)) {
            res = static_cast<T*>(world_.resources_[id]);
        }
        return res;
    }



    template<typename T,typename...Args>
    auto createResource(Args&&...args) noexcept
        -> decltype(new T{ std::forward<Args>(args)... }) {
        resourceID_t id = ResourceManager::resourceID<T>();
        if (world_.resources_.contain(id)) {
            return static_cast<T*>(world_.resources_[id]);
        }
        T* res = ResourceManager::instance<T>(std::forward<Args>(args)...);
        world_.resources_.insert(id,res);
        return res;
    }



    template<typename T>
    bool hasResource() const noexcept {
        resourceID_t id = ResourceManager::resourceID<T>();
        return world_.resources_.contain(id);
    }


    
    template<typename T>
    void destoryResource() noexcept {
        resourceID_t id = ResourceManager::resourceID<T>();
        ResourceManager::destory<T>(world_.resources_[id]);
        world_.resources_.remove(id);
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