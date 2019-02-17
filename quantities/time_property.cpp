#include "time_property.h"

namespace property
{

const TimeProperty::value_type TimeProperty::max_value;

TimeProperty::TimeProperty(const std::string& name,
                           const value_type& value,
                           const value_type& min,
                           const value_type& max,
                           const std::string& displayName)
    : DoubleProperty(name, value.count(), min.count(), max.count(), displayName)
{
}

TimeProperty::TimeProperty(const std::string& name, const value_type& value, const std::string displayName)
    : DoubleProperty(name, value.count(), displayName)
{
}

TimeProperty::~TimeProperty() = default;

TimeProperty TimeProperty::convert(const Property& property)
{
    const TimeProperty& cast = property.cast<TimeProperty>();
    return TimeProperty(property.name(), cast.value(), cast.min(), cast.max(), property.displayName());
}

TimeProperty& TimeProperty::operator=(const value_type& value)
{
    DoubleProperty::operator=(value.count());
    return *this;
}

const TimeProperty::value_type TimeProperty::value() const
{
    return value_type(DoubleProperty::value());
}

const TimeProperty::value_type TimeProperty::min() const
{
    return value_type(DoubleProperty::min());
}

const TimeProperty::value_type TimeProperty::max() const
{
    return value_type(DoubleProperty::max());
}
}
