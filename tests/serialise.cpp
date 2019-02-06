#include <catch2/catch.hpp>

#include <serialisation/json_serialiser.h>

#include <known_group_property.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace property
{

class Bool2Property : public KnownGroupProperty<2>
{
public:
    //    XYProperty(int x, int y)
    Bool2Property(const std::string& name,
                  const BooleanProperty& x,
                  const BooleanProperty& y,
                  const std::string& displayName = "")
        : KnownGroupProperty<2>(name, {{&x_, &y_}}, displayName), x_{x}, y_{y}
    {
    }

    Bool2Property& operator=(const Bool2Property& rhs)
    {
        x_ = rhs.x_;
        y_ = rhs.y_;
        return *this;
    }
    bool operator==(const Bool2Property& rhs) const { return !operator!=(rhs); }
    virtual bool operator!=(const Bool2Property& rhs) const { return different(rhs) || x_ != rhs.x_ || y_ != rhs.y_; }

    //    const IntProperty& x() const { return at<IntProperty>("x"); }
    //    const IntProperty& y() const { return at<IntProperty>("y"); }

    static Bool2Property convert(const Property& property)
    {
        return Bool2Property(property.name(),
                             property.cast<GroupProperty>().get<BooleanProperty>("x"),
                             property.cast<GroupProperty>().get<BooleanProperty>("y"),
                             property.displayName());
    }

private:
    BooleanProperty x_;
    BooleanProperty y_;
};

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
        CHECK(serialiser.serialise(Bool2Property("XY", BooleanProperty("a", true), BooleanProperty("b", false))) ==
              R"JSON({"display":"MyBool","id":"bool","name":"Boole","value":true})JSON");
    }
}
}
