#include "basic_property.h"

namespace property
{

template <>
const std::string BooleanProperty::identifier = "bool";

template <>
const std::string StringProperty::identifier = "string";

template <>
const std::string WStringProperty::identifier = "utf8";
}
