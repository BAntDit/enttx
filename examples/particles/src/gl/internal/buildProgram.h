//
// Created by bantdit on 4/12/20.
//

#ifndef ENTTX_BUILD_PROGRAM_JS_H
#define ENTTX_BUILD_PROGRAM_JS_H

#include "../../../glad/glad.h"
#include <boost/scope_exit.hpp>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace examples::particles::gl {
namespace {
GLuint _buildProgram(GLuint vertexShader, GLuint fragmentShader)
{
    auto program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    {
        int success = 0;

        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (!success) {
            char log[512];

            glGetProgramInfoLog(program, 512, nullptr, log);

            throw std::runtime_error(std::string{ "program linking error: " } + log);
        }
    }

    return program;
}

GLuint _buildProgram(char const* vertexShaderSrc, char const* fragmentShaderSrc)
{
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    BOOST_SCOPE_EXIT_ALL(&)
    {
        if (vertexShader != 0)
            glDeleteShader(vertexShader);

        if (fragmentShader != 0)
            glDeleteShader(fragmentShader);
    };

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    {
        glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(vertexShader);

        int success = 0;

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char log[512];

            glGetShaderInfoLog(vertexShader, 512, nullptr, log);

            throw std::runtime_error(std::string{ "vertex shader compilation error: " } + log);
        }
    }

    {
        glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
        glCompileShader(fragmentShader);

        int success = 0;

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char log[512];

            glGetShaderInfoLog(fragmentShader, 512, nullptr, log);

            throw std::runtime_error(std::string{ "fragment shader compilation error: " } + log);
        }
    }

    return _buildProgram(vertexShader, fragmentShader);
}

GLuint _buildProgram(std::filesystem::path const& vertexShaderPath, std::filesystem::path const& fragmentShaderPath)
{
    std::string vertexSrc;
    std::string fragmentSrc;

    {
        std::ifstream file;
        std::stringstream ss;

        file.exceptions(std::ios::failbit);
        file.open(vertexShaderPath.string());
        file.exceptions(std::ios::badbit);

        ss << file.rdbuf();

        file.close();

        vertexSrc = ss.str();
    }

    {
        std::ifstream file;
        std::stringstream ss;

        file.exceptions(std::ios::failbit);
        file.open(fragmentShaderPath.string());
        file.exceptions(std::ios::badbit);

        ss << file.rdbuf();

        file.close();

        fragmentSrc = ss.str();
    }

    return _buildProgram(vertexSrc.c_str(), fragmentSrc.c_str());
}
}
}

#endif // ENTTX_BUILD_PROGRAM_JS_H
