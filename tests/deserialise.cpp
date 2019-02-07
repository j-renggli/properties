#include <catch2/catch.hpp>

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

}
}
