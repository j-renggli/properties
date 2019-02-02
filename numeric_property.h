#pragma once

#include "property.h"

#include <limits>

namespace property
{

template <class A>
class NumericProperty : public Property
{
public:
    using value_type = A;
    static constexpr value_type infinity = std::numeric_limits<value_type>::infinity();

    NumericProperty(const std::string& name,
                    const value_type& value,
                    const value_type& min = -infinity,
                    const value_type& max = infinity,
                    const std::string displayName = "")
        : Property(name, displayName), value_{value}, min_{min}, max_{max}
    {
        if (value_ < min_) {
            throw std::out_of_range("Min value was not respected");
        }
        if (value_ > max_) {
            throw std::out_of_range("Max value was not respected");
        }
    }

    NumericProperty(const std::string& name, const value_type& value, const std::string displayName = "")
        : Property(name, displayName), value_{value}, min_{-infinity}, max_{infinity}
    {
    }


    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    const value_type& value() const { return value_; }
    value_type& value() { return value_; }
    const value_type& min() const { return min_; }
    value_type& min() { return min_; }
    const value_type& max() const { return max_; }
    value_type& max() { return max_; }


    std::shared_ptr<Property> clone() const override
    {
        return std::make_shared<NumericProperty>(name_, value_, min_, max_, displayName_);
    }

private:
    value_type value_;
    value_type min_;
    value_type max_;
};

using IntProperty = NumericProperty<int>;
using DoubleProperty = NumericProperty<double>;
}
