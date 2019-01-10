//
// Created by bantdit on 1/8/19.
//

#include "example.h"

Example::Example() :
    entities_{ nullptr }
    , systems_{ nullptr }
{
}

auto Example::init(Options const& options) -> Example&
{
    entities_ = std::make_unique<enttx::EntityManager<entity_manager_config_t>>(options.initialEntitiesCount());
    systems_ = std::make_unique<enttx::SystemManager<system_manager_config_t>>(entities_.get());

    systems_->get<CameraSystem>().init();
    systems_->get<DirectionalLightSystem>().init();
    systems_->get<TransformSystem>().init();

    return *this;
}

auto Example::run() -> Example& {
    return *this;
}