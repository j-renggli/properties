#pragma once

#include <basic_property.h>
#include <known_group_property.h>

namespace property
{

class Bool2Property : public KnownGroupProperty<2>
{
public:
    Bool2Property(const std::string& name,
                  const BooleanProperty& a,
                  const BooleanProperty& b,
                  const std::string& displayName = "")
        : KnownGroupProperty<2>(name, {{&a_, &b_}}, displayName), a_{"a", a, "MyA"}, b_{"b", b, "MyB"}
    {
    }

    Bool2Property& operator=(const Bool2Property& rhs)
    {
        a_ = rhs.a_;
        b_ = rhs.b_;
        return *this;
    }
    bool operator==(const Bool2Property& rhs) const { return !operator!=(rhs); }
    virtual bool operator!=(const Bool2Property& rhs) const { return different(rhs) || a_ != rhs.a_ || b_ != rhs.b_; }

    const BooleanProperty& a() const { return a_; }
    const BooleanProperty& b() const { return b_; }

    static Bool2Property convert(const Property& property)
    {
        return Bool2Property(property.name(),
                             property.cast<GroupProperty>().get<BooleanProperty>("a"),
                             property.cast<GroupProperty>().get<BooleanProperty>("b"),
                             property.displayName());
    }

private:
    BooleanProperty a_;
    BooleanProperty b_;
};
}
