//
// Created by bantdit on 12/11/18.
//

#include "entity.h"

namespace enttx {
Entity::Entity()
  : id_{ 0 }
{
}

Entity::Entity(uint64_t id)
  : id_{ id }
{
}

Entity::Entity(uint32_t index, uint32_t version)
  : id_{ static_cast<uint64_t>(index) | static_cast<uint64_t>(version) << 32UL }
{
}
}
