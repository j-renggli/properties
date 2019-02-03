#pragma once

#include "property.h"

#include <vector>

namespace property
{

class GroupProperty : public Property
{
public:
    using value_type = std::vector<std::shared_ptr<Property>>;

    GroupProperty(const std::string& name, const value_type& children, const std::string displayName = "")
        : Property(name, displayName), children_{children}
    {
    }
    ~GroupProperty() override {}


    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    template <class T>
    const T& at(const std::string& key) const
    {
        for (const auto& child : children_) {
            if (child->name() != key)
                continue;
            return child->cast<const T&>();
        }
        throw std::out_of_range("No such key: " + key);
    }
    const value_type& children() const { return children_; }

    std::shared_ptr<Property> clone(const std::string& name = "", const std::string& displayName = "") const override
    {
        return std::make_shared<GroupProperty>(
            name.empty() ? name_ : name, children_, displayName.empty() ? displayName_ : displayName);
    }

private:
    value_type children_;
};
}
