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
void testNumericProperty(const typename T::value_type& min, const typename T::value_type& max)
{
    {
        INFO("Constructors and related");
        testNumericPropertyConstructor<T>(min, max);
    }
}

TEST_CASE("Test IntProperty")
{
    testNumericProperty<IntProperty>(1, 2);

    // MIN == MAX!
}
}
