//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_TRANSFORMSYSTEM_H
#define ENTTX_TRANSFORMSYSTEM_H

#include "../../src/baseSystem.h"

class EngineSystem: public enttx::BaseSystem<EngineSystem> {
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void EngineSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
}

#endif //ENTTX_TRANSFORMSYSTEM_H
