
#include <SDL2/SDL.h>
#include <boost/scope_exit.hpp>
#include <chrono>
#include <iostream>
#include <stdexcept>

#include "../glad/glad.h"
#include "controller.h"
#include "view.h"

int main()
{
    using namespace examples;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("could no init SDL");
    }

    SDL_Window* window = nullptr;
    SDL_GLContext context = nullptr;

    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (context != nullptr)
            SDL_GL_DeleteContext(context);

        if (window != nullptr)
            SDL_DestroyWindow(window);

        SDL_Quit();
    };

    const uint32_t width = 1024;
    const uint32_t height = 1024;

    if ((window = SDL_CreateWindow("examples.particles", 100, 100, width, height, SDL_WINDOW_OPENGL)) == nullptr) {
        throw std::runtime_error("could not create window: " + std::string{ SDL_GetError() });
    }

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    if ((context = SDL_GL_CreateContext(window)) == nullptr) {
        throw std::runtime_error("could not create GL context: " + std::string{ SDL_GetError() });
    }

    if (SDL_GL_SetSwapInterval(1) < 0) {
        throw std::runtime_error("could not set swap interval: " + std::string{ SDL_GetError() });
    }

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    std::cout << glGetString(GL_VERSION) << std::endl;

    particles::View view{};
    particles::Controller controller{};

    auto start = std::chrono::high_resolution_clock::now();

    while (controller.alive()) {
        auto end = std::chrono::high_resolution_clock::now();

        auto dt = std::chrono::duration<float, std::ratio<1>>{ end - start }.count();

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN)
                controller.template onEvent<SDL_KEYDOWN>(event.key.keysym.sym);
            else if (event.type == SDL_QUIT)
                controller.template onEvent<SDL_QUIT>();
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}
