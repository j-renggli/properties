#include <catch2/catch.hpp>

#include <numeric_property.h>

namespace property
{

template <class T>
void checkNumericProperty(const T& prop,
                          const typename T::value_type& value,
                          const typename T::value_type& min = typename T::value_type(-T::max_value),
                          const typename T::value_type& max = typename T::value_type(T::max_value))
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
    {
        INFO("Can construct a basic numeric property");
        T prop1("name", min, "display");
        checkNumericProperty(prop1, min);
    }
    {
        INFO("Can construct a numeric property with limits");
        T prop2("name", max, min, max, "display");
        checkNumericProperty(prop2, max, min, max);
    }
    {
        INFO("Can construct a numeric property with single allowed value");
        T prop2("name", min, min, min, "display");
        checkNumericProperty(prop2, min, min, min);
    }
    {
        INFO("Cannot construct a numeric property without respecting min value");
        CHECK_THROWS_AS(T("name", min - typename T::value_type(1), min, max), std::out_of_range);
    }
    {
        INFO("Cannot construct a numeric property without respecting max value");
        CHECK_THROWS_AS(T("name", max + typename T::value_type(1), min, max), std::out_of_range);
    }
    {
        INFO("Cannot construct a numeric property with min > max");
        CHECK_THROWS_AS(T("name", min, max, min), std::out_of_range);
    }
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
    {
        INFO("Keep names");
        auto prop = original.clone();
        checkNumericProperty(prop->template cast<T>(), min, min, max);
    }
    {
        INFO("Change name");
        auto prop = original.clone("rename");
        CHECK(prop->template cast<T>().value() == min);
        CHECK(prop->name() == "rename");
        CHECK(prop->displayName() == "display");
    }
    {
        INFO("Change name and display name");
        auto prop = original.clone("rename", "redisplay");
        CHECK(prop->template cast<T>().value() == min);
        CHECK(prop->name() == "rename");
        CHECK(prop->displayName() == "redisplay");
    }
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
void testNumericPropertyEquality(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty equals another of same name and value");
    T left("name", min, min, max, "display");
    T right("name", min, min, max, "display2");
    CHECK(left == left);
    CHECK(left == right);
}

template <class T>
void testNumericPropertyUnequality(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("NumericProperty is not equal to another of different name, value, min or max");
    const T base("name", min, min, max, "display");
    CHECK(base != T("name2", min, min, max, "display"));
    CHECK(base != T("name", max, min, max, "display"));
    CHECK(base != T("name", min, min - 1, max, "display"));
    CHECK(base != T("name", min, min, max + 1, "display"));
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

    {
        INFO("Set value modifies only target object");
        testNumericPropertyAssignment<T>(min, max);
        testNumericPropertyCopyConstructedAssignment<T>(min, max);
        testNumericPropertyCopyOperatedAssignment<T>(min, max);
        testNumericPropertyClonedAssignment<T>(min, max);
        testNumericPropertyConvertedAssignment<T>(min, max);
    }

    {
        INFO("Equality of basic properties");
        testNumericPropertyEquality<T>(min, max);
        testNumericPropertyUnequality<T>(min, max);
    }

    {
        INFO("To string");
        T prop("a_name", min, "Display");
        std::stringstream ss;
        ss << "Display=" << T::identifier << "[";
        stream::convert(ss, min) << "]";
        std::wstringstream wss;
        wss << L"Display=";
        stream::convert(wss, T::identifier) << L"[";
        stream::convert(wss, min) << L"]";
        CHECK(static_cast<std::string>(prop) == ss.str());
        CHECK(static_cast<std::wstring>(prop) == wss.str());
    }
}

TEST_CASE("Test IntProperty")
{
    testNumericProperty<IntProperty>(1, 2);
}

TEST_CASE("Test DoubleProperty")
{
    testNumericProperty<DoubleProperty>(-1.3, 2.777);
}
}
