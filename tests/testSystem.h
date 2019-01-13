//
// Created by bantdit on 1/13/19.
//

#ifndef ENTTX_TESTSYSTEM_H
#define ENTTX_TESTSYSTEM_H

#include "../src/baseSystem.h"

class TestSystem: public enttx::BaseSystem<TestSystem>
{
public:
    TestSystem() :
        product_{ 1 }
    { }

    template<typename... Args>
    void init(Args&&... args);

    template<typename EntityManager, size_t STAGE>
    void update(EntityManager* entityManager, boost::float32_t dt) {
        (void)dt;

        auto view = entityManager->template getView<uint32_t>();

        product_ = 1;

        for (auto&& entity : view) {
            uint32_t val = *(entityManager->template getComponent<uint32_t>(entity));

            product_ *= val;
        }
    }

    auto product() const -> uint32_t { return product_; }

private:
    uint32_t product_;
};

#endif //ENTTX_TESTSYSTEM_H
