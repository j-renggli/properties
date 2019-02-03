#include <catch2/catch.hpp>

#include <group_property.h>
#include <known_group_property.h>
#include <numeric_property.h>

namespace property
{






template <class T, class... Params>
void checkGroupProperty(const T& prop, const Params&... children)
{
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
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
void testGroupPropertyConvert(const Params&... children)
{
    INFO("GroupProperty convert");
    T original("name", children..., "display");
    //    auto prop = T::convert(original);
    //    checkGroupProperty(*prop, children...);
}

template <class T, class... Params>
void testGroupProperty(const Params&... base, const Params&... other)
{
    {
        INFO("Constructors and related");
        testGroupPropertyConstructor<T, Params...>(base...);
        testGroupPropertyCopyConstructor<T, Params...>(base...);
        testGroupPropertyConvert<T, Params...>(base...);

    }
}

class XYProperty : public KnownGroupProperty<2>
{
public:
    XYProperty(const std::string& name, const IntProperty& x, const IntProperty& y, const std::string& displayName = "")
        : KnownGroupProperty<2>(name, {{&x_, &y_}}, displayName), x_{x}, y_{y}
    {
    }

private:
    IntProperty x_;
    IntProperty y_;
};

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
    for (const auto& it : original)
    {
        CHECK(it.cast<IntProperty>().value() == i++);
    }
    testGroupProperty<XYProperty, IntProperty, IntProperty>(
        IntProperty("x", 0), IntProperty("y", 1), IntProperty("x", 3), IntProperty("y", 4));
}
}
