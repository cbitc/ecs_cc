#pragma once

#include<vector>

namespace ECS
{

/// @brief 组件ID的类型
using componentID_t = uint8_t;





/**
 * @brief
 * 包含组件的ID和数据的结构体.
 * 用于描述一个组件所需的最小信息
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
 * 组件池,所有的组件必须通过这个类进行创建和销毁
 * @tparam T 组件类型
 *
 * @note
 * 该类内部维护一个组件的缓冲池,避免对new和delete的大量调用
 *
 */
template<typename T>
class ComponentPool final
{

public:

    /**
     * @brief
     * 通过传入的参数实例化一个组件
     *
     * @tparam Args 参数包,展开后是各个传入的参数类型
     * @param[in] args 传入的参数
     * @return decltype(new T{ std::forward<Args>(args)... })
     * @note
     *该方法实例化组件时会先检查缓冲池中是否还存有实体
     *如果有则直接将实体指针取出并返回,否则调用new 方法创建实体
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
     * 接收一个组件的指针,执行组件的销毁动作.
     * 该函数并不会真正销毁实体,而是将其存入缓冲池
     * @param[in] ins 要销毁的组件的指针
     */
    static void destory(void* ins) {
        cache_.push_back(static_cast<T*>(ins));
    }


private:
    inline static std::vector<T*> cache_;
};





/**
 * @brief
 * 对组件所有操作的一个集合.
 * 类内的所有属性和方法皆为静态,该类并不会具体实例化
 *
 */
class ComponentManager final
{

    using DestoryFunction = void(*)(void*);
    using DestoryFunctionContainer = std::vector<DestoryFunction>;

public:

    /**
     * @brief 对传入的组件类型T,返回该组件类型的ID
     *
     * @tparam T 传入的组件类型
     * @return componentID_t
     * @retval id 组件的ID
     */
    template<typename T>
    static componentID_t componentID() {
        static const componentID_t id = CurrentComponentID++;
        return id;
    }




    /**
     * @brief
     * 根据传入的参数创建一个组件,并返回该指针
     * @tparam T 组件的类型
     * @tparam Args 参数包,为传入的各个参数类型
     * @param[in] args 传入的各个参数
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
     * 根据传入的信息销毁一个组件
     * @param[in] info 传入的组件信息
     */
    static void destory(ComponentInfo& info) {
        DestoryFunctions[info.id](info.data);
    }




private:

    /**
     * @brief
     * 对类型为T的组件注册一个销毁函数,销毁函数将在组件销毁时被调用
     * @tparam T 组件类型
     * @param func 销毁函数,具体类型为void(*)(void*)
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
     * 判断类型为T的组件是否注册了销毁函数
     * @tparam T 组件类型
     * @return true 已经注册
     * @return false 还未注册
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


