//
// Created by bantdit on 4/12/20.
//

#include "model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace examples::particles {
Model::Model(size_t maxParticlesAtOnce)
  : buffers_{ [](GLsizei s, GLuint* d) -> void { glDeleteBuffers(s, d); } }
  , vao_{ [](GLsizei s, GLuint* d) -> void { glDeleteVertexArrays(s, d); } }
  , commandsPtr_{ nullptr }
  , instancesPtr_{ nullptr }
  , vaoId_{ 0 }
  , instances_{ 0 }
  , commands_{ 0 }
  , view_{ 1.f }
  , projection_{ 1.f }
  , rayOrigin_{}
  , time_{ 0.f }
{
    auto const& [instances, commands] = buffers_.alloc([=, this](auto&& res) -> std::array<GLuint, 2> const& {
        glGenBuffers(res.size(), res.data());

        // 0 - instance data
        {
            auto size = sizeof(mat3x4) * maxParticlesAtOnce;

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, res[0]);

            glBufferStorage(
              GL_SHADER_STORAGE_BUFFER, size, nullptr, GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);

            instancesPtr_ = reinterpret_cast<mat3x4*>(glMapBufferRange(
              GL_SHADER_STORAGE_BUFFER, 0, size, GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT));
        }

        // 1 - commands
        {
            auto size = sizeof(DrawArraysIndirectCommand);

            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, res[1]);
            glBufferStorage(
              GL_DRAW_INDIRECT_BUFFER, size, nullptr, GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT);

            commandsPtr_ = reinterpret_cast<DrawArraysIndirectCommand*>(glMapBufferRange(
              GL_DRAW_INDIRECT_BUFFER, 0, size, GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT | GL_MAP_WRITE_BIT));
        }

        return res;
    });

    auto const& [vao] = vao_.alloc([](auto&& res) -> std::array<GLuint, 1> const& {
        glGenVertexArrays(res.size(), res.data());
        return res;
    });

    vaoId_ = vao;

    instances_ = instances;
    commands_ = commands;

    commandsPtr_->count = 6;
    commandsPtr_->instanceCount = 0;
    commandsPtr_->first = 0;
    commandsPtr_->baseInstance = 0;

    projection_ = glm::perspective(0.78539f, 1.0f, 0.1f, 100.0f);

    // tmp::
    {
        auto xfrm = glm::transpose(mat4{ vec4{ 1.f, 0.f, 0.f, 0.f },
                                         vec4{ 0.f, 1.f, 0.f, 0.f },
                                         vec4{ 0.f, 0.f, 1.f, 0.f },
                                         vec4{ -1.f, 1.f, -0.5f, 1.f } });

        auto src = glm::make_mat3x4(glm::value_ptr(xfrm));

        std::copy_n(&src, 1, instancesPtr_);
    }

    {
        auto xfrm = glm::transpose(mat4{ vec4{ 1.f, 0.f, 0.f, 0.f },
                                         vec4{ 0.f, 1.f, 0.f, 0.f },
                                         vec4{ 0.f, 0.f, 1.f, 0.f },
                                         vec4{ 1.f, -1.f, 0.5f, 1.f } });

        auto src = glm::make_mat3x4(glm::value_ptr(xfrm));

        std::copy_n(&src, 1, instancesPtr_ + 1);
    }

    commandsPtr_->instanceCount = 2;
}
}