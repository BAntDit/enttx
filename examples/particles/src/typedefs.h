//
// Created by bantdit on 4/12/20.
//

#ifndef ENTTX_TYPEDEFS_H
#define ENTTX_TYPEDEFS_H

#include <boost/cstdfloat.hpp>
#include <glm/glm.hpp>

namespace examples::particles {
using real = boost::float32_t;
using uint = std::uint32_t;
using vec2 = glm::tvec2<boost::float32_t, glm::highp>;
using vec3 = glm::tvec3<boost::float32_t, glm::highp>;
using vec4 = glm::tvec4<boost::float32_t, glm::highp>;
using uvec2 = glm::tvec2<std::uint32_t, glm::highp>;
using uvec3 = glm::tvec3<std::uint32_t, glm::highp>;
using uvec4 = glm::tvec4<std::uint32_t, glm::highp>;
using quat = glm::tquat<boost::float32_t, glm::highp>;
using mat3x4 = glm::tmat3x4<boost::float32_t, glm::highp>; // 3x4 => C = 3, R = 4
using mat3 = glm::tmat3x3<boost::float32_t, glm::highp>;   // 3x3
using mat4 = glm::tmat4x4<boost::float32_t, glm::highp>;   // 4x4

#pragma pack(push, 1)
struct DrawArraysIndirectCommand
{
    uint count;
    uint instanceCount;
    uint first;
    uint baseInstance;
};
#pragma pack(pop)

}

#endif // ENTTX_TYPEDEFS_H
