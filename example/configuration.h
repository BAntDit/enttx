//
// Created by bantdit on 1/7/19.
//

#ifndef ENTTX_CONFIGURATION_H
#define ENTTX_CONFIGURATION_H

#include "../src/config.h"
#include "../src/componentStorage.h"
#include "components/missile.h"
#include "components/engine.h"
#include "components/guidance.h"
#include "systems/engineSystem.h"
#include "systems/guidanceSystem.h"
#include "systems/kinematicsSystem.h"

using entity_manager_config_t = enttx::EntityManagerConfig<
        easy_mp::type_list<
                Engine,
                Missile,
                Guidance
            >,
        easy_mp::type_list<
                enttx::ComponentStorage<16, 128, Engine>,
                enttx::ComponentStorage<16, 128, Missile>,
                enttx::ComponentStorage<16, 128, Guidance>
            >
    >;

using system_manager_config_t = enttx::SystemManagerConfig<
        1,
        entity_manager_config_t,
        easy_mp::type_list<
                GuidanceSystem,
                EngineSystem,
                KinematicsSystem
            >
    >;

#endif //ENTTX_CONFIGURATION_H
