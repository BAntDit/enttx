//
// Created by bantdit on 4/12/20.
//

#ifndef ENTTX_MODEL_H
#define ENTTX_MODEL_H

#include "gl/resource.h"
#include "typedefs.h"

namespace examples::particles {
class Model
{
public:
    explicit Model(size_t maxParticlesAtOnce = 10000000);

private:
    gl::Resource<2> buffers_;
    DrawElementsIndirectCommand* commandsPtr_;
    mat3x4* instancesPtr_;
    GLuint instances_;
    GLuint commands_;
};
}

#endif // ENTTX_MODEL_H
