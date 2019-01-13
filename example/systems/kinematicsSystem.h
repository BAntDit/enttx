//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_DIRECTIONALLIGHTSYSTEM_H
#define ENTTX_DIRECTIONALLIGHTSYSTEM_H

#include "../../src/baseSystem.h"

class KinematicsSystem: public enttx::BaseSystem<KinematicsSystem>
{
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void KinematicsSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
}

#endif //ENTTX_DIRECTIONALLIGHTSYSTEM_H
