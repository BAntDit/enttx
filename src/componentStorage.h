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
// TODO:: better storage with view, iterators, size and capacity

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
class ComponentStorage
  : public BaseComponentStorage<ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>, Component>
{
public:
    ComponentStorage();

    auto get(uint32_t index) const -> Component const&;

    auto get(uint32_t index) -> Component&;

    template<typename... Args>
    auto create(uint32_t index, Args&&... args) -> Component&;

    void destroy(uint32_t index);

    void resize(size_t chunkCount);

    void reset();

    auto data() const -> std::pair<Component const*, size_t>;

    auto data() -> std::pair<Component*, size_t>;

    auto capacity() const -> size_t { return store_.capacity(); }

    auto size() const -> size_t { return store_.size(); }

private:
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

    auto index_ = indices_[index];

    assert(index_ < store_.size());

    return store_[index_];
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::get(uint32_t index) -> Component&
{
    assert(index < indices_.size());

    auto index_ = indices_[index];

    assert(index_ < store_.size());

    return store_[index_];
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
template<typename... Args>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::create(uint32_t index, Args&&... args) -> Component&
{
    // TODO:: ...

    {
        auto size = indices_.size();

        if (index >= size) {

            size = index + 1;
            size = size + CHUNK_SIZE - size % CHUNK_SIZE;

            indices_.resize(size, std::numeric_limits<uint32_t>::max());
        }
    }

    uint32_t pos = indices_[index];

    if (pos == std::numeric_limits<uint32_t>::max()) {
        for (auto it = indices_.begin(); it != indices_.end(); it++) {
            if (*it == std::numeric_limits<uint32_t>::max()) continue;

            auto distance = std::distance(indices_.begin(), it);

            if (distance < index) {
                if (pos == std::numeric_limits<uint32_t>::max()) {
                    pos = *it + 1;
                }
            } else {
                if (pos == std::numeric_limits<uint32_t>::max()) {
                    pos = *it;
                }

                (*it)++;
            }
        }
    }

    indices_[index] = pos != std::numeric_limits<uint32_t>::max() ? pos : pos = 0;

    {
        auto capacity = store_.capacity();

        if (pos >= store_.capacity()) {
            // pos =
        }
    }

    return *(new (get(index)) Component(std::forward<Args>(args)...));
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::destroy(uint32_t index)
{
    assert(index < store_.size() * CHUNK_SIZE);

    get(index)->~Component();
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::resize(size_t chunkCount)
{
    store_.resize(chunkCount);
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
void ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::reset()
{
    store_.resize(0);
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::data() const -> std::pair<Component const*, size_t>
{
    return { static_cast<Component const*>(store_.data()), store_.data() * CHUNK_SIZE };
}

template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::data() -> std::pair<Component*, size_t>
{
    return { static_cast<Component*>(store_.data()), store_.data() * CHUNK_SIZE };
}
}

#endif // ENTTX_COMPONENTSTORAGE_H
