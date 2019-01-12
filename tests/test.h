//
// Created by bantdit on 1/12/19.
//

#ifndef ENTTX_TEST_H
#define ENTTX_TEST_H

#include <gtest/gtest.h>
#include "../src/entityManager.h"
#include "configuration.h"

class TestFixture: public testing::Test {
public:
    TestFixture() :
        entities_{ nullptr }
    { }

protected:
    void SetUp() override {
        entities_ = std::make_unique<enttx::EntityManager<entity_manager_config_t>>(1000);
    }

    void TearDown() override {
        entities_.reset();
    }

protected:
    std::unique_ptr<enttx::EntityManager<entity_manager_config_t>> entities_;
};

#endif //ENTTX_TEST_H
