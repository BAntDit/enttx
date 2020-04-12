//
// Created by bantdit on 4/11/20.
//

#ifndef ENTTX_CONTROLLER_H
#define ENTTX_CONTROLLER_H

#include <SDL2/SDL.h>
#include <tuple>

namespace examples::particles {
class Controller
{
public:
    Controller() = default;

    template<int eventType, typename... EventArgs>
    void onEvent(EventArgs&&... args);

    [[nodiscard]] auto alive() const -> bool { return !shutdown_; }

private:
    bool shutdown_ = false;
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
}
}

#endif // ENTTX_CONTROLLER_H
