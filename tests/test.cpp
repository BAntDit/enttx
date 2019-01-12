//
// Created by bantdit on 1/11/19.
//

#include "test.h"

void TestEntity(enttx::EntityManager<entity_manager_config_t> &entityManager)
{
    {
        auto entity = entityManager.create();

        ASSERT_TRUE(entityManager.isValid(entity));
    }

    {
        enttx::Entity entity;

        ASSERT_FALSE(entityManager.isValid(entity));
    }
}

TEST_F(TestFixture, _TestEntity) {
    TestEntity(*entities_);
}
