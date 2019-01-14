
# Enttx

Compile-time and header-only [Entity-Component-System](https://en.wikipedia.org/wiki/Entity_component_system) library.

## Overview

There is a lot of ECS implementations you can find in the github, at least i've seen many of them,
but didn't find something suitable for my goals. Some of them do a lot of job in runtime,
some of then are complicated for integration, some of them have tons of unecessary for me functionality and etc.
Finally i decided to create another one.
I don't pretend for best solution i just created another one solution suitable for my goals
and shared it here if somebody finds my solutions usefull for his own projects as well.

## Usage tutorial

### configuration

User application should provide specializations for following config templates:

1. EntityManagerConfig to define list of possible types of components and those storages.

```cpp
using entity_manager_config_t = enttx::EntityManagerConfig<
        easy_mp::type_list<
                Transform,
                Camera,
                DirectionalLight
            >,
        easy_mp::type_list<
                enttx::ComponentStorage<16, 128, Transform>,
                enttx::ComponentStorage<16, 128, Camera>,
                enttx::ComponentStorage<16, 128, DirectionalLight>
            >
    >;
```

2. SystemManagerConfig to define list of systems and update stages

```cpp
using system_manager_config_t = enttx::SystemManagerConfig<
        UPDATE_STAGES::STAGE_COUNT,
        entity_manager_config_t,
        easy_mp::type_list<
                CameraSystem,
                DirectionalLightSystem,
                TransformSystem
            >
    >;
```

### entities

### components

### entities and components management

EntityManager class provides all necessary functionality to manage entities and those classes

1. to create / destroy entity

```cpp
auto entity = entityManager.create();

entityManager.destroy(entity);
```
2. to test if entity valid:

```cpp
entityManager.isValid(entity);
```

3. to manage components:

```cpp
// ...
// to assign component to entity
entityManager.assign<Camera>(entity, near, far, fov, aspect);

//...
// to remove one or more components from entity
entityManager.remove<Camera, DirectionalLight>(entity);

// ...
// to get access to component of entity
entityManager.getComponent<Camera>(entity); // return pointer
// or to get access to several components of entity at once
entityManager.getComponents<Camera, Transform>(entity); // return tuple of pointers

// ...
// to test if entity has some component or components
entityManager.hasComponent<Camera>(entity); // return bool
entityManager.hasComponents<Camera, Transform>(entity); // return bitset

```

4. to get access to entities with necessary list of components:

 ```cpp
 auto view = entityManager.getView<Transform, Camera>();

 for (auto&& entity : view) {
    auto [transfrom, camera] = entityManager.getComponents<Transform, Camera>(entity);

    // do smth...
 }
 ```

 // or to get all entities that has at least one component

  ```cpp
  auto view = entityManager.getView<>();

  for (auto&& entity : view) {
      // do smth...
  }
  ```

 // or to get direct access to component's storage

 ```cpp
 auto storage = entityManager.getStorage<Camera>(); // storage of camera's components
 ```

5. to apply changes for any components of entity:

```cpp
auto fn = [](auto&& component) -> void {
    if constexpr (std::is_same_v<std::decay_t<decltype(component)>, Camera>) {
        // do this...
    }

    if constexpr (std::is_same_v<std::decay_t<decltype(component)>, DirectionalLight>) {
        // do that...
    }
};

entityManager.applyForComponents<Camera, DirectionalLight>(entity, fn);
```

 ### systems management




