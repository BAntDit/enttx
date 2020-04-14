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
  : outputs_{ [](GLsizei s, GLuint* d) -> void { glDeleteTextures(s, d); } }
  , renderTarget_{ [](GLsizei s, GLuint* d) -> void { glDeleteFramebuffers(s, d); } }
  , particlesProgram_{ std::string{ BASE_PATH } + "/src/shaders/vertex.glsl",
                       std::string{ BASE_PATH } + "/src/shaders/fragment.glsl" }
  , screenProgram_{ std::string{ BASE_PATH } + "/src/shaders/screen.vert.glsl",
                    std::string{ BASE_PATH } + "/src/shaders/screen.frag.glsl" }
  , color_{ 0 }
  , alpha_{ 0 }
  , framebuffer_{ 0 }
  , viewLocation_{ 1 }
  , projectionLocation_{ 2 }
  , origin_{ 3 }
  , colorLocation_{ 0 }
  , alphaLocation_{ 1 }
{
    auto const& [color, alpha, depth] = outputs_.alloc([=](auto&& res) -> std::array<GLuint, 3> const& {
        glGenTextures(res.size(), res.data());

        glBindTexture(GL_TEXTURE_2D, res[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, width, height);

        glBindTexture(GL_TEXTURE_2D, res[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16F, width, height);

        glBindTexture(GL_TEXTURE_2D, res[2]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);

        return res;
    });

    color_ = color;
    alpha_ = alpha;

    auto const& [target] =
      renderTarget_.alloc([color = color, alpha = alpha, depth = depth](auto&& res) -> std::array<GLuint, 1> const& {
          glGenFramebuffers(res.size(), res.data());

          glBindFramebuffer(GL_FRAMEBUFFER, res[0]);
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, alpha, 0);
          glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);

          if (auto result = glCheckFramebufferStatus(GL_FRAMEBUFFER); result != GL_FRAMEBUFFER_COMPLETE) {
              throw std::runtime_error(&"framebuffer is incomplete "[result]);
          }

          std::array<GLenum, 2> drawBuffers = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
          glDrawBuffers(drawBuffers.size(), drawBuffers.data());

          return res;
      });

    framebuffer_ = target;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glViewport(0, 0, width, height);
    glClearColor(0.188f, 0.835f, 0.784, 1.f);
    glClearDepth(1.f);

    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void View::draw(Model const& model)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    particlesProgram_.use();

    glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    glUniformMatrix4fv(viewLocation_, 1, false, glm::value_ptr(model.view()));
    glUniformMatrix4fv(projectionLocation_, 1, false, glm::value_ptr(model.projection()));
    glUniform3fv(origin_, 1, glm::value_ptr(model.origin()));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, model.instances());
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, model.instances());

    glBindVertexArray(model.vao());

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, model.commands());

    glMultiDrawArraysIndirect(GL_TRIANGLES,
                              (void*)(sizeof(DrawArraysIndirectCommand) * 0),
                              1, // tmp
                              sizeof(DrawArraysIndirectCommand));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    screenProgram_.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, alpha_);
    glUniform1i(colorLocation_, 0);
    glUniform1i(alphaLocation_, 1);

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBindVertexArray(model.vao());

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glFinish();
}
}
