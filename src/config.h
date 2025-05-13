//
// Created by bantdit on 12/15/18.
//

#ifndef ENTTX_CONFIG_H
#define ENTTX_CONFIG_H

#include <bitset>
#include <metrix/type_list.h>

namespace enttx {
template<typename ComponentList, typename StorageList>
struct EntityManagerConfig;

template<typename... Components, typename... Storages>
struct EntityManagerConfig<metrix::type_list<Components...>, metrix::type_list<Storages...>>
{
    static_assert(sizeof...(Components) == sizeof...(Storages));

    static_assert((std::is_default_constructible_v<Components> && ...));

    static_assert((std::is_default_constructible_v<Storages> && ...));

    using component_list_t = metrix::type_list<Components...>;

    using storage_list_t = metrix::type_list<Storages...>;

    using component_mask_t = std::bitset<component_list_t::size>;
};

template<size_t UPDATE_STAGE_COUNT, typename SystemList>
struct SystemManagerConfig;

template<size_t UPDATE_STAGE_COUNT, typename... Systems>
struct SystemManagerConfig<UPDATE_STAGE_COUNT, metrix::type_list<Systems...>>
{
    static_assert((std::is_default_constructible_v<Systems> && ...));

    static constexpr size_t updateStageCount = UPDATE_STAGE_COUNT;

    static constexpr size_t systemCount = sizeof...(Systems);

    using system_list_t = metrix::type_list<Systems...>;
};
}

#endif // ENTTX_CONFIG_H
