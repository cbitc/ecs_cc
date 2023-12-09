#pragma once

#include<iostream>
#include<vector>
#include<type_traits>
#include<cassert>

#define assertm(msg,condition) assert((msg),(condition))

namespace CDataStruct
{
    template<typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
    class SparseSet
    {
    public:
        using value_t = T;
    private:
        std::vector<value_t> packed_;
        std::vector<value_t> sparse_;
    private:
        
    public:
        inline static constexpr value_t null = std::numeric_limits<value_t>::max();
        SparseSet() {}
        
        void insert(value_t value) {
            value_t index = packed_.size();
            packed_.push_back(value);
            if (value >= sparse_.size()) { sparse_.resize(value + 1, null); }
            sparse_[value] = index;
        }

        bool exist(value_t target) const {
            return target < sparse_.size() && sparse_[target] != null;
        }

        value_t erase(value_t target) {
            if (!exist(target))return null;
            value_t index = sparse_[target];
            std::swap(packed_[index], packed_.back()), packed_.pop_back();
            sparse_[target] = null;
            sparse_[packed_[index]] = index;
            return index;
        }

        void clear() {
            packed_.clear(), sparse_.clear();
        }

        void print() {
            for (value_t val : packed_)printf("%d ", val);
            printf("\n");
            for (value_t index : sparse_)printf("%d ", index);
            printf("\n");
        }
    };
}



namespace ECS
{
    using entity_size = uint32_t;
    using entity_t = entity_size;

    

}