#include <catch2/catch.hpp>

#include <serialisation/json_serialiser.h>

#include "bool2property.h"
#include "xyproperty.h"

namespace property
{

TEST_CASE("Serialise to JSON")
{
    JSONSerialiser serialiser;

    SECTION("String")
    {
        CHECK(serialiser.serialise(StringProperty("name", "Value")) ==
              R"JSON({"display":"name","id":"string","name":"name","value":"Value"})JSON");
    }

    SECTION("Boolean")
    {
        CHECK(serialiser.serialise(BooleanProperty("Bool", true, "MyBool")) ==
              R"JSON({"display":"MyBool","id":"bool","name":"Bool","value":true})JSON");
    }

    SECTION("Int with default limits")
    {
        CHECK(serialiser.serialise(IntProperty("Int", 3, "MyInt")) ==
              R"JSON({"display":"MyInt","id":"int","name":"Int","value":3})JSON");
    }

    SECTION("Int with default min")
    {
        CHECK(serialiser.serialise(IntProperty("Int", 3, -IntProperty::max_value, 9, "MyInt")) ==
              R"JSON({"display":"MyInt","id":"int","max":9,"name":"Int","value":3})JSON");
    }

    SECTION("Int with default max")
    {
        CHECK(serialiser.serialise(IntProperty("Int", 3, -2)) ==
              R"JSON({"display":"Int","id":"int","min":-2,"name":"Int","value":3})JSON");
    }

    SECTION("Int with min/max")
    {
        CHECK(serialiser.serialise(IntProperty("Int", 3, -4, 7, "MyInt")) ==
              R"JSON({"display":"MyInt","id":"int","max":7,"min":-4,"name":"Int","value":3})JSON");
    }

    SECTION("Double with default limits")
    {
        CHECK(serialiser.serialise(DoubleProperty("Double", 3, "MyDouble")) ==
              R"JSON({"display":"MyDouble","id":"double","name":"Double","value":3.0})JSON");
    }

    SECTION("Double with default min")
    {
        CHECK(serialiser.serialise(DoubleProperty("Double", -0.1, -DoubleProperty::max_value, -0.001, "MyDouble")) ==
              R"JSON({"display":"MyDouble","id":"double","max":-0.001,"name":"Double","value":-0.1})JSON");
    }

    SECTION("Double with default max")
    {
        CHECK(serialiser.serialise(DoubleProperty("Double", 0.11, -2.6)) ==
              R"JSON({"display":"Double","id":"double","min":-2.6,"name":"Double","value":0.11})JSON");
    }

    SECTION("Double with min/max")
    {
        CHECK(serialiser.serialise(DoubleProperty("Double", 3.11, -4.2, 7.333, "MyDouble")) ==
              R"JSON({"display":"MyDouble","id":"double","max":7.333,"min":-4.2,"name":"Double","value":3.11})JSON");
    }

    SECTION("Group")
    {

        CHECK(
            serialiser.serialise(XYProperty("XY", IntProperty("a", 3), IntProperty("b", 1, -3, 9, "MyB"))) ==
            R"JSON({"children":[{"display":"a","id":"int","name":"a","value":3},{"display":"MyB","id":"int","max":9,"min":-3,"name":"b","value":1}],"display":"XY","id":"group","name":"XY"})JSON");
    }
}
}
