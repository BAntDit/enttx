//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_CAMERASYSTEM_H
#define ENTTX_CAMERASYSTEM_H

#include "../../src/baseSystem.h"
#include "../components/camera.h"
#include "../components/transform.h"
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
    (void)dt;

    auto entities = entityManager->template getView<Transform, Camera>();

    for(auto&& entity : entities) {
        if constexpr (STAGE == UPDATE_STAGES::EARLY_UPDATE_STAGE) {
            auto& camera = *entityManager->template getComponent<Camera>();

            if (camera.projectionUpdate) {
                camera.projectionMatrix =
                    glm::perspective(camera.yFov, camera.aspect, camera.zNear, camera.zFar);
            }
        }

        if constexpr (STAGE == UPDATE_STAGES::LATE_UPDATE_STAGE) {
            auto&& components = entityManager->template getComponents<Transform, Camera>();

            auto& transform = *std::get<0>(components);
            auto& camera = *std::get<1>(components);

            if (transform.matrixUpdate) {
                camera.viewMatrix = glm::inverse(transform.matrix);
                camera.viewProjectionMatrix = camera.projectionMatrix * transform.viewMatrix;
            }
        }
    }
}

#endif //ENTTX_CAMERASYSTEM_H
