#include <catch2/catch.hpp>

#include <group_property.h>
#include <numeric_property.h>

namespace property
{

template <class T>
void toVector(GroupProperty::value_type& vec, const T& value)
{
    vec.push_back(value.clone());
}

template <class First, class Second, class... Rest>
void toVector(GroupProperty::value_type& vec, const First& first, const Second& second, const Rest&... rest)
{
    vec.push_back(first.clone());
    toVector(vec, second, rest...);
}

template <class T>
GroupProperty::value_type toVector(const T& value)
{
    return GroupProperty::value_type(1, value.clone());
}

template <class First, class Second, class... Rest>
GroupProperty::value_type toVector(const First& first, const Second& second, const Rest&... rest)
{
    GroupProperty::value_type vec;
    toVector(vec, first, second, rest...);
    return vec;
}

template <class T>
void compareChildren(GroupProperty::value_type left, const T& value)
{
    REQUIRE(left.size() == 1);
    CHECK(left[0]->cast<T>() == value);
}
template <class First, class Second, class... Rest>
void compareChildren(GroupProperty::value_type left, const First& first, const Second& second, const Rest&... rest)
{
    REQUIRE(left.size() >= 2);
    CHECK(left[0]->cast<First>() == first);
    left.erase(left.begin());
    compareChildren(left, second, rest...);
}

template <class T, class... Params>
void checkGroupProperty(const T& prop, const Params&... children)
{
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
    compareChildren(prop.children(), children...);
}

template <class T, class... Params>
void testGroupPropertyConstructor(const Params&... children)
{
    INFO("GroupProperty constructor");
    T prop("name", children..., "display");
    checkGroupProperty(prop, children...);
}
template <class T, class... Params>
void testGroupProperty(const Params&... base, const Params&... other)
{
    {
        INFO("Constructors and related");
        testGroupPropertyConstructor<T>(base...);

    }
}

class XYProperty : public GroupProperty
{
public:
    XYProperty(const std::string& name, const IntProperty& x, const IntProperty& y, const std::string& displayName = "")
        : GroupProperty(name, {x.clone(), y.clone()}, displayName)
    {
    }

    const IntProperty& x() const { return at<IntProperty>("x"); }
};

TEST_CASE("Test XYProperty")
{
    testGroupProperty<XYProperty, IntProperty, IntProperty>(
        IntProperty("x", 0), IntProperty("y", 1), IntProperty("x", 3), IntProperty("y", 4));
}
}
