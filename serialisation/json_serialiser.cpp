#include "json_serialiser.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace property
{

struct JSONNode : public Node {
    JSONNode(json& node) : node_{node} {}

    json& node_;
};

class JSONPropertySerialiser : public PropertySerialiser
{
private:
    void serialise(Node& raw, const Property& prop) override
    {
        JSONNode& node = raw.cast<JSONNode>();
        node.node_["id"] = prop.id();
        node.node_["name"] = prop.name();
        node.node_["display"] = prop.displayName();

        serialiseSpecific(node, prop);
    }

    virtual void serialiseSpecific(JSONNode& node, const Property& prop) = 0;
};

class JSONStringSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = StringProperty;

    void serialiseSpecific(JSONNode& node, const Property& prop) override
    {
        node.node_["value"] = prop.cast<value_type>().value();
    }
};

class JSONBooleanSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = BooleanProperty;

    void serialiseSpecific(JSONNode& node, const Property& prop) override
    {
        node.node_["value"] = prop.cast<value_type>().value();
    }
};

JSONSerialiser::JSONSerialiser() : Serialiser(Mapper<JSONStringSerialiser, JSONBooleanSerialiser>())
{
}

std::string JSONSerialiser::serialise(const Property& prop)
{
    json root;
    JSONNode node{root};
    serialiseNode(node, prop);
    return root.dump();
}
}
