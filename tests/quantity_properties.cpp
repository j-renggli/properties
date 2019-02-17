#include <quantities/time_property.h>

std::ostream& operator<<(std::ostream& out, const property::TimeProperty::value_type& sec)
{
    out << sec.count() << "s";
    return out;
}

#include <catch2/catch.hpp>

namespace property
{

template <class T>
void checkQuantityProperty(const T& prop,
                           const typename T::value_type value,
                           const typename T::value_type min = typename T::value_type(-T::max_value),
                           const typename T::value_type max = typename T::value_type(T::max_value))
{
    CHECK(prop.value() == value);
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
    CHECK(prop.min() == min);
    CHECK(prop.max() == max);
}

template <class T>
void testQuantityPropertyConstructor(const typename T::value_type min, const typename T::value_type max)
{
    INFO("QuantityProperty constructor");
    {
        INFO("Can construct a basic quantity property");
        T prop1("name", min, "display");
        checkQuantityProperty(prop1, min);
    }
    {
        INFO("Can construct a numeric property with limits");
        T prop2("name", max, min, max, "display");
        checkQuantityProperty(prop2, max, min, max);
    }
    {
        INFO("Can construct a numeric property with single allowed value");
        T prop2("name", min, min, min, "display");
        checkQuantityProperty(prop2, min, min, min);
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
void testQuantityPropertyCopyConstructor(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty copy-constructor");
    T original("name", min, min, max, "display");
    T prop(original);
    checkQuantityProperty(prop, min, min, max);
}

template <class T>
void testQuantityPropertyCopyOperator(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty copy-operator");
    T original("name2", min, min, max, "display2");
    T prop("name", max, "display");
    prop = original;
    checkQuantityProperty(prop, min, min, max);
}

template <class T>
void testQuantityPropertyConvert(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty convert");
    T original("name", min, min, max, "display");
    auto prop = T::convert(original);
    checkQuantityProperty(prop, min, min, max);
}

template <class T>
void testQuantityPropertyAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty constructor");
    T prop("name", min, min, max, "display");
    prop = max;
    checkQuantityProperty(prop, max, min, max);
}

template <class T>
void testQuantityPropertyCopyConstructedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty copy-constructor is independent from its base");
    T original("name", min, min, max, "display");
    T prop(original);
    prop = max;
    {
        INFO("Original is intact");
        checkQuantityProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkQuantityProperty(prop, max, min, max);
    }
}

template <class T>
void testQuantityPropertyCopyOperatedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty copy-operator is independent from its base");
    T original("name", min, min, max, "display");
    T prop("name", max, "display");
    prop = original;
    checkQuantityProperty(prop, min, min, max);
    prop = max;
    {
        INFO("Original is intact");
        checkQuantityProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkQuantityProperty(prop, max, min, max);
    }
}

template <class T>
void testQuantityPropertyConvertedAssignment(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty convert is independent from its base");
    T original("name", min, min, max, "display");
    auto prop = T::convert(original);
    prop = max;
    {
        INFO("Original is intact");
        checkQuantityProperty(original, min, min, max);
    }
    {
        INFO("Copy is modified");
        checkQuantityProperty(prop, max, min, max);
    }
}

template <class T>
void testQuantityPropertyEquality(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty equals another of same name and value");
    T left("name", min, min, max, "display");
    T right("name", min, min, max, "display2");
    CHECK(left == left);
    CHECK(left == right);
}

template <class T>
void testQuantityPropertyUnequality(const typename T::value_type& min, const typename T::value_type& max)
{
    INFO("QuantityProperty is not equal to another of different name, value, min or max");
    const T base("name", min, min, max, "display");
    CHECK(base != T("name2", min, min, max, "display"));
    CHECK(base != T("name", max, min, max, "display"));
    CHECK(base != T("name", min, min - typename T::value_type(1), max, "display"));
    CHECK(base != T("name", min, min, max + typename T::value_type(1), "display"));
}

template <class T>
void testQuantityProperty(const typename T::value_type min, const typename T::value_type max)
{
    {
        INFO("Constructors and related");
        testQuantityPropertyConstructor<T>(min, max);
        testQuantityPropertyCopyConstructor<T>(min, max);
        testQuantityPropertyCopyOperator<T>(min, max);
        testQuantityPropertyConvert<T>(min, max);
    }

    {
        INFO("Set value modifies only target object");
        testQuantityPropertyAssignment<T>(min, max);
        testQuantityPropertyCopyConstructedAssignment<T>(min, max);
        testQuantityPropertyCopyOperatedAssignment<T>(min, max);
        testQuantityPropertyConvertedAssignment<T>(min, max);
    }

    {
        INFO("Equality of basic properties");
        testQuantityPropertyEquality<T>(min, max);
        testQuantityPropertyUnequality<T>(min, max);
    }

    {
        INFO("To string");
        T prop("a_name", min, "Display");
        std::stringstream ss;
        ss << "Display=" << T::identifier << "[";
        stream::convert(ss, min.count()) << "s]";
        std::wstringstream wss;
        wss << L"Display=";
        stream::convert(wss, T::identifier) << L"[";
        stream::convert(wss, min.count()) << L"s]";
        CHECK(static_cast<std::string>(prop) == ss.str());
        CHECK(static_cast<std::wstring>(prop) == wss.str());
    }
}

using sec = std::chrono::duration<double>;
using ms = std::chrono::duration<double, std::milli>;

TEST_CASE("Test TimeProperty")
{
    testQuantityProperty<TimeProperty>(ms{1.}, sec{2.});
}
}
