//
// Created by bantdit on 12/15/18.
//

#ifndef ENTTX_ENTITYMANAGER_H
#define ENTTX_ENTITYMANAGER_H

#include <cassert>
#include <cstdint>
#include <iterator>
#include <tuple>
#include <vector>

#include "config.h"
#include "entity.h"

namespace enttx {
template<typename Config>
class EntityManager;

template<typename... Components, typename... Storages>
class EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>
{
public:
    using config_t = EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>;

    using component_mask_t = typename config_t::component_mask_t;

    using component_list_t = typename config_t::component_list_t;

    using storage_list_t = typename config_t::storage_list_t;

    template<typename C, typename R = void>
    using enable_if_component = std::enable_if_t<component_list_t::template has_type<C>::value, R>;

    template<typename R, typename... RequestedComponents>
    using enable_if_components = std::enable_if_t<
      std::is_same_v<typename easy_mp::inner_join<component_list_t, easy_mp::type_list<RequestedComponents...>>::type,
                     easy_mp::type_list<RequestedComponents...>>,
      R>;

    template<typename C>
    using component_storage_t =
      typename storage_list_t::template get_type<component_list_t::template get_type_index<C>::value>::type;

    explicit EntityManager(size_t initialSize = 10000);

    auto size() const -> size_t;

    auto capacity() const -> size_t;

    auto create() -> Entity;

    template<uint32_t COUNT>
    auto createMany(std::array<Entity, COUNT>& entities) -> std::array<Entity, COUNT>&;

    template<uint32_t COUNT>
    auto createMany(std::array<Entity, COUNT>&& entities) -> std::array<Entity, COUNT>&&;

    void destroy(Entity const& entity);

    auto isValid(Entity const& entity) const -> bool;

    template<typename Component>
    auto remove(Entity const& entity) -> enable_if_component<Component>;

    template<typename... Cs>
    void remove(Entity const& entity);

    template<typename Component, typename... Args>
    auto assign(Entity const& entity, Args&&... args) -> enable_if_component<Component, Component&>;

    template<typename Component>
    auto getComponent(Entity const& entity) const -> enable_if_component<Component, Component const*>;

    template<typename Component>
    auto getComponent(Entity const& entity) -> enable_if_component<Component, Component*>;

    template<typename... Cs>
    auto getComponents(Entity const& entity) const -> enable_if_components<std::tuple<Cs const*...>, Cs...>;

    template<typename... Cs>
    auto getComponents(Entity const& entity) -> enable_if_components<std::tuple<Cs*...>, Cs...>;

    template<typename Component>
    auto hasComponent(Entity const& entity) const -> enable_if_component<Component, bool>;

    template<typename... Cs>
    auto hasComponents(Entity const& entity) const -> enable_if_components<std::bitset<sizeof...(Cs)>, Cs...>;

    template<typename Fn, typename Component>
    auto applyForComponents(Entity const& entity, Fn&& fn) -> enable_if_component<Component>;

    template<typename Fn, typename Component, typename... Tail>
    void applyForComponents(Entity const& entity, Fn&& fn);

    template<typename Component>
    auto getStorage() const -> enable_if_component<Component, component_storage_t<Component> const&>;

    template<typename Component>
    auto getStorage() -> enable_if_component<Component, component_storage_t<Component>&>;

    template<typename... FilterComponents>
    class View
    {
    public:
        using filter_component_list_t = easy_mp::type_list<FilterComponents...>;

        class Iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = Entity;
            using difference_type = uint32_t;
            using pointer = Entity*;
            using reference = Entity&;

            auto operator++() -> Iterator&;

            auto operator==(Iterator const& rhs) const -> bool { return cursor_ == rhs.cursor_; }
            auto operator!=(Iterator const& rhs) const -> bool { return cursor_ != rhs.cursor_; }

            auto operator*() const -> Entity;

        private:
            friend class View<FilterComponents...>;

            Iterator(EntityManager<config_t> const& entityManager, component_mask_t filter, uint32_t cursor)
              : cursor_{ cursor }
              , capacity_{ entityManager.capacity() }
              , filter_{ filter }
              , entityManager_{ entityManager }
            {}

            void next();

            uint32_t cursor_;
            size_t capacity_;

            component_mask_t filter_;

            EntityManager<config_t> const& entityManager_;
        };

        auto begin() const -> Iterator
        {
            Iterator iterator{ entityManager_, filter_, 0 };

            iterator.next();

            return iterator;
        }

        auto end() const -> Iterator
        {
            return Iterator(entityManager_, filter_, static_cast<uint32_t>(entityManager_.capacity()));
        }

    private:
        friend class EntityManager<config_t>;

        explicit View(EntityManager<config_t> const& entityManager)
          : entityManager_{ entityManager }
          , filter_{}
        {
            static_assert(std::is_same_v<typename easy_mp::inner_join<component_list_t, filter_component_list_t>::type,
                                         filter_component_list_t>);

            (filter_.set(component_list_t::template get_type_index<FilterComponents>::value), ...);
        }

        EntityManager<config_t> const& entityManager_;

        component_mask_t filter_;
    };

    template<typename... FilterComponents>
    auto getView() const -> View<FilterComponents...>;

private:
    std::vector<uint32_t> versions_;
    std::vector<uint32_t> dump_;
    std::vector<component_mask_t> masks_;

    std::tuple<Storages...> storage_;
};

template<typename... Components, typename... Storages>
EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::EntityManager(
  size_t initialSize /* = 10000*/)
  : versions_{}
  , dump_{}
  , masks_{}
  , storage_{}
{
    versions_.reserve(initialSize);
    dump_.reserve(initialSize);
    masks_.reserve(initialSize);
}

template<typename... Components, typename... Storages>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::size()
  const -> size_t
{
    return versions_.size() - dump_.size();
}

template<typename... Components, typename... Storages>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::capacity()
  const -> size_t
{
    return versions_.capacity();
}

template<typename... Components, typename... Storages>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::create()
  -> Entity
{
    uint32_t index = 0, version = 0;

    if (dump_.empty()) {
        index = versions_.size();
        version = 1;

        versions_.emplace_back(version);
        masks_.emplace_back();
    } else {
        index = dump_.back();
        version = versions_[index];

        dump_.pop_back();
    }

    return Entity{ index, version };
}

template<typename... Components, typename... Storages>
template<uint32_t COUNT>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::createMany(
  std::array<Entity, COUNT>& entities) -> std::array<Entity, COUNT>&
{
    uint32_t counter = 0;

    uint32_t index = 0;
    uint32_t version = 0;

    while (counter < COUNT && !dump_.empty()) {
        index = dump_.back();

        dump_.pop_back();

        version = versions_[index];

        entities[counter++] = Entity{ index, version };

        counter++;
    }

    {
        uint32_t left = COUNT - counter;
        uint32_t size = versions_.size();
        uint32_t capacity = versions_.capacity();

        if (size + left > capacity) {
            capacity = size + left;

            versions_.reserve(capacity);
            masks_.reserve(capacity);
            dump_.reserve(capacity);
        }
    }

    while (counter < COUNT) {
        index = versions_.size();
        version = 1;

        versions_.emplace_back(version);
        masks_.emplace_back();

        entities[counter++] = Entity{ index, version };
    }

    return entities;
}

template<typename... Components, typename... Storages>
template<uint32_t COUNT>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::createMany(
  std::array<Entity, COUNT>&& entities) -> std::array<Entity, COUNT>&&
{
    return std::move(createMany(entities));
}

template<typename... Components, typename... Storages>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::destroy(
  Entity const& entity)
{
    assert(isValid(entity));

    remove<Components...>(entity);

    versions_[entity.index()]++;

    dump_.emplace_back(entity.index());
}

template<typename... Components, typename... Storages>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::isValid(
  Entity const& entity) const -> bool
{
    return entity.index() < versions_.size() && versions_[entity.index()] == entity.version();
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::remove(
  Entity const& entity) -> enable_if_component<Component>
{
    assert(isValid(entity));

    if (masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)) {
        masks_[entity.index()].reset(component_list_t::template get_type_index<Component>::value);

        std::get<component_list_t::template get_type_index<Component>::value>(storage_).template destroy(
          entity.index());
    }
}

template<typename... Components, typename... Storages>
template<typename... Cs>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::remove(
  Entity const& entity)
{
    (remove<Cs>(entity), ...);
}

template<typename... Components, typename... Storages>
template<typename Component, typename... Args>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::assign(
  Entity const& entity,
  Args&&... args) -> enable_if_component<Component, Component&>
{
    assert(isValid(entity));

    remove<Component>(entity);

    masks_[entity.index()].set(component_list_t::template get_type_index<Component>::value);

    return std::get<component_list_t::template get_type_index<Component>::value>(storage_).template create(
      entity.index(), std::forward<Args>(args)...);
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponent(Entity const& entity) const -> enable_if_component<Component, Component const*>
{
    assert(isValid(entity));

    return masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)
             ? &(std::get<component_list_t::template get_type_index<Component>::value>(storage_).template get(entity))
             : nullptr;
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponent(Entity const& entity) -> enable_if_component<Component, Component*>
{
    return const_cast<Component*>(std::as_const(this)->template getComponent<Component>(entity));
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponents(Entity const& entity) const -> enable_if_components<std::tuple<Cs const*...>, Cs...>
{
    return std::tuple<Cs const*...>(getComponent<Cs>(entity)...);
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponents(Entity const& entity) -> enable_if_components<std::tuple<Cs*...>, Cs...>
{
    return std::tuple<Cs*...>(const_cast<Cs*>(std::as_const(this)->template getComponent<Cs>(entity))...);
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  hasComponent(Entity const& entity) const -> enable_if_component<Component, bool>
{
    assert(isValid(entity));

    return masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value);
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  hasComponents(Entity const& entity) const -> enable_if_components<std::bitset<sizeof...(Cs)>, Cs...>
{
    std::bitset<sizeof...(Cs)> result;

    (result.set(hasComponent<Cs>(entity)), ...);

    return result;
}

template<typename... Components, typename... Storages>
template<typename Fn, typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  applyForComponents(Entity const& entity, Fn&& fn) -> enable_if_component<Component>
{
    static_assert(std::is_invocable_v<Fn, Component&>);

    assert(isValid(entity));

    if (masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)) {
        fn(std::get<component_list_t::template get_type_index<Component>::value>(storage_).template get(entity));
    }
}

template<typename... Components, typename... Storages>
template<typename Fn, typename Component, typename... Tail>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  applyForComponents(Entity const& entity, Fn&& fn)
{
    applyForComponents<Fn, Component>(entity, std::forward<Fn>(fn));
    applyForComponents<Fn, Tail...>(entity, std::forward<Fn>(fn));
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<
  EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getStorage() const
  -> enable_if_component<Component, component_storage_t<Component> const&>
{
    return std::get<component_list_t::template get_type_index<Component>::value>(storage_);
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<
  EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getStorage()
  -> enable_if_component<Component, component_storage_t<Component>&>
{
    return const_cast<component_storage_t<Component>&>(std::as_const(this)->template getStorage<Component>());
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
  FilterComponents...>::Iterator::next()
{
    if constexpr (sizeof...(FilterComponents) != 0) {
        while (cursor_ < capacity_ && (entityManager_.mask_[cursor_] & filter_) != filter_) {
            cursor_++;
        }
    }

    if constexpr (sizeof...(FilterComponents) == 0) {
        while (cursor_ < capacity_ && entityManager_.mask_[cursor_].any()) {
            cursor_++;
        }
    }
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
  FilterComponents...>::Iterator::operator++()
  -> EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
    FilterComponents...>::Iterator&
{
    cursor_++;
    next();
    return *this;
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
  FilterComponents...>::Iterator::operator*() const -> Entity
{
    return Entity{ cursor_, entityManager_.versions_[cursor_] };
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getView()
  const -> EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
    FilterComponents...>
{
    return EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::View<
      FilterComponents...>{ *this };
}
}

#endif // ENTTX_ENTITYMANAGER_H
