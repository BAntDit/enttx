//
// Created by bantdit on 1/12/19.
//

#ifndef ENTTX_TEST_H
#define ENTTX_TEST_H

#include "../src/enttx.h"
#include "configuration.h"
#include <gtest/gtest.h>

class TestFixture : public testing::Test
{
public:
    TestFixture()
      : entities_{ nullptr }
      , systems_{ nullptr }
    {
    }

protected:
    void SetUp() override
    {
        entities_ = std::make_unique<enttx::EntityManager<entity_manager_config_t>>(1000);
        systems_ = std::make_unique<enttx::SystemManager<system_manager_config_t>>();
    }

    void TearDown() override
    {
        entities_.reset();
        systems_.reset();
    }

protected:
    std::unique_ptr<enttx::EntityManager<entity_manager_config_t>> entities_;
    std::unique_ptr<enttx::SystemManager<system_manager_config_t>> systems_;
};

#endif // ENTTX_TEST_H
