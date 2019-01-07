//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_BASESYSTEM_H
#define ENTTX_BASESYSTEM_H

#include <boost/cstdfloat.hpp>
#include <cstddef>

namespace enttx {
template<class Derived>
class BaseSystem
{
public:
    template<typename... Args>
    void init(Args&&... args);

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<class Derived>
template<typename EntityManager, size_t STAGE>
void BaseSystem<Derived>::update(EntityManager* entityManager, boost::float32_t dt)
{
    static_cast<Derived*>(this)->update<EntityManager, STAGE>(entityManager, dt);
}

template<class Derived>
template<typename... Args>
void BaseSystem<Derived>::init(Args&&... args)
{
    static_cast<Derived*>(this)->init(std::forward<Args>(args)...);
}
}

#endif // ENTTX_BASESYSTEM_H
