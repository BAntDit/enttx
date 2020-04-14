//
// Created by bantdit on 4/11/20.
//

#ifndef ENTTX_VIEW_H
#define ENTTX_VIEW_H

#include "gl/program.h"
#include "gl/resource.h"

namespace examples::particles {
class Model;

class View
{
public:
    View(uint32_t width, uint32_t height);

    void draw(Model const& model);

private:
    gl::Resource<3> outputs_;
    gl::Resource<1> renderTarget_;
    gl::Program particlesProgram_;
    gl::Program screenProgram_;
    uint32_t color_;
    uint32_t alpha_;
    uint32_t framebuffer_;
    uint32_t viewLocation_;
    uint32_t projectionLocation_;
    uint32_t origin_;
    uint32_t colorLocation_;
    uint32_t alphaLocation_;
};
}

#endif // ENTTX_VIEW_H
