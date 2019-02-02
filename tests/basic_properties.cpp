#include <catch2/catch.hpp>

#include <basic_property.h>

namespace property
{

template <class T>
void checkBaseProperty(const T& prop, const typename T::value_type& value)
{
    CHECK(prop.value() == value);
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
}

template <class T>
void testBasePropertyConstructor(const typename T::value_type& value)
{
    INFO("BaseProperty constructor");
    T prop(value, "name", "display");
    checkBaseProperty(prop, value);
}

template <class T>
void testBasePropertyCopyConstructor(const typename T::value_type& value)
{
    INFO("BaseProperty copy-constructor");
    T original(value, "name", "display");
    T prop(original);
    checkBaseProperty(prop, value);
}

template <class T>
void testBasePropertyCopyOperator(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BaseProperty copy-operator");
    T original(value, "name", "display");
    T prop(other, "name", "display");
    prop = original;
    checkBaseProperty(prop, value);
}

template <class T>
void testBasePropertyClone(const typename T::value_type& value)
{
    INFO("BaseProperty clone");
    T original(value, "name", "display");
    auto prop = original.clone();
    checkBaseProperty(prop->template cast<T>(), value);
}

template <class T>
void testBasePropertyConvert(const typename T::value_type& value)
{
    INFO("BaseProperty convert");
    T original(value, "name", "display");
    auto prop = T::convert(original);
    checkBaseProperty(*prop, value);
}

template <class T>
void testBaseProperty(const typename T::value_type& base, const typename T::value_type& other)
{
    testBasePropertyConstructor<T>(base);
    testBasePropertyCopyConstructor<T>(base);
    testBasePropertyCopyOperator<T>(base, other);
    testBasePropertyClone<T>(base);
    testBasePropertyConvert<T>(base);
}

template <class T, class... Params>
struct GroupPropertyTester {
};

TEST_CASE("Test BooleanProperty")
{
    testBaseProperty<BooleanProperty>(true, false);
}

TEST_CASE("Test WStringProperty")
{
    testBaseProperty<WStringProperty>(L"Asdf", L"Qwer");
}
}
