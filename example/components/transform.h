//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_TRANSFORM_H
#define ENTTX_TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <boost/cstdfloat.hpp>

struct Transform {
    glm::tvec3<boost::float32_t, glm::highp>    position;
    glm::tvec3<boost::float32_t, glm::highp>    scale;
    glm::tquat<boost::float32_t, glm::highp>    orientation;
    glm::tmat4x4<boost::float32_t, glm::highp>  matrix;
    glm::tmat4x4<boost::float32_t, glm::highp>  worldMatrix;
};

#endif //ENTTX_TRANSFORM_H
