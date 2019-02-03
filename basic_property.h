#pragma once

#include "property.h"

namespace property
{

template <class T>
class BasicProperty : public Property
{
public:
    using value_type = T;

    BasicProperty(const std::string& name, const value_type& value, const std::string displayName = "")
        : Property(name, displayName), value_{value}
    {
    }
    ~BasicProperty() override {}

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
    bool operator!=(const BasicProperty& rhs) const { return name_ != rhs.name_ || value_ != rhs.value_; }

    STR(out << "="; stream::convert(out, identifier) << "["; stream::convert(out, value_) << "]";)

    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    const value_type& value() const { return value_; }
    value_type& value() { return value_; }

    static std::shared_ptr<BasicProperty> convert(const Property& property)
    {
        return std::make_shared<BasicProperty>(
            property.name(), property.cast<BasicProperty>().value(), property.displayName());
    }

    std::shared_ptr<Property> clone(const std::string& name = "", const std::string& displayName = "") const override
    {
        return std::make_shared<BasicProperty>(
            name.empty() ? name_ : name, value_, displayName.empty() ? displayName_ : displayName);
    }

private:
    value_type value_;
};

using BooleanProperty = BasicProperty<bool>;
using StringProperty = BasicProperty<std::string>;
using WStringProperty = BasicProperty<std::wstring>;
}
