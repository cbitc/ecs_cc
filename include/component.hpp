#pragma once

#include<vector>

namespace ECS
{
    template<typename T>
    class ComponentPool final
    {
    public:
        template<typename...Args>
        static auto instance(Args&&...args) -> decltype(new T{ std::forward<Args>(args)... }) {
            if (cache_.empty()) {
                cache_.push_back(new T{ std::forward<Args>(args)... });
            }
            T* result = cache_.back();
            cache_.pop_back();
            return result;
        }

        static void destory(void* ins) {
            cache_.push_back(static_cast<T*>(ins));
        }

    private:
        inline static std::vector<T*> cache_;
    };


    class ComponentManager final
    {
        using DestoryFunction = void(*)(void*);
        using DestoryFunctionContainer = std::vector<DestoryFunction>;
    public:
        using componentID_t = uint8_t;

        struct ComponentInfo
        {
            componentID_t id;
            void* data;
        };
    public:

        template<typename T>
        static componentID_t componentID() {
            static const componentID_t id = CurrentComponentID++;
            return id;
        }


        template<typename T, typename...Args>
        static ComponentInfo create(Args&&...args) {
            if (!hasRegistryDestory<T>()) {
                registryDestory<T>(&ComponentPool<T>::destory);
            }
            return ComponentInfo{ componentID<T>(),
            ComponentPool<T>::instance(std::forward<Args>(args)...) };
        }

        static void destory(ComponentInfo& info) {
            DestoryFunctions[info.id](info.data);
        }

    private:
        template<typename T>
        static void registryDestory(DestoryFunction&& func) {
            componentID_t id = componentID<T>();
            if (id >= DestoryFunctions.size()) {
                DestoryFunctions.resize(id + 1, nullptr);
            }
            DestoryFunctions[id] = func;
        }

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


