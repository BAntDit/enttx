//
// Created by bantdit on 4/11/20.
//

#include "view.h"
#include "../glad/glad.h"
#include "model.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace examples::particles {
View::View(uint32_t width, uint32_t height)
  : program_{ std::string{ BASE_PATH } + "/src/shaders/vertex.glsl",
              std::string{ BASE_PATH } + "/src/shaders/fragment.glsl" }
  , viewLocation_{ 1 }
  , projectionLocation_{ 2 }
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    program_.use();

    glViewport(0, 0, width, height);
    glClearColor(0.188f, 0.835f, 0.784, 1.f);
    glClearDepth(1.f);
}

void View::draw(Model const& model)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(viewLocation_, 1, false, glm::value_ptr(model.view()));
    glUniformMatrix4fv(projectionLocation_, 1, false, glm::value_ptr(model.projection()));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.instances());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, model.instances());

    glBindVertexArray(model.vao());

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, model.commands());

    glMultiDrawArraysIndirect(GL_TRIANGLES,
                              (void*)(sizeof(DrawArraysIndirectCommand) * 0),
                              1, // tmp
                              sizeof(DrawArraysIndirectCommand));

    glFinish();
}
}
