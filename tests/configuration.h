//
// Created by bantdit on 1/12/19.
//

#ifndef ENTTX_CONFIGURATION_H
#define ENTTX_CONFIGURATION_H

#include "../src/config.h"
#include "../src/componentStorage.h"

using entity_manager_config_t = enttx::EntityManagerConfig<
        easy_mp::type_list<uint32_t, uint64_t>,
        easy_mp::type_list<
                enttx::ComponentStorage<16, 64, uint32_t>,
                enttx::ComponentStorage<16, 64, uint64_t>
        >
>;

#endif //ENTTX_CONFIGURATION_H
