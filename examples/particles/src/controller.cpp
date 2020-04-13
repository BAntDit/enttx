//
// Created by bantdit on 4/13/20.
//

#include "controller.h"
#include "model.h"
#include "typedefs.h"

namespace examples::particles {
Controller::Controller(uint32_t width, uint32_t height, real distance, real azimuth, real polar)
  : target{ 0.f }
  , dumping{ 0.9f }
  , minDistance{ 1.f }
  , maxDistance{ 20.f }
  , width_{ width }
  , height_{ height }
  , scroll_{ 0.f }
  , distance_{ distance }
  , azimuth_{ azimuth }
  , polar_{ polar }
  , rotationStart_{}
  , rotate_{}
  , state_{ State::NONE }
  , shutdown_{ false }
{}

void Controller::update(Model& model, real dt)
{
    auto pi = boost::math::constants::pi<boost::float32_t>();

    distance_ += scroll_;
    distance_ = std::max(minDistance, std::min(maxDistance, distance_));

    scroll_ -= scroll_ * (1.0 - dumping);
    scroll_ = abs(scroll_) < 0.1e-3f ? 0.0f : scroll_;

    azimuth_ += rotate_.x * dt;
    polar_ += rotate_.y * dt;

    rotate_.x -= rotate_.x * (1.0 - dumping);
    rotate_.y -= rotate_.y * (1.0 - dumping);

    polar_ = std::max((pi / 2.0f), std::min(pi - 0.1f, polar_));

    auto pos = vec3{ distance_ * sinf(polar_) * cosf(azimuth_),
                     distance_ * cosf(polar_),
                     distance_ * sinf(azimuth_) * sinf(polar_) };

    auto up = vec3{ .0f, 1.f, .0f };
    auto fw = glm::normalize(-pos);
    auto rt = glm::normalize(glm::cross(up, fw));

    up = glm::normalize(glm::cross(fw, rt));

    model.view() = glm::inverse(mat4{ vec4{ rt.x, rt.y, rt.z, 0.0f },
                                      vec4{ up.x, up.y, up.z, 0.0f },
                                      vec4{ fw.x, fw.y, fw.z, 0.0f },
                                      vec4{ target.x - pos.x, target.y - pos.y, target.z - pos.z, 1.0f } });
}
}