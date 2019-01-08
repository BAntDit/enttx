//
// Created by bantdit on 1/7/19.
//

#ifndef ENTTX_CONFIGURATION_H
#define ENTTX_CONFIGURATION_H

#include "../src/config.h"
#include "../src/componentStorage.h"
#include "components/transform.h"
#include "components/camera.h"
#include "components/directionalLight.h"
#include "updateStages.h"

using entity_manager_config_t = enttx::EntityManagerConfig<
        easy_mp::type_list<
                Transform,
                Camera,
                DirectionalLight
            >,
        easy_mp::type_list<
                enttx::ComponentStorage<16, 128, Transform>,
                enttx::ComponentStorage<16, 128, Camera>,
                enttx::ComponentStorage<16, 128, DirectionalLight>
            >
    >;

#endif //ENTTX_CONFIGURATION_H
