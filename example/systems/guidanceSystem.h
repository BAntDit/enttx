//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_CAMERASYSTEM_H
#define ENTTX_CAMERASYSTEM_H

#include "../../src/baseSystem.h"

class GuidanceSystem: public enttx::BaseSystem<GuidanceSystem>
{
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void GuidanceSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
}

#endif //ENTTX_CAMERASYSTEM_H
