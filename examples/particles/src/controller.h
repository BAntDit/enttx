//
// Created by bantdit on 4/11/20.
//

#ifndef ENTTX_CONTROLLER_H
#define ENTTX_CONTROLLER_H

#include "typedefs.h"
#include <SDL2/SDL.h>
#include <boost/math/constants/constants.hpp>
#include <iostream>
#include <tuple>

namespace examples::particles {
class Model;

class Controller
{
private:
    enum class State
    {
        NONE,
        ROTATE,
        ZOOM, // ER
        PAN
    };

public:
    Controller(uint32_t width, uint32_t height, real distance, real azimuth, real polar);

    template<int eventType, typename... EventArgs>
    void onEvent(EventArgs&&... args);

    [[nodiscard]] auto alive() const -> bool { return !shutdown_; }

    void update(Model& model, real dt);

    vec3 target;
    real dumping;
    real minDistance;
    real maxDistance;

private:
    uint32_t width_;
    uint32_t height_;
    real scroll_;
    real distance_;
    real azimuth_;
    real polar_;
    vec2 rotationStart_;
    vec2 rotate_;
    State state_;
    bool shutdown_;
};

template<int eventType, typename... EventArgs>
void Controller::onEvent(EventArgs&&... args)
{
    if constexpr (eventType == SDL_QUIT) {
        shutdown_ = true;
    }

    if constexpr (eventType == SDL_KEYDOWN) {
        auto&& [key] = std::forward_as_tuple(std::forward<EventArgs>(args)...);

        if (key == SDLK_ESCAPE)
            shutdown_ = true;
    }

    if constexpr (eventType == SDL_MOUSEBUTTONDOWN) {
        auto&& [button, clicks, x, y] = std::forward_as_tuple(std::forward<EventArgs>(args)...);

        if (button == SDL_BUTTON_LEFT && clicks == 1) {
            state_ = State::ROTATE;
            rotationStart_.x = x;
            rotationStart_.y = y;
        }
    }

    if constexpr (eventType == SDL_MOUSEBUTTONUP) {
        state_ = State::NONE;
    }

    if constexpr (eventType == SDL_MOUSEMOTION) {
        auto&& [x, y] = std::forward_as_tuple(std::forward<EventArgs>(args)...);

        if (state_ == State::ROTATE) {
            auto pi = boost::math::constants::pi<boost::float32_t>();
            auto speed = 0.25f;

            rotate_.x = pi * 2.0f * speed * (static_cast<boost::float32_t>(x) - rotationStart_.x) /
                        static_cast<boost::float32_t>(width_);
            rotate_.y = pi * 2.0f * speed * (static_cast<boost::float32_t>(y) - rotationStart_.y) /
                        static_cast<boost::float32_t>(height_);

            rotationStart_.x = x;
            rotationStart_.y = y;
        }
    }

    if constexpr (eventType == SDL_MOUSEWHEEL) {
        auto&& [y] = std::forward_as_tuple(std::forward<EventArgs>(args)...);

        if (y > 0)
            scroll_ -= 0.05f;
        else if (y < 0)
            scroll_ += 0.05f;
    }
}
}

#endif // ENTTX_CONTROLLER_H
