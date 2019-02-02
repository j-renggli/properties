#include "numeric_property.h"

namespace property
{

template <>
const std::string IntProperty::identifier = "int";

template <>
const std::string DoubleProperty::identifier = "double";
}
