//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_EXAMPLE_H
#define ENTTX_EXAMPLE_H

#include "app.h"

class Example: public BaseApp<Example> {
public:
    auto init(Options const& options) -> Example& { return *this; }

    auto run() -> Example& { return *this; }

    void done() { }
};

#endif //ENTTX_EXAMPLE_H
