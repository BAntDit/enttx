//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_DIRECTIONALLIGHT_H
#define ENTTX_DIRECTIONALLIGHT_H

#include <glm/glm.hpp>
#include <boost/cstdfloat.hpp>

struct Guidance
{
    explicit Guidance(
        glm::tvec3<boost::float32_t, glm::highp> t = glm::tvec3<boost::float32_t, glm::highp>{0.0f},
        boost::float32_t m = 0.0f) :
        target{ t }
        , mass{ m }
    { }

    glm::tvec3<boost::float32_t, glm::highp> target;
    boost::float32_t mass;
};

#endif //ENTTX_DIRECTIONALLIGHT_H
