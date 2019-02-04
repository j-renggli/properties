#pragma once

#include "property.h"

#include <limits>

namespace property
{

template <class T>
class NumericProperty : public Property
{
public:
    using value_type = T;
    static constexpr value_type has_inf = std::numeric_limits<value_type>::has_infinity;
    static constexpr value_type max_value =
        has_inf ? std::numeric_limits<value_type>::infinity() : std::numeric_limits<value_type>::max();

    NumericProperty(const std::string& name,
                    const value_type& value,
                    const value_type& min = value_type(-max_value),
                    const value_type& max = value_type(max_value),
                    const std::string displayName = "")
        : Property(name, displayName), value_{value}, min_{min}, max_{max}
    {
        if (min_ > max_) {
            throw std::out_of_range("Min " + std::to_string(min_) + " must be smaller or equal to max " +
                                    std::to_string(max_));
        }
        if (value_ < min_) {
            throw std::out_of_range("Min value " + std::to_string(min_) + " was not respected: " +
                                    std::to_string(value_));
        }
        if (value_ > max_) {
            throw std::out_of_range("Max value " + std::to_string(max_) + " was not respected: " +
                                    std::to_string(value_));
        }
    }

    NumericProperty(const std::string& name, const value_type& value, const std::string displayName)
        : NumericProperty(name, value, value_type(-max_value), value_type(max_value), displayName)
    {
    }

    ~NumericProperty() override {}

public:
    NumericProperty& operator=(const NumericProperty& rhs)
    {
        value_ = rhs.value_;
        min_ = rhs.min_;
        max_ = rhs.max_;
        return *this;
    }
    NumericProperty& operator=(const value_type& value)
    {
        if (value < min_) {
            throw std::out_of_range("Min value was not respected");
        }
        if (value > max_) {
            throw std::out_of_range("Max value was not respected");
        }
        value_ = value;
        return *this;
    }
    bool operator==(const NumericProperty& rhs) const { return !operator!=(rhs); }
    bool operator!=(const NumericProperty& rhs) const
    {
        return different(rhs) || value_ != rhs.value_ || min_ != rhs.min_ || max_ != rhs.max_;
    }

    STR(out << "="; stream::convert(out, identifier) << "["; stream::convert(out, value_) << "]";)

public:
    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    const value_type& value() const { return value_; }
    const value_type& min() const { return min_; }
    const value_type& max() const { return max_; }

    static NumericProperty convert(const Property& property)
    {
        const NumericProperty& cast = property.cast<NumericProperty>();
        return NumericProperty(property.name(), cast.value(), cast.min(), cast.max(), property.displayName());
    }

private:
    value_type value_;
    value_type min_;
    value_type max_;
};

using IntProperty = NumericProperty<int>;
using DoubleProperty = NumericProperty<double>;
}
