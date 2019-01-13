//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_CAMERA_H
#define ENTTX_CAMERA_H

#include <boost/cstdfloat.hpp>

struct Engine {
    explicit Engine(
        boost::float32_t _fuel = 0.0f,
        boost::float32_t _mass = 0.0f,
        boost::float32_t _force = 0.0f,
        boost::float32_t _lifetime = 0.0f) :
        fuel{ _fuel }
        , mass{ _mass }
        , force{ _force }
        , lifetime{ _lifetime }
    { }

    boost::float32_t fuel;
    boost::float32_t mass;
    boost::float32_t force;
    boost::float32_t lifetime;
};

#endif //ENTTX_CAMERA_H
