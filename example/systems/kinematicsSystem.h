//
// Created by bantdit on 1/8/19.
//

#ifndef ENTTX_DIRECTIONALLIGHTSYSTEM_H
#define ENTTX_DIRECTIONALLIGHTSYSTEM_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <algorithm>

#include "../../src/baseSystem.h"
#include "../components/guidance.h"
#include "../components/missle.h"
#include "../updateStages.h"

class DirectionalLightSystem: public enttx::BaseSystem<DirectionalLightSystem>
{
public:
    void init() { }

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt);
};

template<typename EntityManager, size_t STAGE>
void DirectionalLightSystem::update(EntityManager* entityManager, boost::float32_t dt)
{
    (void)dt;

    auto entities = entityManager->template getView<Transform, DirectionalLight>();

    for(auto&& entity : entities) {
        auto&& components = entityManager->template getComponents<Transform, DirectionalLight>();

        auto& transform = *std::get<0>(components);
        auto& directionalLight = *std::get<1>(components);

        if constexpr (STAGE == UPDATE_STAGES::LATE_UPDATE_STAGE) {
            if (transform.matrixUpdate) {
                std::copy_n(
                    glm::value_ptr(glm::column(transform.matrix, 2)), 3,
                    glm::value_ptr(directionalLight.direction)
                );
            }
        }
    }
}

#endif //ENTTX_DIRECTIONALLIGHTSYSTEM_H
