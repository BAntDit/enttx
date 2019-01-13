//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_DIRECTIONALLIGHT_H
#define ENTTX_DIRECTIONALLIGHT_H

#include <boost/cstdfloat.hpp>

struct Payload
{
    explicit Payload(boost::float32_t m = 0.0f) :
        mass{ m }
    { }

    boost::float32_t mass;
};

#endif //ENTTX_DIRECTIONALLIGHT_H
