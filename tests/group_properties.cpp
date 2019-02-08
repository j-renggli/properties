#include <catch2/catch.hpp>

#include "xyproperty.h"

#include <group_property.h>

namespace property
{

template <class T>
void compareChildren(GroupPropertyIterator it, GroupPropertyIterator end, const T& value)
{
    REQUIRE(it != end);
    CHECK(it->cast<T>() == value);
    REQUIRE(++it == end);
}
template <class First, class Second, class... Rest>
void compareChildren(GroupPropertyIterator it,
                     GroupPropertyIterator end,
                     const First& first,
                     const Second& second,
                     const Rest&... rest)
{
    REQUIRE(it != end);
    CHECK(it->cast<First>() == first);
    compareChildren(++it, end, second, rest...);
}

template <class T, class... Params>
void checkGroupProperty(const T& prop, const Params&... children)
{
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
    compareChildren(prop.begin(), prop.end(), children...);
}

template <class T, class... Params>
void testGroupPropertyConstructor(const Params&... children)
{
    INFO("GroupProperty constructor");
    T prop("name", children..., "display");
    checkGroupProperty(prop, children...);
}

template <class T, class... Params>
void testGroupPropertyCopyConstructor(const Params&... children)
{
    INFO("GroupProperty copy-constructor");
    T original("name", children..., "display");
    T prop(original);
    checkGroupProperty(prop, children...);
}

template <class T, class... Params>
void testGroupPropertyCopyOperator(const Params&... children, const Params&... other)
{
    INFO("GroupProperty copy-operator");
    T original("name", children..., "display");
    T prop("name", other..., "display");
    prop = original;
    checkGroupProperty(prop, children...);
}

template <class T, class... Params>
void testGroupPropertyConvert(const Params&... children)
{
    INFO("GroupProperty convert");
    T original("name", children..., "display");
    auto prop = T::convert(original);
    checkGroupProperty(prop, children...);
}

template <class T, class... Params>
void testGroupPropertyEquality(const Params&... children)
{
    INFO("GroupProperty equals another of same name and value");
    T left("name", children..., "display");
    T right("name", children..., "display2");
    CHECK(left == left);
    CHECK(left == right);
}

template <class T, class... Params>
void testGroupPropertyUnequality(const Params&... children, const Params&... other)
{
    INFO("GroupProperty is not equal to another of different name or value");
    T base("name", children..., "display");
    T name("name2", children..., "display");
    T val("name", other..., "display");
    CHECK(base != name);
    CHECK(base != val);
}

template <class T, class... Params>
void testGroupProperty(const Params&... base, const Params&... other)
{
    {
        INFO("Constructors and related");
        testGroupPropertyConstructor<T, Params...>(base...);
        testGroupPropertyCopyConstructor<T, Params...>(base...);
        testGroupPropertyCopyOperator<T, Params...>(base..., other...);
        testGroupPropertyConvert<T, Params...>(base...);
    }

    {
        INFO("Equality of basic properties");
        testGroupPropertyEquality<T, Params...>(base...);
        testGroupPropertyUnequality<T, Params...>(base..., other...);
    }

    {
        INFO("To string");
        T prop("a_name", base..., "Display");
        std::stringstream ss;
        std::wstringstream wss;
        ss << "Display=" << T::identifier << "[";
        wss << L"Display=";
        stream::convert(wss, T::identifier) << L"[";
        auto it = prop.begin();
        if (it != prop.end()) {
            it->str(ss);
            it->str(wss);
        }
        while (++it != prop.end()) {
            ss << ",";
            wss << L",";
            it->str(ss);
            it->str(wss);
        }
        ss << "]";
        wss << "]";
        CHECK(static_cast<std::string>(prop) == ss.str());
        CHECK(static_cast<std::wstring>(prop) == wss.str());
    }
}

TEST_CASE("Test XYProperty")
{
    XYProperty original("name", IntProperty("x", 0), IntProperty("y", 1), "display");
    // Check iterators
    auto b = original.begin();
    auto e = original.end();
    CHECK(b != e);
    CHECK(++b != e);
    CHECK(++b == e);
    // Try iterating
    int i = 0;
    for (const auto& it : original) {
        CHECK(it.cast<IntProperty>().value() == i++);
    }

    testGroupProperty<XYProperty, IntProperty, IntProperty>(
        IntProperty("x", 0), IntProperty("y", 1), IntProperty("x", 3), IntProperty("y", 4));
}
}
