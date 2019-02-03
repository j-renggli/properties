#include <catch2/catch.hpp>

#include <numeric_property.h>

namespace property
{

template <class T>
void checkNumericProperty(const T& prop,
                          const typename T::value_type& value,
                          const typename T::value_type& min = -std::numeric_limits<typename T::value_type>::infinity(),
                          const typename T::value_type& max = std::numeric_limits<typename T::value_type>::infinity())
{
    CHECK(prop.value() == value);
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
    CHECK(prop.min() == min);
    CHECK(prop.max() == max);
}

template <class T>
void testNumericPropertyConstructor(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty constructor");
    T prop1("name", min, "display");
    checkNumericProperty(prop1, min);
    T prop2("name", max, min, max, "display");
    checkNumericProperty(prop2, max, min, max);
    CHECK_THROWS_AS(T("name", min - typename T::value_type(1), min, max), std::out_of_range);
}

template <class T>
void testNumericPropertyCopyConstructor(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty copy-constructor");
    T original("name", min, min, max, "display");
    T prop(original);
    checkNumericProperty(prop, min, min, max);
}

template <class T>
void testNumericPropertyCopyOperator(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty copy-operator");
    T original("name", min, min, max, "display");
    T prop("name", max, "display");
    prop = original;
    checkNumericProperty(prop, min, min, max);
}

template <class T>
void testNumericPropertyClone(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty clone");
    T original("name", min, min, max, "display");
    auto prop = original.clone();
    checkNumericProperty(prop->template cast<T>(), min, min, max);
}

template <class T>
void testNumericPropertyConvert(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty convert");
    T original("name", min, min, max, "display");
    auto prop = T::convert(original);
    checkNumericProperty(*prop, min, min, max);
}

template <class T>
void testNumericPropertyAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty constructor");
    T prop("name", min, min, max, "display");
    prop = max;
    checkNumericProperty(prop, max, min, max);
}

template <class T>
void testNumericPropertyCopyConstructedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty copy-constructor is independent from its base");
    T original("name", min, min, max, "display");
    T prop(original);
    prop = max;
    {
        INFO("Original is intact");
        checkNumericProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkNumericProperty(prop, max, min, max);
    }
}

template <class T>
void testNumericPropertyCopyOperatedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty copy-operator is independent from its base");
    T original("name", min, min, max, "display");
    T prop("name", max, "display");
    prop = original;
    checkNumericProperty(prop, min, min, max);
    prop = max;
    {
        INFO("Original is intact");
        checkNumericProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkNumericProperty(prop, max, min, max);
    }
}

template <class T>
void testNumericPropertyClonedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty clone is independent from its base");
    T original("name", min, min, max, "display");
    auto clone = original.clone();
    T& prop = clone->template cast<T>();
    prop = max;
    {
        INFO("Original is intact");
        checkNumericProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkNumericProperty(prop, max, min, max);
    }
}

template <class T>
void testNumericPropertyConvertedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty convert is independent from its base");
    T original("name", min, min, max, "display");
    auto prop = T::convert(original);
    *prop = max;
    {
        INFO("Original is intact");
        checkNumericProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkNumericProperty(*prop, max, min, max);
    }
}

template <class T>
void testNumericProperty(const typename T::value_type& min, const typename T::value_type& max)
{
    {
        INFO("Constructors and related");
        testNumericPropertyConstructor<T>(min, max);
        testNumericPropertyCopyConstructor<T>(min, max);
        testNumericPropertyCopyOperator<T>(min, max);
        testNumericPropertyClone<T>(min, max);
        testNumericPropertyConvert<T>(min, max);
    }
}

TEST_CASE("Test IntProperty")
{
    testNumericProperty<IntProperty>(1, 2);

    // MIN == MAX!
}
}
