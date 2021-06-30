//
//  ObjectPool.hpp
//  Maze
//
//  Created by Jaraxus on 22/01/2021.
//

#ifndef ObjectPool_h
#define ObjectPool_h

#include <limits>
#include <memory>
#include <utility>
#include <forward_list>

namespace mz {

namespace details {

template <typename T>
class DataBlock;

} // details

template <typename T>
class ObjectHandler;

template <typename T>
class ObjectPool {
    using DataBlock_T = details::DataBlock<T>;
    using DataBlockIterator = typename std::forward_list<DataBlock_T>::iterator;
    
public:
    ObjectPool();
    
    ObjectPool(double releaseThreshold, std::size_t maxAllocationsCount = std::numeric_limits<std::size_t>::max());
    
    ~ObjectPool() = default;
    
    ObjectPool(ObjectPool const& pool) = delete;
    
    ObjectPool(ObjectPool && pool) = delete;
    
    ObjectPool& operator=(ObjectPool const& pool) = delete;
    
    ObjectPool& operator=(ObjectPool && pool) = delete;
    
    template <typename... Args>
    ObjectHandler<T> New(Args && ...args);
    
    inline std::size_t getObjectsCount() const noexcept;

private:
    inline DataBlockIterator pushDataBlock();
    
private:
    DataBlockIterator               m_currentDataBlock;
    std::forward_list<DataBlock_T>  m_dataBlocks;
    std::size_t                     m_dataBlocksCount;
    std::size_t                     m_maxAllocationsCount;
    double                          m_releaseThreshold;
};

template <typename T>
class ObjectHandler {
    using   DataBlock_T = details::DataBlock<T>;
    friend  DataBlock_T;
    
public:
    ~ObjectHandler();
    
    ObjectHandler(ObjectHandler const& handler) = delete;
    
    ObjectHandler(ObjectHandler && handler) noexcept;
    
    ObjectHandler& operator=(ObjectHandler const& handler) = delete;
    
    ObjectHandler& operator=(ObjectHandler && handler) noexcept;
    
    typename std::add_lvalue_reference<T>::type operator*() const noexcept;
    
    T* operator->() const noexcept;
    
    T* get() const noexcept;
    
    void swap(ObjectHandler<T>& other) noexcept;
    
private:
    ObjectHandler(T* ptr, DataBlock_T& parent, std::size_t slot);
    
    ObjectHandler(T* ptr, DataBlock_T& parent, std::size_t slot, std::shared_ptr<bool> parentPoolLivingPtr);
    
private:
    DataBlock_T&            m_parent;
    T*                      m_ptr;
    std::size_t             m_slot;
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    std::shared_ptr<bool>   m_parentPoolLiving;
#endif
};

namespace details {

template <typename T>
class DataBlock {
    using   Type_T = T;
    friend  ObjectHandler<T>;
    
public:
    DataBlock();
    
    ~DataBlock();
    
    DataBlock(DataBlock const& block) = delete;
    
    DataBlock(DataBlock && block) noexcept;
    
    DataBlock& operator=(DataBlock const& block) = delete;
    
    DataBlock& operator=(DataBlock && block) noexcept;
    
    template <typename... Args>
    ObjectHandler<T> New(Args && ...args);
    
    inline std::size_t getObjectsCount() const noexcept;
    
    inline bool isDataSlotAvailable() const noexcept;
    
    inline bool isEmpty() const noexcept;
    
    inline bool isFull() const noexcept;

private:
    void release(std::size_t slot);
    
    void releaseAll();
    
public:
    static constexpr std::size_t s_slotsCount = 256; // -- 1_000_000 : 2 | 15_000_000 : 59
//    static constexpr std::size_t s_slotsCount = 512; // -- 1_000_000 : 3 | 15_000_000 : 53
    
private:
    T*                          m_data = nullptr;
    std::size_t                 m_objectsCount = 0;
    std::bitset<s_slotsCount>   m_slots;
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    std::shared_ptr<bool>       m_poolExists;
#endif
};

}

} // mz

namespace std {

template <typename T>
void swap(mz::ObjectHandler<T>& lhs, mz::ObjectHandler<T>& rhs) noexcept;

} // std

#include "ObjectPool.inl"

#endif /* ObjectPool_h */
