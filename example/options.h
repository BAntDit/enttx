//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_OPTIONS_H
#define ENTTX_OPTIONS_H

class Options
{
public:
    explicit Options(int argc = 0, const char* argv[] = {});

    auto initialEntitiesCount() const -> size_t { return initialEntitiesCount_; }

private:
    size_t initialEntitiesCount_;
};

#endif //ENTTX_OPTIONS_H
