//
// Created by bantdit on 4/12/20.
//

#ifndef ENTTX_PROGRAM_H
#define ENTTX_PROGRAM_H

#include <filesystem>

namespace examples::particles::gl {
class Program
{
public:
    Program(std::filesystem::path const& vertexShaderPath, std::filesystem::path const& fragmentShaderPath);

    Program(char const* vertexShaderSrc, char const* fragmentShaderSrc);

    Program(uint32_t vertexShader, uint32_t fragmentShader);

    Program(Program const&) = delete;

    Program(Program&& program) noexcept;

    ~Program() noexcept;

    auto operator=(Program const&) -> Program& = delete;

    auto operator=(Program&& rhs) noexcept -> Program&;

    void use() const;

private:
    uint32_t program_;
};
}

#endif // ENTTX_PROGRAM_H
