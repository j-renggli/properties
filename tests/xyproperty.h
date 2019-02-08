#pragma once

#include <known_group_property.h>
#include <numeric_property.h>

namespace property
{

class XYProperty : public KnownGroupProperty<2>
{
public:
    XYProperty(const std::string& name, const IntProperty& x, const IntProperty& y, const std::string& displayName = "")
        : KnownGroupProperty<2>(name, {{&x_, &y_}}, displayName), x_{x}, y_{y}
    {
    }

    XYProperty& operator=(const XYProperty& rhs)
    {
        x_ = rhs.x_;
        y_ = rhs.y_;
        return *this;
    }
    bool operator==(const XYProperty& rhs) const { return !operator!=(rhs); }
    virtual bool operator!=(const XYProperty& rhs) const { return different(rhs) || x_ != rhs.x_ || y_ != rhs.y_; }

    const IntProperty& x() const { return x_; }
    const IntProperty& y() const { return y_; }

    static XYProperty convert(const Property& property)
    {
        return XYProperty(property.name(),
                          property.cast<GroupProperty>().get<IntProperty>("x"),
                          property.cast<GroupProperty>().get<IntProperty>("y"),
                          property.displayName());
    }

private:
    IntProperty x_;
    IntProperty y_;
};
}
