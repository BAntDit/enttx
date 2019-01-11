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

    auto capacity() const -> size_t;

    auto size() const -> size_t;

    auto begin() const;

    auto end() const;

    auto begin();

    auto end();
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
auto BaseComponentStorage<Derived, Component>::capacity() const -> size_t
{
    return static_cast<Derived const*>(this)->capacity();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::size() const -> size_t
{
    return static_cast<Derived const*>(this)->size();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::begin() const
{
    return static_cast<Derived const*>(this)->begin();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::end() const
{
    return static_cast<Derived const*>(this)->end();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::begin()
{
    return static_cast<Derived*>(this)->begin();
}

template<class Derived, typename Component>
auto BaseComponentStorage<Derived, Component>::end()
{
    return static_cast<Derived*>(this)->end();
}
}

#endif // ENTTX_BASESTORAGE_H
