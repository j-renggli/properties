#pragma once

#include "properties_export.h"
#include "serialiser.h"

namespace property
{

class PROPERTIES_EXPORT JSONSerialiser : public Serialiser
{
public:
    JSONSerialiser();

    std::string serialise(const Property& prop);
};
}
