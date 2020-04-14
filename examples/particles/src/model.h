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

    [[nodiscard]] auto view() const -> mat4 const& { return view_; }

    [[nodiscard]] auto view() -> mat4& { return view_; }

    [[nodiscard]] auto projection() const -> mat4 const& { return projection_; }

    [[nodiscard]] auto projection() -> mat4& { return projection_; }

    [[nodiscard]] auto origin() const -> vec3 const& { return rayOrigin_; }

    [[nodiscard]] auto origin() -> vec3& { return rayOrigin_; }

    [[nodiscard]] auto instances() const -> GLuint { return instances_; }

    [[nodiscard]] auto commands() const -> GLuint { return commands_; }

    [[nodiscard]] auto vao() const -> GLuint { return vaoId_; }

private:
    gl::Resource<2> buffers_;
    gl::Resource<1> vao_;
    DrawArraysIndirectCommand* commandsPtr_;
    mat3x4* instancesPtr_;
    GLuint vaoId_;
    GLuint instances_;
    GLuint commands_;
    mat4 view_;
    mat4 projection_;
    vec3 rayOrigin_;
};
}

#endif // ENTTX_MODEL_H
