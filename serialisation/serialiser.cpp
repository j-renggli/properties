#include "serialiser.h"

#include <cassert>

namespace property
{

void Serialiser::serialiseNode(Node& node, const Property& prop)
{
    auto it = serialisers_.find(prop.id());
    assert(it != serialisers_.end());
    it->second->serialise(*this, node, prop);
}
}
