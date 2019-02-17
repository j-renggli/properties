#pragma once

#include "../numeric_property.h"

#include <chrono>

namespace property
{

class PROPERTIES_EXPORT TimeProperty : public DoubleProperty
{
public:
    using value_type = std::chrono::duration<double>;
    static constexpr value_type max_value = value_type(std::numeric_limits<double>::infinity());

    TimeProperty(const std::string& name,
                 const value_type& value,
                 const value_type& min = value_type(-max_value),
                 const value_type& max = value_type(max_value),
                 const std::string& displayName = "");

    TimeProperty(const std::string& name, const value_type& value, const std::string displayName);

    ~TimeProperty() override;

public:
    TimeProperty& operator=(const value_type& value);

    STR(out << "="; stream::convert(out, identifier) << "["; stream::convert(out, DoubleProperty::value()) << "s]";)

public:
    const value_type value() const;
    const value_type min() const;
    const value_type max() const;

    static TimeProperty convert(const Property& property);
};
}
