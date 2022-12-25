//
// Created by bantdit on 12/15/18.
//

#ifndef ENTTX_ENTITYMANAGER_H
#define ENTTX_ENTITYMANAGER_H

#include <cassert>
#include <cstdint>
#include <easy-mp/containers.h>
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

    template<typename R, typename... Cs>
    using enable_if_components = std::enable_if_t<(... && component_list_t::template has_type<Cs>::value), R>;

    template<typename C>
    using component_storage_t =
      typename storage_list_t::template get_type<component_list_t::template get_type_index<C>::value>::type;

    explicit EntityManager(size_t initialSize = 10000);

    auto size() const -> size_t;

    auto capacity() const -> size_t;

    auto create() -> Entity;

    template<typename Container>
    auto create(Container&& entities)
      -> std::enable_if_t<easy_mp::is_contiguous_v<Container> &&
                            std::is_same_v<typename std::decay_t<Container>::value_type, Entity>,
                          Container&&>;

    void destroy(Entity entity);

    auto isValid(Entity entity) const -> bool;

    template<typename... Cs>
    void remove(Entity entity);

    template<typename Component, typename... Args>
    auto assign(Entity entity, Args&&... args) -> enable_if_component<Component, Component&>;

    template<typename Component>
    auto getComponent(Entity entity) const -> enable_if_component<Component, Component const*>;

    template<typename Component>
    auto getComponent(Entity entity) -> enable_if_component<Component, Component*>;

    template<typename... Cs>
    auto getComponents(Entity entity) const -> enable_if_components<std::tuple<Cs const*...>, Cs...>;

    template<typename... Cs>
    auto getComponents(Entity entity) -> enable_if_components<std::tuple<Cs*...>, Cs...>;

    template<typename Component>
    auto hasComponent(Entity entity) const -> enable_if_component<Component, bool>;

    template<typename... Cs>
    auto hasComponents(Entity entity) const -> enable_if_components<std::bitset<sizeof...(Cs)>, Cs...>;

    template<typename Fn, typename Component>
    auto applyForComponents(Entity entity, Fn&& fn) -> enable_if_component<Component>;

    template<typename Fn, typename Component, typename... Tail>
    void applyForComponents(Entity entity, Fn&& fn);

    template<typename Component>
    auto getStorage() const -> enable_if_component<Component, component_storage_t<Component> const&>;

    template<typename Component>
    auto getStorage() -> enable_if_component<Component, component_storage_t<Component>&>;

    template<bool isConst, typename... FilterComponents>
    class View
    {
    private:
        using entity_manager_t =
          typename std::conditional_t<isConst, EntityManager<config_t> const&, EntityManager<config_t>&>;

    public:
        using filter_component_list_t = easy_mp::type_list<FilterComponents...>;

        class Iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type = std::tuple<Entity, FilterComponents&...>;
            using difference_type = uint32_t;
            using pointer = value_type*;
            using reference = value_type&;

            auto operator++() -> Iterator&;

            auto operator==(Iterator const& rhs) const -> bool { return cursor_ == rhs.cursor_; }
            auto operator!=(Iterator const& rhs) const -> bool { return cursor_ != rhs.cursor_; }

            auto operator*() const -> std::tuple<Entity, FilterComponents&...>;

        private:
            friend class View<isConst, FilterComponents...>;

            Iterator(entity_manager_t entityManager, component_mask_t filter, uint32_t cursor)
              : cursor_{ cursor }
              , capacity_{ entityManager.capacity() }
              , filter_{ filter }
              , entityManager_{ entityManager }
            {}

            void next();

            uint32_t cursor_;
            size_t capacity_;

            component_mask_t filter_;

            entity_manager_t entityManager_;
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

        explicit View(entity_manager_t entityManager)
          : entityManager_{ entityManager }
          , filter_{}
        {
            static_assert(std::is_same_v<typename easy_mp::inner_join<component_list_t, filter_component_list_t>::type,
                                         filter_component_list_t>);

            (filter_.set(component_list_t::template get_type_index<FilterComponents>::value), ...);
        }

        entity_manager_t entityManager_;

        component_mask_t filter_;
    };

    template<typename... FilterComponents>
    auto getView() const -> View<true, FilterComponents...>;

    template<typename... FilterComponents>
    auto getView() -> View<false, FilterComponents...>;

private:
    template<typename Component>
    auto _remove(Entity entity) -> enable_if_component<Component>;

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
    return versions_.size();
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
template<typename Container>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::create(
  Container&& entities) -> std::enable_if_t<easy_mp::is_contiguous_v<Container> &&
                                              std::is_same_v<typename std::decay_t<Container>::value_type, Entity>,
                                            Container&&>
{
    size_t count = entities.size();
    size_t counter = 0;

    uint32_t index = 0;
    uint32_t version = 0;

    while (counter < count && !dump_.empty()) {
        index = dump_.back();

        dump_.pop_back();

        version = versions_[index];

        entities[counter++] = Entity{ index, version };
    }

    {
        size_t left = count - counter;
        size_t size = versions_.size();
        size_t capacity = versions_.capacity();

        if (size + left > capacity) {
            capacity = size + left;

            versions_.reserve(capacity);
            masks_.reserve(capacity);
            dump_.reserve(capacity);
        }
    }

    while (counter < count) {
        index = versions_.size();
        version = 1;

        versions_.emplace_back(version);
        masks_.emplace_back();

        entities[counter++] = Entity{ index, version };
    }

    return std::forward<Container>(entities);
}

template<typename... Components, typename... Storages>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::destroy(
  Entity entity)
{
    assert(isValid(entity));

    remove<Components...>(entity);

    versions_[entity.index()]++;

    dump_.emplace_back(entity.index());
}

template<typename... Components, typename... Storages>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::isValid(
  Entity entity) const -> bool
{
    return entity.index() < versions_.size() && versions_[entity.index()] == entity.version();
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::_remove(
  Entity entity) -> enable_if_component<Component>
{
    assert(isValid(entity));

    if (masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)) {
        masks_[entity.index()].reset(component_list_t::template get_type_index<Component>::value);

        std::get<component_list_t::template get_type_index<Component>::value>(storage_).destroy(entity.index());
    }
}

template<typename... Components, typename... Storages>
template<typename... Cs>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::remove(
  Entity entity)
{
    (_remove<Cs>(entity), ...);
}

template<typename... Components, typename... Storages>
template<typename Component, typename... Args>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::assign(
  Entity entity,
  Args&&... args) -> enable_if_component<Component, Component&>
{
    assert(isValid(entity));

    remove<Component>(entity);

    masks_[entity.index()].set(component_list_t::template get_type_index<Component>::value);

    return std::get<component_list_t::template get_type_index<Component>::value>(storage_).create(
      entity.index(), std::forward<Args>(args)...);
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponent(Entity entity) const -> enable_if_component<Component, Component const*>
{
    assert(isValid(entity));

    return masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)
             ? &(std::get<component_list_t::template get_type_index<Component>::value>(storage_).get(entity.index()))
             : nullptr;
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<
  EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getComponent(Entity entity)
  -> enable_if_component<Component, Component*>
{
    return const_cast<Component*>(std::as_const(*this).template getComponent<Component>(entity));
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponents(Entity entity) const -> enable_if_components<std::tuple<Cs const*...>, Cs...>
{
    return std::tuple<Cs const*...>(getComponent<Cs>(entity)...);
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  getComponents([[maybe_unused]] Entity entity) -> enable_if_components<std::tuple<Cs*...>, Cs...>
{
    return std::tuple<Cs*...>(const_cast<Cs*>(std::as_const(*this).template getComponent<Cs>(entity))...);
}

template<typename... Components, typename... Storages>
template<typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  hasComponent(Entity entity) const -> enable_if_component<Component, bool>
{
    assert(isValid(entity));

    return masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value);
}

template<typename... Components, typename... Storages>
template<typename... Cs>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  hasComponents(Entity entity) const -> enable_if_components<std::bitset<sizeof...(Cs)>, Cs...>
{
    std::bitset<sizeof...(Cs)> result;

    (result.set(hasComponent<Cs>(entity)), ...);

    return result;
}

template<typename... Components, typename... Storages>
template<typename Fn, typename Component>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  applyForComponents(Entity entity, Fn&& fn) -> enable_if_component<Component>
{
    static_assert(std::is_invocable_v<Fn, Component&>);

    assert(isValid(entity));

    if (masks_[entity.index()].test(component_list_t::template get_type_index<Component>::value)) {
        fn(std::get<component_list_t::template get_type_index<Component>::value>(storage_).template get(entity));
    }
}

template<typename... Components, typename... Storages>
template<typename Fn, typename Component, typename... Tail>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>,
                                       easy_mp::type_list<Storages...>>>::applyForComponents(Entity entity, Fn&& fn)
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
    return const_cast<component_storage_t<Component>&>(std::as_const(*this).template getStorage<Component>());
}

template<typename... Components, typename... Storages>
template<bool isConst, typename... FilterComponents>
void EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  View<isConst, FilterComponents...>::Iterator::next()
{
    if constexpr (sizeof...(FilterComponents) != 0) {
        while (cursor_ < capacity_ && (entityManager_.masks_[cursor_] & filter_) != filter_) {
            cursor_++;
        }
    }

    if constexpr (sizeof...(FilterComponents) == 0) {
        while (cursor_ < capacity_ && entityManager_.masks_[cursor_].none()) {
            cursor_++;
        }
    }
}

template<typename... Components, typename... Storages>
template<bool isConst, typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  View<isConst, FilterComponents...>::Iterator::operator++()
    -> EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
      View<isConst, FilterComponents...>::Iterator&
{
    cursor_++;
    next();
    return *this;
}

template<typename... Components, typename... Storages>
template<bool isConst, typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
  View<isConst, FilterComponents...>::Iterator::operator*() const -> std::tuple<Entity, FilterComponents&...>
{
    auto entity = Entity{ cursor_, entityManager_.versions_[cursor_] };

    [[maybe_unused]] auto components = entityManager_.template getComponents<FilterComponents...>(entity);

    return std::tie(entity, (*std::get<FilterComponents*>(components))...);
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getView()
  const -> EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>,
                                             easy_mp::type_list<Storages...>>>::View<true, FilterComponents...>
{
    return EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
      View<true, FilterComponents...>{ *this };
}

template<typename... Components, typename... Storages>
template<typename... FilterComponents>
auto EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::getView()
  -> EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>,
                                       easy_mp::type_list<Storages...>>>::View<false, FilterComponents...>
{
    return EntityManager<EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>>::
      View<false, FilterComponents...>{ *this };
}
}

#endif // ENTTX_ENTITYMANAGER_H
