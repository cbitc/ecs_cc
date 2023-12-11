#pragma once

#include<vector>

namespace ECS
{

/// @brief ���ID������
using componentID_t = uint8_t;





/**
 * @brief
 * ���������ID�����ݵĽṹ��.
 * ��������һ������������С��Ϣ
 */
struct ComponentInfo
{
    componentID_t id;
    void* data;
    ComponentInfo(componentID_t id_,void* data_) :id(id_),data(data_) {}
    ComponentInfo() = default;
};





/**
 * @brief
 * �����,���е��������ͨ���������д���������
 * @tparam T �������
 *
 * @note
 * �����ڲ�ά��һ������Ļ����,�����new��delete�Ĵ�������
 *
 */
template<typename T>
class ComponentPool final
{

public:

    /**
     * @brief
     * ͨ������Ĳ���ʵ����һ�����
     *
     * @tparam Args ������,չ�����Ǹ�������Ĳ�������
     * @param[in] args ����Ĳ���
     * @return decltype(new T{ std::forward<Args>(args)... })
     * @note
     *�÷���ʵ�������ʱ���ȼ�黺������Ƿ񻹴���ʵ��
     *�������ֱ�ӽ�ʵ��ָ��ȡ��������,�������new ��������ʵ��
     */
    template<typename...Args>
    static auto instance(Args&&...args) -> decltype(new T{ std::forward<Args>(args)... }) {
        if (cache_.empty()) {
            cache_.push_back(new T{ std::forward<Args>(args)... });
        }
        T* result = cache_.back();
        cache_.pop_back();
        return result;
    }




    /**
     * @brief
     * ����һ�������ָ��,ִ����������ٶ���.
     * �ú�����������������ʵ��,���ǽ�����뻺���
     * @param[in] ins Ҫ���ٵ������ָ��
     */
    static void destory(void* ins) {
        cache_.push_back(static_cast<T*>(ins));
    }


private:
    inline static std::vector<T*> cache_;
};





/**
 * @brief
 * ��������в�����һ������.
 * ���ڵ��������Ժͷ�����Ϊ��̬,���ಢ�������ʵ����
 *
 */
class ComponentManager final
{

    using DestoryFunction = void(*)(void*);
    using DestoryFunctionContainer = std::vector<DestoryFunction>;

public:

    /**
     * @brief �Դ�����������T,���ظ�������͵�ID
     *
     * @tparam T ������������
     * @return componentID_t
     * @retval id �����ID
     */
    template<typename T>
    static componentID_t componentID() {
        static const componentID_t id = CurrentComponentID++;
        return id;
    }




    /**
     * @brief
     * ���ݴ���Ĳ�������һ�����,�����ظ�ָ��
     * @tparam T ���������
     * @tparam Args ������,Ϊ����ĸ�����������
     * @param[in] args ����ĸ�������
     * @return T*
     */
    template<typename T,typename...Args>
    static T* create(Args&&...args) {
        if (!hasRegistryDestory<T>()) {
            registryDestory<T>(&ComponentPool<T>::destory);
        }
        return ComponentPool<T>::instance(std::forward<Args>(args)...);
    }




    /**
     * @brief
     * ���ݴ������Ϣ����һ�����
     * @param[in] info ����������Ϣ
     */
    static void destory(ComponentInfo& info) {
        DestoryFunctions[info.id](info.data);
    }




private:

    /**
     * @brief
     * ������ΪT�����ע��һ�����ٺ���,���ٺ��������������ʱ������
     * @tparam T �������
     * @param func ���ٺ���,��������Ϊvoid(*)(void*)
     */
    template<typename T>
    static void registryDestory(DestoryFunction&& func) {
        componentID_t id = componentID<T>();
        if (id >= DestoryFunctions.size()) {
            DestoryFunctions.resize(id + 1,nullptr);
        }
        DestoryFunctions[id] = func;
    }




    /**
     * @brief
     * �ж�����ΪT������Ƿ�ע�������ٺ���
     * @tparam T �������
     * @return true �Ѿ�ע��
     * @return false ��δע��
     */
    template<typename T>
    static bool hasRegistryDestory() {
        componentID_t id = componentID<T>();
        return id < DestoryFunctions.size() && DestoryFunctions[id] != nullptr;
    }



private:
    inline static componentID_t CurrentComponentID = 0;
    inline static DestoryFunctionContainer DestoryFunctions;
};




}


