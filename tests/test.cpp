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
        auto entities = entityManager.createMany(std::array<enttx::Entity, 1000>());

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, 1);
        }
    }

    {
        auto entities = entityManager.createMany(std::array<enttx::Entity, 1000>());

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
            entityManager.remove<uint64_t>(entity);
        }

        ASSERT_EQ(view.begin(), view.end());
    }
}

// TODO:: Extend storage test

void componentsStorageTest(enttx::EntityManager<entity_manager_config_t> &entityManager) {
    {
        auto entities = entityManager.createMany(std::array<enttx::Entity, 1000>());

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, 1);
        }
    }

    {
        auto storage = entityManager.getStorage<uint32_t>();

        ASSERT_EQ(storage.size(), 1000);

        uint32_t sum = 0;

        for (auto component : storage) {
            sum += component;
        }

        ASSERT_EQ(sum, 1000);
    }
}

void systemsTest(
        enttx::EntityManager<entity_manager_config_t> &entityManager,
        enttx::SystemManager<system_manager_config_t> &systemManager)
{
    uint32_t product = 1;

    {
        auto entities = entityManager.createMany(std::array<enttx::Entity, 4>());

        uint32_t val = 0;

        for (auto&& entity : entities) {
            entityManager.assign<uint32_t>(entity, ++val);

            product *= val;
        }
    }

    systemManager.update();

    auto& system = systemManager.get<TestSystem>();

    ASSERT_EQ(system.product(), product);
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
