//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_DIRECTIONALLIGHT_H
#define ENTTX_DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <boost/cstdfloat.hpp>

struct DirectionalLight
{
    boost::float32_t temperature;
    boost::float32_t illuminance;
    glm::tvec3<boost::float32_t, glm::highp> direction;
};

#endif //ENTTX_DIRECTIONALLIGHT_H
