//
// Created by bantdit on 12/15/18.
//

#ifndef ENTTX_BASESTORAGE_H
#define ENTTX_BASESTORAGE_H

#include <cstddef>
#include <cstdint>
#include <utility>

namespace enttx {
template<typename Derived, typename Component>
class BaseComponentStorage
{
public:
    auto get(uint32_t index) const -> Component const&;

    auto get(uint32_t index) -> Component&;

    template<typename... Args>
    auto create(uint32_t index, Args&&... args) -> Component&;

    void destroy(uint32_t index);

    void resize(size_t chunkCount);

    void reset();

    auto data() const -> std::pair<Component const*, size_t>;

    auto data() -> std::pair<Component*, size_t>;
};

template<typename Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::get(uint32_t index) const -> Component const&
{
    return static_cast<Derived const*>(this)->get(index);
}

template<typename Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::get(uint32_t index) -> Component&
{
    return static_cast<Derived*>(this)->get(index);
}

template<typename Derived, typename Component>
template<typename... Args>
auto BaseComponentStorage<Derived, Component>::create(uint32_t index, Args&&... args) -> Component&
{
    return static_cast<Derived*>(this)->create(index, std::forward<Args>(args)...);
}

template<class Derived, typename Component>
void BaseComponentStorage<Derived, Component>::destroy(uint32_t index)
{
    return static_cast<Derived*>(this)->destroy(index);
}

template<class Derived, typename Component>
void BaseComponentStorage<Derived, Component>::resize(size_t chunkCount)
{
    static_cast<Derived*>(this)->resize(chunkCount);
}

template<class Derived, typename Component>
void BaseComponentStorage<Derived, Component>::reset()
{
    static_cast<Derived*>(this)->reset();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::data() const -> std::pair<Component const*, size_t>
{
    return static_cast<Derived const*>(this)->data();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::data() -> std::pair<Component*, size_t>
{
    return static_cast<Derived*>(this)->data();
}
}

#endif // ENTTX_BASESTORAGE_H
