//
// Created by bantdit on 1/12/19.
//

#ifndef ENTTX_CONFIGURATION_H
#define ENTTX_CONFIGURATION_H

#include "../src/config.h"
#include "../src/componentStorage.h"
#include "testSystem.h"

using entity_manager_config_t = enttx::EntityManagerConfig<
        easy_mp::type_list<uint32_t, uint64_t>,
        easy_mp::type_list<
                enttx::ComponentStorage<16, 64, uint32_t>,
                enttx::ComponentStorage<16, 64, uint64_t>
        >
>;

using system_manager_config_t = enttx::SystemManagerConfig<
        1,
        entity_manager_config_t,
        easy_mp::type_list<TestSystem>
>;

#endif //ENTTX_CONFIGURATION_H
