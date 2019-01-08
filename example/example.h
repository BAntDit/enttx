//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_EXAMPLE_H
#define ENTTX_EXAMPLE_H

#include <memory>
#include "app.h"
#include "configuration.h"
#include "../src/entityManager.h"

class Example: public BaseApp<Example> {
public:
    auto init(Options const& options) -> Example& { return *this; }

    auto run() -> Example& { return *this; }

    void done() { }

private:
    std::unique_ptr<enttx::EntityManager<entity_manager_config_t >> entities_;
};

#endif //ENTTX_EXAMPLE_H
