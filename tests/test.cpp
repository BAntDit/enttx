//
// Created by bantdit on 1/11/19.
//

#include "test.h"

void createEntityTest(enttx::EntityManager<entity_manager_config_t> &entityManager)
{
    {
        auto entity = entityManager.create();

        ASSERT_TRUE(entityManager.isValid(entity));
    }

    {
         auto entities = entityManager.createMany(std::array<enttx::Entity, 1000>());

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        std::array<enttx::Entity, 1000> entities;

        entityManager.createMany(entities);

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        auto entities = entityManager.createMany(std::vector<enttx::Entity>(1000));

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        std::vector<enttx::Entity> entities(1000);

        entityManager.createMany(entities);

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        enttx::Entity entity;

        ASSERT_FALSE(entityManager.isValid(entity));
    }
}

void destoryEntityTest(enttx::EntityManager<entity_manager_config_t> &entityManager) {
    auto entities = entityManager.createMany(std::array<enttx::Entity, 1000>());

    for (auto&& entity : entities) {
        ASSERT_TRUE(entityManager.isValid(entity));

        entityManager.destroy(entity);

        ASSERT_FALSE(entityManager.isValid(entity));
    }
}

TEST_F(TestFixture, CreateEntityTest) {
    createEntityTest(*entities_);
}

TEST_F(TestFixture, DestoryEntityTest) {
    destoryEntityTest(*entities_);
}
