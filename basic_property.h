#pragma once

#include "property.h"

namespace property
{

template <class T>
class BasicProperty : public Property
{
public:
    using value_type = T;

    /// Constructor for a basic property
    BasicProperty(const std::string& name, const value_type& value, const std::string displayName = "")
        : Property(name, displayName), value_{value}
    {
    }
    ~BasicProperty() override {}

    /// Copy-operators: do not modify the name
    BasicProperty& operator=(const BasicProperty& rhs)
    {
        value_ = rhs.value_;
        return *this;
    }
    BasicProperty& operator=(const value_type& value)
    {
        value_ = value;
        return *this;
    }
    bool operator==(const BasicProperty& rhs) const { return !operator!=(rhs); }
    bool operator!=(const BasicProperty& rhs) const { return different(rhs) || value_ != rhs.value_; }

    STR(out << "="; stream::convert(out, identifier) << "["; stream::convert(out, value_) << "]";)

    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    const value_type& value() const { return value_; }

    static BasicProperty convert(const Property& property)
    {
        return BasicProperty(property.name(), property.cast<BasicProperty>().value(), property.displayName());
    }

private:
    value_type value_;
};

using BooleanProperty = BasicProperty<bool>;
using StringProperty = BasicProperty<std::string>;
using WStringProperty = BasicProperty<std::wstring>;
}
