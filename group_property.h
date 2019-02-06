#pragma once

#include "properties_export.h"
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

class PROPERTIES_EXPORT GroupProperty : public Property
{
public:
    GroupProperty(const std::string& name, const std::string displayName = "") : Property(name, displayName) {}
    ~GroupProperty() override {}

    STR(out << "="; stream::convert(out, identifier) << "["; auto it = begin(); if (it != end()) it->str(out);
        while (++it != end()) {
            out << ",";
            it->str(out);
        };
        out << "]";)

    static const std::string identifier;
    const std::string& id() const override { return identifier; }

    virtual GroupPropertyIterator begin() const = 0;
    virtual GroupPropertyIterator find(const std::string& name) const
    {
        for (auto it = begin(); it != end(); ++it) {
            if (it->name() == name)
                return it;
        }
        return end();
    }
    virtual GroupPropertyIterator end() const = 0;
    virtual size_t size() const = 0;

    template <class T>
    const T& get(const std::string& name) const
    {
        auto it = find(name);
        if (it != end())
            return it->cast<T>();
        throw std::out_of_range("No child with name: " + name);
    }
};
}
