//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_BASESYSTEM_H
#define ENTTX_BASESYSTEM_H

#include "config.h"
#include "entityManager.h"
#include <boost/cstdfloat.hpp>
#include <cstddef>

namespace enttx {
template<class EntityManager, class Derived>
class BaseSystem;

template<class Derived, typename... Components, typename... Storages>
class BaseSystem<EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>,
                 Derived>
{
public:
    using entity_manager_t =
      EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>;

    explicit BaseSystem(entity_manager_t* entityManager)
      : entityManager_{ entityManager }
    {}

    template<typename... Args>
    void init(Args&&... args);

    template<size_t STAGE>
    void update(boost::float32_t dt);

protected:
    entity_manager_t* entityManager_;
};

template<class Derived, typename... Components, typename... Storages>
template<size_t STAGE>
void BaseSystem<EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>,
                Derived>::update(boost::float32_t dt)
{
    static_cast<Derived*>(this)->update<STAGE>(dt);
}

template<class Derived, typename... Components, typename... Storages>
template<typename... Args>
void BaseSystem<EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>,
                Derived>::init(Args&&... args)
{
    static_cast<Derived*>(this)->init(std::forward<Args>(args)...);
}
}

#endif // ENTTX_BASESYSTEM_H
