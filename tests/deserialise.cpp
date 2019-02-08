#include <catch2/catch.hpp>

#include "bool2property.h"

#include <basic_property.h>
#include <serialisation/json_serialiser.h>

namespace property
{

TEST_CASE("Deserialise from JSON")
{
    JSONSerialiser serialiser;

    SECTION("String")
    {

        CHECK(StringProperty("name", "Value") ==
              StringProperty::convert(*serialiser.deserialise(
                  R"JSON({"display":"name","id":"string","name":"name","value":"Value"})JSON")));
    }

    SECTION("Boolean")
    {
        CHECK(BooleanProperty("Boole", true, "MyBool") ==
              BooleanProperty::convert(*serialiser.deserialise(
                  R"JSON({"display":"MyBool","id":"bool","name":"Boole","value":true})JSON")));
    }

    SECTION("Group")
    {
        CHECK(
            Bool2Property("XY", BooleanProperty("x", true), BooleanProperty("y", false)) ==
            Bool2Property::convert(*serialiser.deserialise(
                R"JSON({"children":[{"display":"a","id":"bool","name":"a","value":true},{"display":"b","id":"bool","name":"b","value":false}],"display":"XY","id":"group","name":"XY"})JSON")));
    }

}
}
