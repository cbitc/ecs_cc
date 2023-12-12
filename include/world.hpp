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
 * ����ʵ������֮���ϵ������
 */
class World
{
    friend class Registry;


    
    /**
     * @brief 
     * �����ŵ���ʵ���ϵ�����������
     */
    class ComponentContainer final
    {
    public:


        
        /**
         * @brief 
         * ��������������һ�����
         * @param info 
         */
        void insert(const ComponentInfo& info) noexcept {
            componentInfos_.push_back(info);
        }



        /**
         * @brief
         * ���ݴ�������ID,������������ڼ����о͵ع���һ�������Ϣ
         * @param[in] id ���ID
         * @param[in] data �����ָ��
         */
        void insert(componentID_t id,void* data) noexcept {
            componentInfos_.emplace_back(id,data);
        }



        /**
         * @brief 
         * ���ݴ�������ID,�ж�ĳһ����Ƿ��ڼ�����
         * @param[in] id ���ID
         * @return true ����
         * @return false ������
         */
        bool contain(componentID_t id) const noexcept {
            for (const ComponentInfo& info : componentInfos_) {
                if (info.id == id)return true;
            }
            return false;
        }



        /**
         * @brief 
         * ���ݴ�������ID,�Ӽ������Ƴ�ĳ�����
         * @param[in] id ���ID
         * @return true ��ʾ�Ƴ��ɹ�;
         * @return false �Ƴ�ʧ��;
         * @note 
         * ����������ڼ�����,�÷��������κ���,����ֵΪfalse;
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
         * ���ݴ�������ID,���������Ϣ������
         * @param[in] id ���ID
         * @retval ComponentInfo&
         * @return �����������򷵻ض�Ӧ��Info,���򷵻�ֵδ����
         * @warning
         * ʹ�ø÷���ǰҪȷ������ڼ�����,���򽫲���δ������Ϊ
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
         * ���ݴ�������ID,���������Ϣ�ĳ�����
         * @param[in] id ���ID
         * @retval const ComponentInfo&
         * @return �����������򷵻ض�Ӧ��Info,���򷵻�ֵδ����
         * @warning
         * ʹ�ø÷���ǰҪȷ������ڼ�����,���򽫲���δ������Ϊ
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
         * ��ȡ������ϵ���ʼ������
         * @retval std::vector<ComponentInfo>::iterator
         * @return ������ϵ���ʼ������
         */
        std::vector<ComponentInfo>::iterator begin() noexcept {
            return componentInfos_.begin();
        }



        /**
         * @brief 
         * ��ȡ������ϵ�const��ʼ������
         * @retval std::vector<ComponentInfo>::const_iterator
         * @return ������ϵ�const��ʼ������
         */
        std::vector<ComponentInfo>::const_iterator begin() const noexcept {
            return componentInfos_.begin();
        }



        /**
         * @brief 
         * ��ȡ������ϵ���ֹ������
         * @retval std::vector<ComponentInfo>::iterator
         * @return ������ϵ���ֹ������
         */
        std::vector<ComponentInfo>::iterator end() noexcept {
            return componentInfos_.end();
        }



        /**
         * @brief 
         * ��ȡ������ϵ�const��ֹ������
         * @retval std::vector<ComponentInfo>::const_iterator
         * @return ������ϵ�const��ֹ������
         */
        std::vector<ComponentInfo>::const_iterator end() const noexcept {
            return componentInfos_.end();
        }



    private:
        std::vector<ComponentInfo> componentInfos_; 
    };




    
    /**
     * @brief 
     * ĳһ���͵����������ӵ�и������ʵ��Ķ�Ӧ��ϵ�ļ���
     */
    class ComponentMap final
    {

    public:


        
        /**
         * @brief 
         * ���ݴ�������ID,��ȡ����ӵ�и��������������ʵ��ļ���
         * @param[in] id ���ID
         * @retval CDataStruct::SparseSet<Entity>&
         * @return ����ӵ�и��������������ʵ��ļ���
         * @note
         * ��������͵��������������,�򷽷������������������ʵ��Ķ�Ӧ��ϵ,�����䷵��
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
         * ���ݴ�������ID,�жϸ���������Ƿ���������
         * @param[in] id ���ID 
         * @return true ��ʾ����;
         * @return false ��ʾ������;
         */
        bool contain(componentID_t id) const noexcept {
            return id < map_.size();
        }


        
        /**
         * @brief 
         * ��ȡ��������ʼ������
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::iterator
         * @return ��������ʼ������
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator begin() noexcept {
            return map_.begin();
        }



        /**
         * @brief 
         * ��ȡ������const��ʼ������
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::const_iterator
         * @return ������const��ʼ������
         */
        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator begin() const noexcept {
            return map_.begin();
        }




        /**
         * @brief 
         * ��ȡ��������ֹ������
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::iterator
         * @return ��������ֹ������
         */
        std::vector<CDataStruct::SparseSet<Entity>>::iterator end() noexcept {
            return map_.end();
        }




        /**
         * @brief 
         * ��ȡ������const��ֹ������
         * @retval std::vector<CDataStruct::SparseSet<Entity>>::const_iterator
         * @return ������const��ֹ������
         */
        std::vector<CDataStruct::SparseSet<Entity>>::const_iterator end() const noexcept {
            return map_.end();
        }


        
    private:
        /// @brief �洢�����ʵ�弯��ӳ���ϵ�����ݽṹ
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

    /// @brief ��ʾʵ�嵽�������ӳ���������ϵ�����
    using componentSet_t = ComponentContainer;

    /// @brief ��ʾ�����ʵ�弯��ӳ���ʵ�弯������
    using entitySet_t = CDataStruct::SparseSet<Entity>;

    /// @brief ��ʾʵ�嵽�������ӳ�������
    using entityMap_t = CDataStruct::DenseMap<Entity,componentSet_t>;

    /// @brief ��ʾ�����ʵ�弯��ӳ�������
    using componentMap_t = ComponentMap;

    /// @brief �洢��Դ�ļ���
    using resourceSet_t = ResourceContainer;

private:
    
    /// @brief ʵ�嵽�������ӳ�������
    entityMap_t entityMap_;

    /// @brief �����ʵ�弯��ӳ�������
    componentMap_t componentMap_;

    /// @brief ��Դ����
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
 * world�����в�����ͨ���������
 */
class Registry final
{

    /**
     * @brief 
     * ����һ��ʵ��ID
     * @retval Entity
     * @return ʵ��ID
     */
    Entity _get_entity() const {
        return EntityManager::spawn();
    }


    
public:


    
    /**
     * @brief 
     * ���ݴ����world����һ�������ڸ�world��Registry
     * @param[in] world 
     */
    Registry(World& world) :world_(world) {}



    /**
     * @brief 
     * ���ݴ����������Ͳ���,����һ���������������ʵ�弯��
     * @tparam T ��ǰ���������
     * @tparam Ts ʣ����������
     * @retval std::vector<Entity>
     * @return �������������ʵ�弯��
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
     * �������ʵ�幹������ΪT�����
     * @tparam T ������������
     * @tparam Args ���ڹ��������Ĳ�������
     * @param entity Ŀ��ʵ��
     * @param args ���ڹ��������Ĳ���
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