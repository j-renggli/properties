#include <catch2/catch.hpp>

#include <serialisation/json_serialiser.h>

#include "bool2property.h"

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
        CHECK(serialiser.serialise(BooleanProperty("Boole", true, "MyBool")) ==
              R"JSON({"display":"MyBool","id":"bool","name":"Boole","value":true})JSON");
    }

    SECTION("Group")
    {
        CHECK(
            serialiser.serialise(Bool2Property("XY", BooleanProperty("x", true), BooleanProperty("y", false))) ==
            R"JSON({"children":[{"display":"MyA","id":"bool","name":"a","value":true},{"display":"MyB","id":"bool","name":"b","value":false}],"display":"XY","id":"group","name":"XY"})JSON");

    }
}
}
