//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_SYSTEMMANAGER_H
#define ENTTX_SYSTEMMANAGER_H

#include "config.h"
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace enttx {
template<typename Config>
class SystemManager;

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
class SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>
{
public:
    using config_t = SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>;

    using system_list_t = typename config_t::system_list_t;

    template<typename... Components>
    using system_list_for_components_t =
      typename easy_mp::flatten<easy_mp::type_list<std::conditional_t<Systems::template is_in_use_v<Components...>,
                                                                      easy_mp::type_list<Systems&>,
                                                                      easy_mp::type_list<>>...>>::type;
    template<typename... Components>
    using const_system_list_for_components_t =
      typename easy_mp::flatten<easy_mp::type_list<std::conditional_t<Systems::template is_in_use_v<Components...>,
                                                                      easy_mp::type_list<Systems const&>,
                                                                      easy_mp::type_list<>>...>>::type;

    template<typename S, typename R = void>
    using enable_if_system = std::enable_if_t<system_list_t ::template has_type<S>::value, R>;

    SystemManager();

    template<typename System>
    auto get() const -> enable_if_system<System, System const&>;

    template<typename System>
    auto get() -> enable_if_system<System, System&>;

    template<typename... Components>
    auto getSystemsForComponents() ->
      typename system_list_for_components_t<Components...>::template specialization_t<std::tuple>;

    template<typename... Components>
    auto getSystemsForComponents() const ->
      typename const_system_list_for_components_t<Components...>::template specialization_t<std::tuple>;

    template<typename EntityManager, typename... Args>
    void update(EntityManager&& entityManager, Args&&... args);

    template<typename... Components>
    static constexpr bool has_system_for_components_v = (Systems::template is_in_use_v<Components...> | ...);

private:
    template<size_t STAGE, typename System, typename EntityManager, typename... Args>
    void _update(EntityManager&& entityManager, Args&&... args);

    template<size_t STAGE, typename EntityManager, typename... Args>
    void _updateStage(EntityManager&& entityManager, Args&&... args);

    template<size_t... STAGES, typename EntityManager, typename... Args>
    void _updateStages(std::index_sequence<STAGES...>, EntityManager&& entityManager, Args&&... args);

    template<typename... Ss>
    auto _getSystemTuple(easy_mp::type_list<Ss...>) const -> std::tuple<Ss...>;

    template<typename... Ss>
    auto _getSystemTuple(easy_mp::type_list<Ss...>) -> std::tuple<Ss...>;

private:
    std::tuple<Systems...> systems_;
};

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::SystemManager()
  : systems_{ Systems()... }
{}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename EntityManager, typename... Args>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::update(
  EntityManager&& entityManager,
  Args&&... args)
{
    _updateStages(std::make_index_sequence<UPDATE_STAGE_COUNT>{},
                  std::forward<EntityManager>(entityManager),
                  std::forward<Args>(args)...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<size_t... STAGES, typename EntityManager, typename... Args>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_updateStages(
  std::index_sequence<STAGES...>,
  EntityManager&& entityManager,
  Args&&... args)
{
    (_updateStage<STAGES>(std::forward<EntityManager>(entityManager), std::forward<Args>(args)...), ...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<size_t STAGE, typename EntityManager, typename... Args>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_updateStage(
  EntityManager&& entityManager,
  Args&&... args)
{
    (_update<STAGE, Systems>(std::forward<EntityManager>(entityManager), std::forward<Args>(args)...), ...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<size_t STAGE, typename System, typename EntityManager, typename... Args>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_update(
  EntityManager&& entityManager,
  Args&&... args)
{
    std::get<system_list_t::template get_type_index<System>::value>(systems_)
      .template update<std::decay_t<decltype(*this)>, STAGE>(
        *this, std::forward<EntityManager>(entityManager), std::forward<Args>(args)...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename System>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::get() const
  -> enable_if_system<System, System const&>
{
    return std::get<system_list_t::template get_type_index<System>::value>(systems_);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename System>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::get()
  -> enable_if_system<System, System&>
{
    return std::get<system_list_t::template get_type_index<System>::value>(systems_);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename... Ss>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_getSystemTuple(
  easy_mp::type_list<Ss...>) const -> std::tuple<Ss...>
{
    return std::tuple<Ss const&...>(std::get<std::decay_t<Ss>>(systems_)...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename... Ss>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_getSystemTuple(
  easy_mp::type_list<Ss...>) -> std::tuple<Ss...>
{
    return std::tuple<Ss&...>(std::get<std::decay_t<Ss>>(systems_)...);
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename... Components>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::getSystemsForComponents()
  -> typename system_list_for_components_t<Components...>::template specialization_t<std::tuple>
{
    return _getSystemTuple(system_list_for_components_t<Components...>{});
}

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
template<typename... Components>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::getSystemsForComponents()
  const -> typename const_system_list_for_components_t<Components...>::template specialization_t<std::tuple>
{
    return _getSystemTuple(const_system_list_for_components_t<Components...>{});
}
}

#endif // ENTTX_SYSTEMMANAGER_H
