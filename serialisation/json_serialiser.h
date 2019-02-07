#pragma once

#include "properties_export.h"
#include "serialiser.h"

namespace property
{

class PROPERTIES_EXPORT JSONSerialiser : public Serialiser
{
public:
    JSONSerialiser();

    std::string serialise(const Property& prop) const;
    std::unique_ptr<Property> deserialise(const std::string& jsonString) const;
};
}
