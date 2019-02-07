#include "serialiser.h"

#include <cassert>

namespace property
{

std::unique_ptr<Property> Serialiser::deserialiseNode(Node& node) const
{
    auto it = serialisers_.find(node.id());
    assert(it != serialisers_.end());
    return it->second->deserialise(node);
}

void Serialiser::serialiseNode(Node& node, const Property& prop) const
{
    auto it = serialisers_.find(prop.id());
    assert(it != serialisers_.end());
    it->second->serialise(node, prop);
}
}
