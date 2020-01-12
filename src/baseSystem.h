//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_BASESYSTEM_H
#define ENTTX_BASESYSTEM_H

#include <cstddef>
#include <easy-mp/type_list.h>

namespace enttx {
template<class Derived>
class BaseSystem
{
public:
    using tag_t = easy_mp::type_list<>;

    template<typename... Args>
    void init(Args&&... args);

    template<typename SystemManager, typename EntityManager, size_t STAGE>
    void update(SystemManager& systemManager, EntityManager& entityManager);

    template<typename... Components>
    static constexpr bool is_in_use_v =
      std::is_same_v<typename easy_mp::inner_join<typename Derived::tag_t, easy_mp::type_list<Components...>>::type,
                     easy_mp::type_list<Components...>>;
};

template<class Derived>
template<typename SystemManager, typename EntityManager, size_t STAGE>
void BaseSystem<Derived>::update(SystemManager& systemManager, EntityManager& entityManager)
{
    static_cast<Derived*>(this)->template update<SystemManager, EntityManager, STAGE>(systemManager, entityManager);
}

template<class Derived>
template<typename... Args>
void BaseSystem<Derived>::init(Args&&... args)
{
    static_cast<Derived*>(this)->template init(std::forward<Args>(args)...);
}
}

#endif // ENTTX_BASESYSTEM_H
