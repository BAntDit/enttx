//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_SYSTEMMANAGER_H
#define ENTTX_SYSTEMMANAGER_H

#include "config.h"
#include <boost/cstdfloat.hpp>
#include <cstddef>
#include <tuple>
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

    template<typename S, typename R = void>
    using enable_if_system = std::enable_if_t<system_list_t ::template has_type<S>::value, R>;

public:
    template<typename System>
    auto get() const -> enable_if_system<System, System const&>;

    template<typename System>
    auto get() -> enable_if_system<System, System&>;

    void update(boost::float32_t dt);

private:
    template<size_t STAGE, typename System>
    void _update(boost::float32_t dt);

    template<size_t STAGE>
    void _updateStage(boost::float32_t dt);

    template<size_t... STAGES>
    void _updateStages(boost::float32_t dt, std::index_sequence<STAGES...>);

private:
    std::tuple<Systems...> systems_;
};

template<typename... Systems>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::update(boost::float32_t dt)
{
    _updateStages(dt, std::make_index_sequence(UPDATE_STAGE_COUNT));
}

template<typename... Systems>
template<size_t... STAGES>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_updateStages(
  boost::float32_t dt,
  std::index_sequence<STAGES...>)
{
    (_updateStage<STAGES>(dt), ...);
}

template<typename... Systems>
template<size_t STAGE>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_updateStage(
  boost::float32_t dt)
{
    (_update<STAGE, Systems>(dt), ...);
}

template<typename... Systems>
template<size_t STAGE, typename System>
void SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::_update(
  boost::float32_t dt)
{
    std::get<system_list_t::template get_type_index<System>::value>(systems_).template update<STAGE>(dt);
}

template<typename... Systems>
template<typename System>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::get() const
  -> enable_if_system<System, System const&>
{
    return std::get<system_list_t::template get_type_index<System>::value>(systems_);
}

template<typename... Systems>
template<typename System>
auto SystemManager<SystemManagerConfig<UPDATE_STAGE_COUNT, easy_mp::type_list<Systems...>>>::get()
  -> enable_if_system<System, System&>
{
    return const_cast<System&>(std::as_const(this)->template get<System>());
}
}

#endif // ENTTX_SYSTEMMANAGER_H
