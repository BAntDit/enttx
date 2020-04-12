//
// Created by bantdit on 4/12/20.
//

#ifndef ENTTX_RESOURCE_H
#define ENTTX_RESOURCE_H

#include "../../glad/glad.h"
#include <array>
#include <cstddef>
#include <functional>

namespace examples::particles::gl {
template<GLsizei size>
class Resource
{
public:
    explicit Resource(void (*deleter)(GLsizei, GLuint*), std::array<GLuint, size>&& res = {})
      : res_{ res }
      , deleter_{ deleter }
      , valid_{ false }
    {}

    Resource(Resource const&) = delete;

    Resource(Resource&& resource) noexcept
      : res_(resource.res_)
      , deleter_{ std::move(resource.deleter_) }
      , valid_{ resource.valid_ }
    {
        resource.valid_ = false;
    }

    ~Resource() noexcept { reset(); }

    auto operator=(Resource const&) -> Resource& = delete;

    auto operator=(Resource&& rhs) noexcept -> Resource&
    {
        reset();

        res_ = rhs.res_;
        deleter_ = std::move(rhs.deleter_);
        valid_ = rhs.valid_;

        rhs.valid_ = false;
    }

    explicit operator bool() const { return valid_; }

    [[nodiscard]] auto operator&() const -> GLuint const* { return res_.data(); }

    void reset()
    {
        if (valid_) {
            deleter_(res_.size(), res_.data());
            valid_ = false;
        }
    }

    template<typename Allocator>
    auto alloc(Allocator&& allocator)
      -> std::enable_if_t<std::is_invocable_v<Allocator, std::array<GLuint, size>&>, std::array<GLuint, size> const&>
    {
        valid_ = true;
        return allocator(res_);
    }

private:
    std::array<GLuint, size> res_;
    std::function<void(GLsizei, GLuint*)> deleter_;
    bool valid_;
};
}

#endif // ENTTX_RESOURCE_H
