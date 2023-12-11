#pragma once

#include<type_traits>
#include<vector>


namespace CDataStruct
{
template<typename T,typename = std::enable_if_t<std::is_integral_v<T>>>
class SparseSet final
{
public:
    
    using value_t = T;


    
    SparseSet() noexcept {
        sparse_.resize(1,null);
    }



    

    void insert(value_t value) noexcept {
        if (value >= sparse_.size()) {
            sparse_.resize(value << 1,null);
        }
        sparse_[value] = pakced_.size();
        pakced_.push_back(value);
    }




    
    bool contain(value_t value) const noexcept {
        return value < sparse_.size() && sparse_[value] != null;
    }




    
    value_t& operator[](value_t value) noexcept {
        return pakced_[sparse_[value]];
    }




    
    const value_t& operator[](value_t value) const noexcept {
        return pakced_[sparse_[value]];
    }




    
    bool remove(value_t value) noexcept {
        if (!contain(value))return false;
        value_t index = sparse_[value];
        pakced_[index] = pakced_.back();
        pakced_.pop_back();
        sparse_[pakced_[index]] = index;
        sparse_[value] = null;
        return true;
    }




    
    size_t size() const noexcept {
        return pakced_.size();
    }




    
    bool empty() const noexcept {
        return pakced_.empty();
    }




    
    void clear() noexcept {
        sparse_.clear(),pakced_.clear();
    }




    
    typename std::vector<value_t>::iterator begin() noexcept {
        
        return pakced_.begin();
    }




    
    typename std::vector<value_t>::const_iterator begin() const noexcept {
        return pakced_.begin();
    }




    
    typename std::vector<value_t>::iterator end() noexcept {
        return pakced_.end();
    }




    
    typename std::vector<value_t>::const_iterator end() const noexcept {
        return pakced_.end();
    }







private:
    std::vector<value_t> sparse_;
    std::vector<value_t> pakced_;
    inline static constexpr value_t null = std::numeric_limits<value_t>::max();
};







template<typename Key_T,typename Value_T,typename = std::enable_if_t<std::is_integral_v<Key_T>>>
class DenseMap final
{

public:
    
    using pack_node = std::pair<Key_T,Value_T>;




    
    DenseMap() noexcept {
        sparse_.resize(1,null);
    }




    
    void insert(Key_T key,const Value_T& value) noexcept {
        if (key >= sparse_.size()) {
            sparse_.resize(key << 1,null);
        }
        sparse_[key] = packed_.size();
        packed_.emplace_back(key,value);
    }




    
    void insert(Key_T key) noexcept {
        insert(key,Value_T{});
    }




    
    bool contain(Key_T key) const noexcept {
        return key < sparse_.size() && sparse_[key] != null;
    }




    
    bool remove(Key_T key) {
        if (!contain(key))return false;
        Key_T index = sparse_[key];
        packed_[index] = std::move(packed_.back());
        packed_.pop_back();
        sparse_[packed_[index].first] = index;
        sparse_[key] = null;
        return true;
    }




    
    size_t size() const noexcept {
        return packed_.size();
    }




    
    bool empty() const noexcept {
        return packed_.empty();
    }




    
    void clear() noexcept {
        sparse_.clear(),packed_.clear();
    }




    
    Value_T& operator[](Key_T key) noexcept {
        if (!contain(key))insert(key);
        assertm("out of range! ",contain(key));
        return packed_[sparse_[key]].second;
    }





    typename std::vector<pack_node>::iterator begin() {
        return packed_.begin();
    }




    
    typename std::vector<pack_node>::const_iterator begin() const {
        return packed_.begin();
    }




    
    typename std::vector<pack_node>::iterator end() {
        return packed_.end();
    }




    
    typename std::vector<pack_node>::const_iterator end() const {
        return packed_.end();
    }





    
private:
    std::vector<Key_T> sparse_;
    std::vector<pack_node> packed_;
    inline static constexpr Key_T null = std::numeric_limits<Key_T>::max();
};



}