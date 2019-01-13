//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_TRANSFORM_H
#define ENTTX_TRANSFORM_H

#include <glm/glm.hpp>
#include <boost/cstdfloat.hpp>

struct Missile {
    explicit Missile(
        glm::tvec3<boost::float32_t, glm::highp> l = glm::tvec3<boost::float32_t, glm::highp>{0.0f},
        glm::tvec3<boost::float32_t, glm::highp> v = glm::tvec3<boost::float32_t, glm::highp>{0.0f},
        glm::tvec3<boost::float32_t, glm::highp> a = glm::tvec3<boost::float32_t, glm::highp>{0.0f},
        boost::float32_t m = 0.0f
    ) :
        location{ l }
        , velocity{ v }
        , acceleration{ a }
        , mass{ m }
    { }

    glm::tvec3<boost::float32_t, glm::highp> location;
    glm::tvec3<boost::float32_t, glm::highp> velocity;
    glm::tvec3<boost::float32_t, glm::highp> acceleration;

    boost::float32_t mass;
};

#endif //ENTTX_TRANSFORM_H
