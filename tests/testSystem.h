//
// Created by bantdit on 1/13/19.
//

#ifndef ENTTX_TESTSYSTEM_H
#define ENTTX_TESTSYSTEM_H

#include "../src/enttx.h"

class TestSystem: public enttx::BaseSystem<TestSystem>
{
public:
    using tag_t = easy_mp::type_list<uint32_t>;

    TestSystem() :
        product_{ 1 }
    { }

    template<typename... Args>
    void init(Args&&... args);

    template<typename SystemManager,  typename EntityManager, size_t STAGE>
    void update(SystemManager&, EntityManager& entityManager) {
        auto view = entityManager.template getView<uint32_t>();

        product_ = 1;

        for (auto&& [entity, component] : view) {
            (void)entity;

            product_ *= component;
        }
    }

    auto product() const -> uint32_t { return product_; }

private:
    uint32_t product_;
};

#endif //ENTTX_TESTSYSTEM_H
