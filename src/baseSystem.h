//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_BASESYSTEM_H
#define ENTTX_BASESYSTEM_H

#include <boost/cstdfloat.hpp>

namespace enttx {
template<class Derived>
class BaseSystem
{
public:
    template<typename... Args>
    void init(Args&&... args);

    void earlyUpdate(boost::float32_t dt);

    void lateUpdate(boost::float32_t dt);
};

template<class Derived>
void BaseSystem<Derived>::earlyUpdate(boost::float32_t dt)
{
    static_cast<Derived*>(this)->earlyUpdate(dt);
}

template<class Derived>
void BaseSystem<Derived>::lateUpdate(boost::float32_t dt)
{
    static_cast<Derived*>(this)->lateUpdate(dt);
}

template<class Derived>
template<typename... Args>
void BaseSystem<Derived>::init(Args&&... args)
{
    static_cast<Derived*>(this)->init(std::forward<Args>(args)...);
}
}

#endif // ENTTX_BASESYSTEM_H
