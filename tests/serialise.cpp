#include <catch2/catch.hpp>

#include <serialisation/json_serialiser.h>

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

    //    json j;
    //    j["a"] = 3.2;
    //    j["b"] = L"asdf";

    //    for (auto& el : j.items()) {
    //        CHECK(el.key() == "q");
    //        CHECK(el.value() == 1);
    //    }

    //    CHECK(j.dump() == "");
}
}
