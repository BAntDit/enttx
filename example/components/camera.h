//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_CAMERA_H
#define ENTTX_CAMERA_H

#include <glm/glm.hpp>
#include <boost/cstdfloat.hpp>

struct Camera {
    boost::float32_t zFar;
    boost::float32_t zNear;
    boost::float32_t yFov;
    boost::float32_t aspect;

    glm::tmat4x4<boost::float32_t, glm::highp>  viewMatrix;
    glm::tmat4x4<boost::float32_t, glm::highp>  projectionMatrix;
    glm::tmat4x4<boost::float32_t, glm::highp>  viewProjectionMatrix;
};

#endif //ENTTX_CAMERA_H
