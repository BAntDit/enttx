//
// Created by bantdit on 12/15/18.
//

#ifndef ENTTX_CONFIG_H
#define ENTTX_CONFIG_H

#include <bitset>
#include <easy-mp/type_list.h>

namespace enttx {
template<typename ComponentList, typename StorageList>
struct EntityManagerConfig;

template<typename... Components, typename... Storages>
struct EntityManagerConfig<easy_mp::type_list<Components...>, easy_mp::type_list<Storages...>>
{
    static_assert(sizeof...(Components) == sizeof...(Storages));

    using component_list_t = easy_mp::type_list<Components...>;

    using storage_list_t = easy_mp::type_list<Storages...>;

    using component_mask_t = std::bitset<component_list_t::size>;
};

template<typename SystemList>
struct SystemManagerConfig;

template<typename... Systems>
struct SystemManagerConfig<easy_mp::type_list<Systems...>>
{
    static constexpr size_t systemCount = sizeof...(Systems);

    using system_list_t = easy_mp::type_list<Systems...>;
};
}

#endif // ENTTX_CONFIG_H
