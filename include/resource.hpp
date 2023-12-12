#pragma once

#include"public.hpp"
#include<vector>

namespace ECS
{

using resourceID_t = uint8_t;




struct ResourceInfo final
{
    resourceID_t id;
    void* data;
};




class ResourceManager final
{

    using DestoryFunction = void(*)(void*);

    struct RegistrationInfo final
    {
        DestoryFunction destoryFunc;
    };

public:

    template<typename T>
    static resourceID_t resourceID() {
        static resourceID_t id = CurResourceID_++;
        return id;
    }



    template<typename T,typename...Args>
    static auto instance(Args&&...args)
        -> decltype(new T{ std::forward<Args>(args)... }) {
        resourceID_t id = resourceID<T>();
        if (!hasRegistry(id)) {
            registry(id,
                [](void* pointer) {
                    delete static_cast<T*>(pointer);
                    pointer = nullptr;
                });
        }
        return static_cast<T*>(new T{ std::forward<Args>(args)... });
    }



    template<typename T>
    static void destory(void* pointer) {
        resourceID_t id = resourceID<T>();
        getRegistrationDestoryFunc(id)(pointer);
    }



private:

    static void registry(resourceID_t id,DestoryFunction&& func) {
        if (id >= RegistrationInfos_.size()) {
            RegistrationInfos_.resize(id + 1,{ nullptr });
        }
        assertm("out of range",id < RegistrationInfos_.size());
        RegistrationInfos_[id].destoryFunc = func;
    }



    static bool hasRegistry(resourceID_t id) {
        return id < RegistrationInfos_.size() && RegistrationInfos_[id].destoryFunc;
    }



    static DestoryFunction getRegistrationDestoryFunc(resourceID_t id) {
        assertm("out of range",id < RegistrationInfos_.size());
        return RegistrationInfos_[id].destoryFunc;
    }



private:
    inline static std::vector<RegistrationInfo> RegistrationInfos_;
    inline static resourceID_t CurResourceID_ = 0;
};





}