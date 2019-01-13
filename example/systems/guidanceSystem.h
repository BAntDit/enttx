//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_CAMERASYSTEM_H
#define ENTTX_CAMERASYSTEM_H

#include "../../src/baseSystem.h"
#include "../components/engine.h"
#include "../components/missle.h"
#include "../updateStages.h"

class CameraSystem: public enttx::BaseSystem<CameraSystem>
{
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void CameraSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
}

#endif //ENTTX_CAMERASYSTEM_H
