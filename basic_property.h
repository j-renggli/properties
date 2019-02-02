#pragma once

#include "property.h"

namespace property
{

template <class T>
class BasicProperty : public Property
{
public:
    using value_type = T;

    BasicProperty(const value_type& value, const std::string& name, const std::string displayName = "")
        : Property(name, displayName), value_{value}
    {
    }

    BasicProperty& operator=(const BasicProperty& rhs)
    {
        value_ = rhs.value_;
        return *this;
    }

    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    const T& value() const { return value_; }
    T& value() { return value_; }

    static std::shared_ptr<BasicProperty> convert(const Property& property)
    {
        return std::make_shared<BasicProperty>(
            property.cast<BasicProperty>().value(), property.name(), property.displayName());
    }

    std::shared_ptr<Property> clone() const override
    {
        return std::make_shared<BasicProperty>(value_, name_, displayName_);
    }

private:
    T value_;
};

using BooleanProperty = BasicProperty<bool>;
using StringProperty = BasicProperty<std::string>;
using WStringProperty = BasicProperty<std::wstring>;
}
