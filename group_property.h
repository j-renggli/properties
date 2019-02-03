#pragma once

#include "property.h"

#include <array>

namespace property
{

class GroupPropertyIterator
{
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = const Property;
    using difference_type = std::ptrdiff_t;
    using pointer = const Property*;
    using reference = const Property&;

    // Construction by GroupProperty
    template <size_t N>
    GroupPropertyIterator(const std::array<const Property*, N>& props, bool begin)
        : props_{props.data()}, pos_{begin ? 0 : N}, size_{N}
    {
    }
    // Copy-construction
    GroupPropertyIterator(const GroupPropertyIterator& rhs) = default;
    GroupPropertyIterator(const Property* const* props, size_t pos, size_t size) : props_{props}, pos_{pos}, size_{size}
    {
    }
    ~GroupPropertyIterator() = default;

    // Copy-assignment
    GroupPropertyIterator& operator=(const GroupPropertyIterator& rhs) = default;
    GroupPropertyIterator& operator++()
    {
        ++pos_;
        return *this;
    }
    GroupPropertyIterator operator++(int) { return GroupPropertyIterator(props_, pos_ + 1, size_); }
    bool operator==(const GroupPropertyIterator& rhs) const { return !operator!=(rhs); }
    bool operator!=(const GroupPropertyIterator& rhs) const { return pos_ != rhs.pos_; }
    reference operator*() const { return *props_[pos_]; }
    pointer operator->() const { return props_[pos_]; }

private:
    const Property* const* props_;
    size_t pos_;
    size_t size_;
};

class GroupProperty : public Property
{
public:
    GroupProperty(const std::string& name, const std::string displayName = "") : Property(name, displayName) {}
    ~GroupProperty() override {}


    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    virtual GroupPropertyIterator begin() = 0;
    virtual GroupPropertyIterator end() = 0;


};
}
