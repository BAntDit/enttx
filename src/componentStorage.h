//
// Created by bantdit on 12/30/18.
//

#ifndef ENTTX_COMPONENTSTORAGE_H
#define ENTTX_COMPONENTSTORAGE_H

#include <vector>
#include <array>
#include <assert.h>

#include "baseComponentStorage.h"

namespace enttx {
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

    private:
        std::vector<std::array<Component, CHUNK_SIZE>> store_;
    };

    template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
    ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::ComponentStorage() :
        store_{}
    {
        store_.resize(INITIAL_CHUNK_COUNT);
    }

    template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
    auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::get(uint32_t index) const -> Component const&
    {
        assert(index < store_.size() * CHUNK_SIZE);

        return *(store_[index / CHUNK_SIZE].data() + index % CHUNK_SIZE);
    }

    template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
    auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>::get(uint32_t index) -> Component&
    {
        assert(index < store_.size() * CHUNK_SIZE);

        return *(store_[index / CHUNK_SIZE].data() + index % CHUNK_SIZE);
    }

    template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
    template<typename... Args>
    auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>
            ::create(uint32_t index, Args&&... args) -> Component&
    {
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
    auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>
        ::data() const -> std::pair<Component const*, size_t>
    {
        return { static_cast<Component const*>(store_.data()), store_.data() * CHUNK_SIZE };
    }

    template<size_t CHUNK_SIZE, size_t INITIAL_CHUNK_COUNT, typename Component>
    auto ComponentStorage<CHUNK_SIZE, INITIAL_CHUNK_COUNT, Component>
        ::data() -> std::pair<Component*, size_t>
    {
        return { static_cast<Component*>(store_.data()), store_.data() * CHUNK_SIZE };
    }
}

#endif //ENTTX_COMPONENTSTORAGE_H
