//
// Created by bantdit on 12/31/18.
//

#ifndef ENTTX_SYSTEMMANAGER_H
#define ENTTX_SYSTEMMANAGER_H

#include <tuple>
#include <boost/cstdfloat.hpp>
#include "config.h"

namespace enttx
{
    template<typename Config>
    class SystemManager;

    template<typename... Systems>
    class SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>
    {
    public:
        using config_t = SystemManagerConfig<easy_mp::type_list<Systems...>>;

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
        template<typename System>
        void _earlyUpdate(boost::float32_t dt);

        template<typename System>
        void _lateUpdate(boost::float32_t dt);

    private:
        std::tuple<Systems...> systems_;
    };

    template<typename... Systems>
    void SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>::update(boost::float32_t dt) {
        (_earlyUpdate<Systems>(dt), ...);
        (_lateUpdate<Systems>(dt), ...);
    }

    template<typename... Systems>
    template<typename System>
    void SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>::_earlyUpdate(boost::float32_t dt) {
        std::get<system_list_t::template get_type_index<System>::value>(systems_).template earlyUpdate(dt);
    }

    template<typename... Systems>
    template<typename System>
    void SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>::_lateUpdate(boost::float32_t dt) {
        std::get<system_list_t::template get_type_index<System>::value>(systems_).template lateUpdate(dt);
    }

    template<typename... Systems>
    template<typename System>
    auto SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>::get() const
        -> enable_if_system<System, System const&>
    {
        return std::get<system_list_t::template get_type_index<System>::value>(systems_);
    }

    template<typename... Systems>
    template<typename System>
    auto SystemManager<SystemManagerConfig<easy_mp::type_list<Systems...>>>::get()
        -> enable_if_system<System, System&>
    {
        return const_cast<System&>(std::as_const(this)->template get<System>());
    }
}

#endif //ENTTX_SYSTEMMANAGER_H
