//
// Created by bantdit on 4/13/20.
//

#include "controller.h"
#include "model.h"
#include "typedefs.h"

namespace examples::particles {
Controller::Controller()
  : shutdown_{ false }
{}

void Controller::update(Model& model, real dt)
{
    model.view() = glm::inverse(mat4{ vec4{ 1.0f, 0.0f, 0.0f, 0.0f },
                                      vec4{ 0.0f, 1.0f, 0.0f, 0.0f },
                                      vec4{ 0.0f, 0.0f, 1.0f, 0.0f },
                                      vec4{ 0.0f, 0.0f, 4.5f, 1.0f } });
}
}