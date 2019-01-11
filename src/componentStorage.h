//
// Created by bantdit on 12/30/18.
//

#ifndef ENTTX_COMPONENTSTORAGE_H
#define ENTTX_COMPONENTSTORAGE_H

#include <array>
#include <assert.h>
#include <vector>

#include "baseComponentStorage.h"

namespace enttx {
template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
class ComponentStorage
  : public BaseComponentStorage<ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>, Component>
{
    ComponentStorage();

    auto get(uint32_t index) const -> Component const&;

    auto get(uint32_t index) -> Component&;

    template<typename... Args>
    auto create(uint32_t index, Args&&... args) -> Component&;

    void destroy(uint32_t index);

    auto capacity() const -> size_t { return store_.capacity(); }

    auto size() const -> size_t { return store_.size(); }

    auto begin() const { return store_.cbegin(); }

    auto end() const { return store_.cend(); }

    auto begin() { return store_.begin(); }

    auto end() { return store_.end(); }

private:
    void _reserveStoreIfNecessary(uint32_t pos);

    void _resizeIndicesIfNecessary(uint32_t index);

    std::vector<uint32_t> indices_;
    std::vector<Component> store_;

    uint32_t maxValidIndex_;
};

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::ComponentStorage()
  : indices_(CHUNK_SIZE * INITIAL_CHUNK_COUNT, std::numeric_limits<uint32_t>::max())
  , store_{}
  , maxValidIndex_{ std::numeric_limits<uint32_t>::max() }
{
    store_.reserve(CHUNK_SIZE * INITIAL_CHUNK_COUNT);
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::get(uint32_t index) const -> Component const&
{
    assert(index < indices_.size());

    auto pos = indices_[index];

    assert(pos < store_.size());

    return store_[pos];
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::get(uint32_t index) -> Component&
{
    assert(index < indices_.size());

    auto pos = indices_[index];

    assert(pos < store_.size());

    return store_[pos];
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
template<typename... Args>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::create(uint32_t index, Args&&... args) -> Component&
{
    assert(index != maxValidIndex_ || index >= indices_.size() ||
           indices_[index] == std::numeric_limits<uint32_t>::max());

    if (maxValidIndex_ == std::numeric_limits<uint32_t>::max()) {
        uint32_t pos = 0;

        maxValidIndex_ = index;

        _resizeIndicesIfNecessary(index);

        indices_[index] = pos;

        _reserveStoreIfNecessary(pos);

        store_.emplace(store_.cbegin(), std::forward<Args>(args)...);
    } else if (index > maxValidIndex_) {
        auto pos = indices_[maxValidIndex_];

        maxValidIndex_ = index;

        assert(pos < store_.size());

        _resizeIndicesIfNecessary(index);

        indices_[index] = ++pos;

        _reserveStoreIfNecessary(pos);

        store_.emplace(std::next(store_.cbegin(), pos), std::forward<Args>(args)...);
    } else {
        assert(indices_[maxValidIndex_] < store_.size());

        _reserveStoreIfNecessary(indices_[maxValidIndex_] + 1);

        auto pos = std::numeric_limits<uint32_t>::max();

        for (auto it = std::next(indices_.begin(), index); it != std::next(maxValidIndex_, maxValidIndex_ + 1); it++) {
            if (*it == std::numeric_limits<uint32_t>::max())
                continue;

            if (pos == std::numeric_limits<uint32_t>::max()) {
                pos = *it;
            }

            (*it)++;
        }

        assert(pos < store_.size());

        store_.emplace(std::next(store_.cbegin(), pos), std::forward<Args>(args)...);
    }
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::destroy(uint32_t index)
{
    assert(indices_[index] != std::numeric_limits<uint32_t>::max());

    auto pos = indices_[index];

    assert(pos < store_.size());

    store_.erase(std::next(store_.cbegin(), pos));

    indices_[index] = std::numeric_limits<uint32_t>::max();

    uint32_t lastValidIndex = std::numeric_limits<uint32_t>::max();

    for (auto it = std::next(indices_.begin(), index); it != std::next(indices_.begin(), maxValidIndex_ + 1); it++) {
        if (*it == std::numeric_limits<uint32_t>::max())
            continue;

        (*it)--;

        lastValidIndex = static_cast<uint32_t>(std::distance(indices_.begin(), it));
    }

    maxValidIndex_ = lastValidIndex;
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::_reserveStoreIfNecessary(uint32_t pos)
{
    auto capacity = store_.capacity();

    if (pos >= capacity) {
        capacity = pos + 1;
        capacity = capacity + CHUNK_SIZE - capacity % CHUNK_SIZE;

        store_.reserve(capacity);
    }
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::_resizeIndicesIfNecessary(uint32_t index)
{
    auto size = indices_.size();

    if (index >= size) {
        size = index + 1;
        size = size + CHUNK_SIZE - size % CHUNK_SIZE;

        indices_.resize(size, std::numeric_limits<uint32_t>::max());
    }
}
}

#endif // ENTTX_COMPONENTSTORAGE_H
