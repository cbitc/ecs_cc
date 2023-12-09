#pragma once

#include<iostream>
#include<vector>
#include<type_traits>
#include<cassert>

#define assertm(msg,condition) assert((msg),(condition))

namespace CDataStruct
{
    template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
    class SparseSet final
    {
    public:
        using value_t = T;
        
        SparseSet() {
            sparse_.resize(1, null);
        }

        void insert(value_t value) {
            if (value >= pakced_.size()) {
                sparse_.resize(value << 1, null);
            }
            sparse_[value] = pakced_.size();
            pakced_.push_back(value);
        }

        bool contain(value_t value) const {
            return value < sparse_.size() && sparse_[value] != null;
        }

        bool erase(value_t value) {
            if (!contain(value))return false;
            value_t index = sparse_[value];
            pakced_[index] = pakced_.back();
            pakced_.pop_back();
            sparse_[pakced_[index]] = index;
            sparse_[value] = null;
            return true;
        }

        size_t size()const {
            return pakced_.size();
        }

        bool empty()const {
            return pakced_.empty();
        }

        void clear() {
            sparse_.clear(), pakced_.clear();
        }

        auto begin() {
            return pakced_.begin();
        }

        auto begin()const {
            return pakced_.begin();
        }

        auto end() {
            return pakced_.end();
        }

        auto end() const {
            return pakced_.end();
        }

    private:
        std::vector<value_t> sparse_;
        std::vector<value_t> pakced_;
        inline static constexpr value_t null = std::numeric_limits<value_t>::max();
    };




    template<typename Key_T, typename Value_T, typename = std::enable_if_t<std::is_integral_v<Key_T>>>
    class DensMap final
    {
        
    public:
        using pack_node = std::pair<Key_T, Value_T>;

        DensMap() {
            sparse_.resize(1, null);
        }

        void insert(Key_T key, const Value_T& value) {
            if (key >= sparse_.size()) {
                sparse_.resize(key << 1, null);
            }
            sparse_[key] = packed_.size();
            packed_.emplace_back(key, value);
        }

        bool contain(Key_T key)const {
            return key < sparse_.size() && sparse_[key] != null;
        }

        bool erase(Key_T key) {
            if (!contain(key))return false;
            Key_T index = sparse_[key];
            packed_[index] = std::move(packed_.back());
            packed_.pop_back();
            sparse_[packed_[index].first] = index;
            sparse_[key] = null;
            return true;
        }

        size_t size() {
            return packed_.size();
        }

        bool empty() {
            return packed_.empty();
        }

        void clear() {
            sparse_.clear(), packed_.clear();
        }

        Value_T& operator[](Key_T key) {
            return packed_[sparse_[key]].second;
        }

        const Value_T& operator[](Key_T key)const {
            return packed_[sparse_[key]].second;
        }

        auto begin() {
            return packed_.begin();
        }

        auto begin()const {
            return packed_.begin();
        }

        auto end() {
            return packed_.end();
        }

        auto end()const {
            return packed_.end();
        }

    private:
        std::vector<Key_T> sparse_;
        std::vector<pack_node> packed_;
        inline static constexpr Key_T null = std::numeric_limits<Key_T>::max();
    };
}



namespace ECS
{
    using entity_t = uint32_t;
    using Entity = entity_t;

    struct ComponentSet
    {
        std::vector<void*> instances_;
        std::vector<void*> cache_;

    public:
        template<typename T, typename...Args>
        T* create(Args&&...args) {
            if (cache_.empty()) {
                instances_.emplace_back(args...);
            } else {

            }
            return static_cast<T*>(instances_.back());
        }
    };

    class EntityContainer final
    {
    public:
    private:
        CDataStruct::SparseSet<Entity> entitys_;
        

    };


    class World
    {
        using Entity_Container = CDataStruct::SparseSet<Entity>;
        using Component_Container = ComponentSet;


    private:
        std::vector<Component_Container> components_;
        Entity_Container entitys_;
    public:

    };

}