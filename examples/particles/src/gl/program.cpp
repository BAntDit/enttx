//
// Created by bantdit on 4/12/20.
//

#include "program.h"
#include "internal/buildProgram.h"
#include <cassert>

namespace examples::particles::gl {
Program::Program(std::filesystem::path const& vertexShaderPath, std::filesystem::path const& fragmentShaderPath)
  : program_{ _buildProgram(vertexShaderPath, fragmentShaderPath) }
{}

Program::Program(char const* vertexShaderSrc, char const* fragmentShaderSrc)
  : program_{ _buildProgram(vertexShaderSrc, fragmentShaderSrc) }
{}

Program::Program(uint32_t vertexShader, uint32_t fragmentShader)
  : program_{ _buildProgram(vertexShader, fragmentShader) }
{}

Program::Program(Program&& program) noexcept
  : program_{ program.program_ }
{
    program.program_ = 0;
}

Program::~Program() noexcept
{
    if (program_ != 0)
        glDeleteProgram(program_);
}

auto Program::operator=(Program&& rhs) noexcept -> Program&
{
    program_ = rhs.program_;
    rhs.program_ = 0;

    return *this;
}

void Program::use() const
{
    assert(program_ != 0);
    glUseProgram(program_);
}
}