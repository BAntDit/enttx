//
// Created by bantdit on 4/12/20.
//

#include "model.h"

namespace examples::particles {
Model::Model(size_t maxParticlesAtOnce)
  : buffers_{ [](GLsizei s, GLuint* d) -> void { glDeleteBuffers(s, d); } }
  , commandsPtr_{ nullptr }
  , instancesPtr_{ nullptr }
  , instances_{ 0 }
  , commands_{ 0 }
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

    instances_ = instances;
    commands_ = commands;

    commandsPtr_->count = 6;
    commandsPtr_->instanceCount = 0;
    commandsPtr_->first = 0;
    commandsPtr_->baseInstance = 0;
};
}