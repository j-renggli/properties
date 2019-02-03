#include <catch2/catch.hpp>

#include <basic_property.h>

namespace property
{

template <class T>
void checkBasicProperty(const T& prop, const typename T::value_type& value)
{
    CHECK(prop.value() == value);
    CHECK(prop.name() == "name");
    CHECK(prop.displayName() == "display");
    CHECK(prop.id() == T::identifier);
}

template <class T>
void testBasicPropertyConstructor(const typename T::value_type& value)
{
    INFO("BasicProperty constructor");
    T prop("name", value, "display");
    checkBasicProperty(prop, value);
}

template <class T>
void testBasicPropertyCopyConstructor(const typename T::value_type& value)
{
    INFO("BasicProperty copy-constructor");
    T original("name", value, "display");
    T prop(original);
    checkBasicProperty(prop, value);
}

template <class T>
void testBasicPropertyCopyOperator(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BasicProperty copy-operator");
    T original("name2", value, "display2");
    T prop("name", other, "display");
    prop = original;
    checkBasicProperty(prop, value);
}

template <class T>
void testBasicPropertyConvert(const typename T::value_type& value)
{
    INFO("BasicProperty convert");
    T original("name", value, "display");
    auto prop = T::convert(original);
    checkBasicProperty(prop, value);
}

template <class T>
void testBasicPropertyAssignment(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BasicProperty constructor");
    T prop("name", value, "display");
    prop = other;
    checkBasicProperty(prop, other);
}

template <class T>
void testBasicPropertyCopyConstructedAssignment(const typename T::value_type& value,
                                                const typename T::value_type& other)
{
    INFO("BasicProperty copy-constructor is independent from its base");
    T original("name", value, "display");
    T prop(original);
    prop = other;
    checkBasicProperty(original, value);
    checkBasicProperty(prop, other);
}

template <class T>
void testBasicPropertyCopyOperatedAssignment(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BasicProperty copy-operator is independent from its base");
    T original("name", value, "display");
    T prop("name", other, "display");
    prop = original;
    checkBasicProperty(prop, value);
    prop = other;
    checkBasicProperty(original, value);
    checkBasicProperty(prop, other);
}

template <class T>
void testBasicPropertyConvertedAssignment(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BasicProperty convert is independent from its base");
    T original("name", value, "display");
    auto prop = T::convert(original);
    prop = other;
    checkBasicProperty(original, value);
    checkBasicProperty(prop, other);
}

template <class T>
void testBasicPropertyEquality(const typename T::value_type& value)
{
    INFO("BasicProperty equals another of same name and value");
    T left("name", value, "display");
    T right("name", value, "display2");
    CHECK(left == left);
    CHECK(left == right);
}

template <class T>
void testBasicPropertyUnequality(const typename T::value_type& value, const typename T::value_type& other)
{
    INFO("BasicProperty is not equal to another of different name or value");
    T base("name", value, "display");
    T name("name2", value, "display");
    T val("name", other, "display");
    CHECK(base != name);
    CHECK(base != val);
}

template <class T>
void testBasicProperty(const typename T::value_type& base, const typename T::value_type& other)
{
    {
        INFO("Constructors and related");
        testBasicPropertyConstructor<T>(base);
        testBasicPropertyCopyConstructor<T>(base);
        testBasicPropertyCopyOperator<T>(base, other);
        testBasicPropertyConvert<T>(base);
    }

    {
        INFO("Set value modifies only target object");
        testBasicPropertyAssignment<T>(base, other);
        testBasicPropertyCopyConstructedAssignment<T>(base, other);
        testBasicPropertyCopyOperatedAssignment<T>(base, other);
        testBasicPropertyConvertedAssignment<T>(base, other);
    }

    {
        INFO("Equality of basic properties");
        testBasicPropertyEquality<T>(base);
        testBasicPropertyUnequality<T>(base, other);
    }

    {
        INFO("To string");
        T prop("a_name", base, "Display");
        std::stringstream ss;
        ss << "Display=" << T::identifier << "[";
        stream::convert(ss, base) << "]";
        std::wstringstream wss;
        wss << L"Display=";
        stream::convert(wss, T::identifier) << L"[";
        stream::convert(wss, base) << L"]";
        CHECK(static_cast<std::string>(prop) == ss.str());
        CHECK(static_cast<std::wstring>(prop) == wss.str());
    }
}

TEST_CASE("Test BooleanProperty")
{
    testBasicProperty<BooleanProperty>(true, false);
}

TEST_CASE("Test StringProperty")
{
    testBasicProperty<StringProperty>("Asdf", "Qwer");
}

TEST_CASE("Test WStringProperty")
{
    testBasicProperty<WStringProperty>(L"Asdf", L"Qwer");
}
}
