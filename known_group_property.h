#pragma once

#include "group_property.h"

namespace property
{

template <size_t N>
class KnownGroupProperty : public GroupProperty
{
public:
    using Children = std::array<const Property*, N>;
    KnownGroupProperty(const std::string& name, const Children& children, const std::string& displayName = "")
        : GroupProperty(name, displayName), children_{children}
    {
    }
    ~KnownGroupProperty() override {}

    GroupPropertyIterator begin() const override { return GroupPropertyIterator(children_, true); }
    GroupPropertyIterator end() const override { return GroupPropertyIterator(children_, false); }
    size_t size() const override { return N; }

private:
    const Children children_;
};
}
