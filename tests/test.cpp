//
// Created by bantdit on 1/11/19.
//

#include "test.h"

void CreateEntityTest(enttx::EntityManager<entity_manager_config_t> &entityManager)
{
    {
        auto entity = entityManager.create();

        ASSERT_TRUE(entityManager.isValid(entity));
    }

    {
         auto entities = entityManager.createMany(std::array<enttx::Entity, 10000>{});

        // for (auto&& entity : entities) {
        //    ASSERT_TRUE(entityManager.isValid(entity));
        // }
    }

    /* {
        auto entities = entityManager.createMany(std::vector<enttx::Entity>(100));

        for (auto&& entity : entities) {
            // ASSERT_TRUE(entityManager.isValid(entity));
        }
    } */

    {
        enttx::Entity entity;

        ASSERT_FALSE(entityManager.isValid(entity));
    }
}

TEST_F(TestFixture, _CreateEntityTest) {
    CreateEntityTest(*entities_);
}
