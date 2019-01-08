//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_TRANSFORMSYSTEM_H
#define ENTTX_TRANSFORMSYSTEM_H

#include "../../src/baseSystem.h"
#include "../components/transform.h"
#include "../updateStages.h"

class TransformSystem: public enttx::BaseSystem<TransformSystem> {
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void TransformSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
    (void)dt;

    auto entities = entityManager->template getView<Transform>();

    for(auto&& entity : entities) {
        auto& transform = *entityManager->template getComponent<Transform>();

        if constexpr (STAGE == UPDATE_STAGES::EARLY_UPDATE_STAGE) {
            if (transform.matrixUpdate) {
                transform.matrix =
                        glm::translate(transform.position) *
                        glm::toMat4(transform.orientation) *
                        glm::scale(transform.scale); // TRS
            }
        }

        if constexpr (STAGE == UPDATE_STAGES::LATE_UPDATE_STAGE) {
            transform.matrixUpdate = false;
        }
    }
}

#endif //ENTTX_TRANSFORMSYSTEM_H
