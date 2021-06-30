//
//  ObjectPool.inl
//  Maze
//
//  Created by Jaraxus on 22/01/2021.
//

namespace mz {

template <typename T>
ObjectPool<T>::ObjectPool() :
ObjectPool(1., std::numeric_limits<std::size_t>::max())
{
}

template <typename T>
ObjectPool<T>::ObjectPool(double releaseThreshold, std::size_t maxAllocationsCount) :
m_dataBlocks(),
m_dataBlocksCount(0),
m_maxAllocationsCount(maxAllocationsCount),
m_releaseThreshold(releaseThreshold)
{
    m_currentDataBlock = pushDataBlock();
}

template <typename T>
template <typename... Args>
ObjectHandler<T> ObjectPool<T>::New(Args && ...args) {
    auto targetBlock = m_currentDataBlock;
    do {
        if (targetBlock->isDataSlotAvailable())
            break;
        targetBlock = (++targetBlock != m_dataBlocks.end() ? targetBlock : m_dataBlocks.begin());
    } while (targetBlock != m_currentDataBlock);
    
    if (targetBlock != m_currentDataBlock && static_cast<double>(getObjectsCount()) / (m_dataBlocksCount * DataBlock_T::s_slotsCount) < m_releaseThreshold) {
        std::size_t blocksToDelete = static_cast<std::size_t>(std::ceil((1. - m_releaseThreshold) * m_dataBlocksCount));
        std::size_t blocksDeleted = 0;
        for (auto blockIt = std::begin(m_dataBlocks), previousIt = m_dataBlocks.before_begin(); blockIt != std::end(m_dataBlocks) && blocksDeleted < blocksToDelete; ++blockIt, ++previousIt) {
            if (blockIt->isEmpty() && blockIt != targetBlock)
                m_dataBlocks.erase_after(previousIt);
        }
    }
    
    m_currentDataBlock = targetBlock;
    
    if (!m_currentDataBlock->isDataSlotAvailable()) {
        if (m_dataBlocksCount * DataBlock_T::s_slotsCount > m_maxAllocationsCount)
            throw std::bad_alloc();
        m_currentDataBlock = pushDataBlock();
    }
    
    return m_currentDataBlock->New(std::forward<Args>(args)...);
}

template <typename T>
inline std::size_t ObjectPool<T>::getObjectsCount() const noexcept {
    std::size_t objectsCount = 0;
    for (auto& block : m_dataBlocks)
        objectsCount += block.getObjectsCount();
    return objectsCount;
}

template <typename T>
inline typename ObjectPool<T>::DataBlockIterator ObjectPool<T>::pushDataBlock() {
    m_dataBlocks.emplace_front();
    ++m_dataBlocksCount;
    return m_dataBlocks.begin();
}

template <typename T>
ObjectHandler<T>::~ObjectHandler() {
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    if (m_ptr) {
        if (*m_parentPoolLiving)
            m_parent.release(m_slot);
        else
            mz::Logs::Global.display("Object Handler tried to release its object but the parent pool was already destructed", LogMessageType::ERROR);
    }
#else
    if (m_ptr)
        m_parent.release(m_slot);
#endif
}

template <typename T>
ObjectHandler<T>::ObjectHandler(ObjectHandler && handler) noexcept :
m_parent(handler.m_parent),
m_ptr(std::exchange(handler.m_ptr, nullptr)),
m_slot(std::exchange(handler.m_slot, std::numeric_limits<std::size_t>::max()))
{
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_parentPoolLiving = std::exchange(handler.m_parentPoolLiving, nullptr);
#endif
}

template <typename T>
ObjectHandler<T>& ObjectHandler<T>::operator=(ObjectHandler && handler) noexcept {
    m_parent = handler.m_parent;
    m_ptr = std::exchange(handler.m_ptr, nullptr);
    m_slot = std::exchange(handler.m_slot, std::numeric_limits<std::size_t>::max());
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_parentPoolLiving = std::exchange(handler.m_parentPoolLiving, nullptr);
#endif
}

template <typename T>
typename std::add_lvalue_reference<T>::type ObjectHandler<T>::operator*() const noexcept {
    return *m_ptr;
}

template <typename T>
T* ObjectHandler<T>::operator->() const noexcept {
    return m_ptr;
}

template <typename T>
T* ObjectHandler<T>::get() const noexcept {
    return m_ptr;
}

template <typename T>
void ObjectHandler<T>::swap(ObjectHandler<T>& other) noexcept {
    std::swap(m_parent, other.m_parent);
    std::swap(m_slot, other.m_slot);
    std::swap(m_ptr, other.m_ptr);
}

template <typename T>
ObjectHandler<T>::ObjectHandler(T* ptr, DataBlock_T& parent, std::size_t slot) :
m_parent(parent),
m_ptr(ptr),
m_slot(slot)
{
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    mz::Logs::Global.display("Memory safety required for pool object but no safety pointer was provided", LogMessageType::WARNING);
#endif
}

template <typename T>
ObjectHandler<T>::ObjectHandler(T* ptr, DataBlock_T& parent, std::size_t slot, std::shared_ptr<bool> parentPoolLivingPtr) :
m_parent(parent),
m_ptr(ptr),
m_slot(slot)
{
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_parentPoolLiving = parentPoolLivingPtr;
#endif
}

namespace details {

template <typename T>
DataBlock<T>::DataBlock() :
m_data(static_cast<Type_T*>(::operator new(s_slotsCount * sizeof(Type_T)))),
m_objectsCount(0),
m_slots()
{
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_poolExists = std::make_shared<bool>(true);
#endif
    m_slots.flip();
}

template <typename T>
DataBlock<T>::~DataBlock() {
    releaseAll();
}

template <typename T>
DataBlock<T>::DataBlock(DataBlock && block) noexcept :
m_data(std::exchange(block.m_data, nullptr)),
m_objectsCount(std::exchange(block.m_objectsCount, 0)),
m_slots(block.m_slots)
{
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_poolExists = std::exchange(block.m_poolExists, nullptr);
#endif
}

template <typename T>
DataBlock<T>& DataBlock<T>::operator=(DataBlock && block) noexcept {
    releaseAll();
    m_data = std::exchange(block.m_data, nullptr);
    m_objectsCount = std::exchange(block.m_objectsCount, 0);
    m_slots = block.m_slots;
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    m_poolExists = std::exchange(block.m_poolExists, nullptr);
#endif
}

template <typename T>
template <typename... Args>
ObjectHandler<T> DataBlock<T>::New(Args && ...args) {
    static_assert(std::is_constructible<T, Args...>::value, "Arguments given must be able to construct an object of the pool");
    
    std::size_t availableSlot = findFirstFreeBit(m_slots);
    assert(availableSlot != std::numeric_limits<std::size_t>::max());
    
    Type_T* ptrConstructed = new (m_data + availableSlot) T(std::forward<Args>(args)...);
    m_slots[availableSlot] = false;
    ++m_objectsCount;
    
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    return ObjectHandler(ptrConstructed, *this, availableSlot, m_poolExists);
#else
    return ObjectHandler(ptrConstructed, *this, availableSlot);
#endif
}

template <typename T>
inline std::size_t DataBlock<T>::getObjectsCount() const noexcept {
    return m_objectsCount;
}

template <typename T>
inline bool DataBlock<T>::isDataSlotAvailable() const noexcept {
    return (m_objectsCount != s_slotsCount);
}

template <typename T>
inline bool DataBlock<T>::isEmpty() const noexcept {
    return (m_objectsCount == 0);
}

template <typename T>
inline bool DataBlock<T>::isFull() const noexcept {
    return (m_objectsCount == s_slotsCount);
}

template <typename T>
void DataBlock<T>::release(std::size_t slot) {
    assert(!m_slots[slot]);
    m_data[slot].~Type_T();
    m_slots[slot] = true;
    --m_objectsCount;
}

template <typename T>
void DataBlock<T>::releaseAll() {
    if (!m_data)
        return;
    
    for (std::size_t clearIndex = 0; clearIndex < s_slotsCount; ++clearIndex) {
        if (!m_slots[clearIndex])
            m_data[clearIndex].~Type_T();
    }
    
    ::operator delete(m_data, s_slotsCount * sizeof(Type_T));
    m_data = nullptr;
    
#ifdef MAZE_SYSTEM_OBJECT_POOL_MEMORY_SAFETY
    *m_poolExists = false;
#endif
}

} // details

} // mz

namespace std {

template <typename T>
void swap(mz::ObjectHandler<T>& lhs, mz::ObjectHandler<T>& rhs) noexcept {
    lhs.swap(rhs);
} // std

}

