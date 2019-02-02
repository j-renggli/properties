#pragma once

#include <memory>
#include <string>

namespace property
{

class Property
{
public:
    Property(const std::string& name, const std::string& displayName) : name_{name}, displayName_{displayName} {}

    virtual const std::string& id() const = 0;
    const std::string& name() const { return name_; }
    const std::string& displayName() const { return displayName_; }

    virtual std::shared_ptr<Property> clone() const = 0;
    template <class T>
    T& cast()
    {
        return dynamic_cast<T&>(*this);
    }
    template <class T>
    const T& cast() const
    {
        return dynamic_cast<const T&>(*this);
    }

protected:
    const std::string name_;
    const std::string displayName_;
};
}
