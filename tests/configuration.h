//
// Created by bantdit on 1/12/19.
//

#ifndef ENTTX_CONFIGURATION_H
#define ENTTX_CONFIGURATION_H

#include "../src/enttx.h"
#include "testSystem.h"

using entity_manager_config_t = enttx::EntityManagerConfig<
  metrix::type_list<uint32_t, uint64_t>,
  metrix::type_list<enttx::ComponentStorage<16, 64, uint32_t>, enttx::ComponentStorage<16, 64, uint64_t>>>;

using system_manager_config_t = enttx::SystemManagerConfig<1, metrix::type_list<TestSystem>>;

#endif // ENTTX_CONFIGURATION_H
