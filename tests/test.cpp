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
         auto entities = entityManager.create(std::array<enttx::Entity, 1000>{});

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        std::array<enttx::Entity, 1000> entities = {};

        entityManager.create(entities);

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        auto entities = entityManager.create(std::vector<enttx::Entity>(1000));

        for (auto&& entity : entities) {
            ASSERT_TRUE(entityManager.isValid(entity));
        }
    }

    {
        std::vector<enttx::Entity> entities(1000);

        entityManager.create(entities);

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
    auto entities = entityManager.create(std::array<enttx::Entity, 1000>{});

    for (auto&& entity : entities) {
        ASSERT_TRUE(entityManager.isValid(entity));

        entityManager.destroy(entity);

        ASSERT_FALSE(entityManager.isValid(entity));
    }
}

void componentsTest(enttx::EntityManager<entity_manager_config_t> &entityManager) {
    {
        auto entity = entityManager.create();

        entityManager.assign<uint32_t>(entity, 1);

        ASSERT_TRUE(entityManager.hasComponent<uint32_t>(entity));

        ASSERT_FALSE(entityManager.hasComponent<uint64_t>(entity));

        entityManager.remove<uint32_t>(entity);

        ASSERT_FALSE(entityManager.hasComponent<uint32_t>(entity));

        entityManager.destroy(entity);
    }

    {
        auto entities = entityManager.create(std::array<enttx::Entity, 1000>{});

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, 1);
        }
    }

    {
        auto entities = entityManager.create(std::array<enttx::Entity, 1000>{});

        for (auto&& entity : entities) {
            entityManager.assign<uint64_t>(entity, 1);
        }
    }

    {
        uint32_t sum = 0;

        auto view = entityManager.getView<uint32_t>();

        for (auto&& [entity, uint32] : view) {
            ASSERT_TRUE(entityManager.hasComponent<uint32_t>(entity));
            ASSERT_FALSE(entityManager.hasComponent<uint64_t>(entity));

            sum += uint32;
        }

        ASSERT_EQ(sum, 1000);
    }

    {
        size_t count1 = 0;
        size_t count2 = 0;

        auto view = entityManager.getView<>();

        for (auto&& [entity] : view) {
            auto [cmp1, cmp2] = entityManager.getComponents<uint32_t, uint64_t>(entity);

            count1 += cmp1 != nullptr ? 1 : 0;
            count2 += cmp2 != nullptr ? 1 : 0;
        }

        ASSERT_EQ(count1, 1000);
        ASSERT_EQ(count2, 1000);
    }

    {
        auto view = entityManager.getView<uint64_t>();

        for (auto&& [entity, uint64] : view) {
            (void)uint64;
            entityManager.remove<uint64_t>(entity);
        }

        ASSERT_EQ(view.begin(), view.end());
    }
}

void componentsStorageTest(enttx::EntityManager<entity_manager_config_t> &entityManager) {
    {
        auto entities = entityManager.create(std::array<enttx::Entity, 1000>{});

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, entity.index());
        }

        for (auto&& entity : entities) {
            if (entity.index() == 10 || entity.index() == 11 || entity.index() == 100 || entity.index() == 101) {
                entityManager.remove<uint32_t>(entity);
            }
        }
    }

    {
        auto view = entityManager.getView<uint32_t>();

        for (auto&& [entity, uint32] : view) {
            ASSERT_NE(entity.index(), 10);
            ASSERT_NE(entity.index(), 11);
            ASSERT_NE(entity.index(), 100);
            ASSERT_NE(entity.index(), 101);
            ASSERT_EQ(entity.index(), uint32);
        }
    }

    {
        auto entity1 = entityManager.create();
        auto entity2 = entityManager.create();
        auto entity3 = entityManager.create();
        auto entity4 = entityManager.create();

        entityManager.assign<uint32_t>(entity1, entity1.index());
        entityManager.assign<uint32_t>(entity2, entity2.index());
        entityManager.assign<uint32_t>(entity4, entity4.index());
        entityManager.assign<uint32_t>(entity3, entity3.index());

        auto view = entityManager.getView<uint32_t>();

        for (auto&& [entity, uint32] : view) {
            ASSERT_EQ(entity.index(), uint32);
        }
    }

    {
        auto view = entityManager.getView<uint32_t>();

        for (auto&& [entity, uint32] : view) {
            (void)uint32;
            if (entity.index() > 950) {
                entityManager.remove<uint32_t>(entity);
            }
        }

        auto entity1 = entityManager.create();
        auto entity2 = entityManager.create();
        auto entity3 = entityManager.create();
        auto entity4 = entityManager.create();

        entityManager.assign<uint32_t>(entity1, entity1.index());
        entityManager.assign<uint32_t>(entity2, entity2.index());
        entityManager.assign<uint32_t>(entity4, entity4.index());
        entityManager.assign<uint32_t>(entity3, entity3.index());
    }

    {
        auto view = entityManager.getView<uint32_t>();

        for (auto&& [entity, uint32] : view) {
            ASSERT_EQ(entity.index(), uint32);
        }
    }
}

void systemsTest(
        enttx::EntityManager<entity_manager_config_t> &entityManager,
        enttx::SystemManager<system_manager_config_t> &systemManager)
{
    uint32_t product = 1;

    {
        auto entities = entityManager.create(std::array<enttx::Entity, 4>{});

        uint32_t val = 0;

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, ++val);

            product *= val;
        }
    }

    systemManager.update();

    auto& system = systemManager.get<TestSystem>();

    ASSERT_EQ(system.product(), product);

    ASSERT_TRUE(system.is_in_use_v<uint32_t>);
    ASSERT_FALSE(system.is_in_use_v<uint64_t>);

    ASSERT_TRUE(systemManager.has_system_for_components_v<uint32_t>);
    ASSERT_FALSE(systemManager.has_system_for_components_v<uint64_t>);

    auto const& [ testSystem ] = systemManager.getSystemsForComponents<uint32_t>();

    ASSERT_TRUE(&testSystem == &system);
}

TEST_F(TestFixture, CreateEntityTest) {
    createEntityTest(*entities_);
}

TEST_F(TestFixture, DestoryEntityTest) {
    destoryEntityTest(*entities_);
}

TEST_F(TestFixture, ComponentsTest) {
    componentsTest(*entities_);
}

TEST_F(TestFixture, ComponentsStorageTest) {
    componentsStorageTest(*entities_);
}

TEST_F(TestFixture, SystemsTest) {
    systemsTest(*entities_, *systems_);
}
