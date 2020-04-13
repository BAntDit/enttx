//
// Created by bantdit on 4/11/20.
//

#ifndef ENTTX_VIEW_H
#define ENTTX_VIEW_H

#include "gl/program.h"

namespace examples::particles {
class Model;

class View
{
public:
    View(uint32_t width, uint32_t height);

    void draw(Model const& model);

    void init(Model const& model);

private:
    gl::Program program_;
    uint32_t viewLocation_;
    uint32_t projectionLocation_;
};
}

#endif // ENTTX_VIEW_H
