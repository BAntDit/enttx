//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_EXAMPLE_H
#define ENTTX_EXAMPLE_H

#include <memory>
#include "app.h"
#include "configuration.h"
#include "../src/entityManager.h"
#include "../src/systemManager.h"

class Example: public BaseApp<Example> {
public:
    Example();

    auto init(Options const& options) -> Example&;

    auto run() -> Example& { return *this; }

    void done() { }

private:
    std::unique_ptr<enttx::EntityManager<entity_manager_config_t>> entities_;
    std::unique_ptr<enttx::SystemManager<system_manager_config_t>> systems_;
};

#endif //ENTTX_EXAMPLE_H
