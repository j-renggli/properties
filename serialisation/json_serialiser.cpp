#include "json_serialiser.h"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace property
{

struct JSONNode : public Node {
    JSONNode(json& node) : node_{node} {}

    std::string id() override { return node_["id"]; }

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

        serialiseInternals(node, prop);
    }

    virtual void serialiseInternals(JSONNode& node, const Property& prop) = 0;
};

template <class T>
class JSONBasicSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = T;

    std::unique_ptr<Property> deserialise(Node& raw) override
    {
        JSONNode& node = raw.cast<JSONNode>();
        return std::make_unique<T>(node.node_["name"], node.node_["value"], node.node_["display"]);
    }

    void serialiseInternals(JSONNode& node, const Property& prop) override
    {
        node.node_["value"] = prop.cast<value_type>().value();
    }
};

class JSONGroupSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = GroupProperty;

    JSONGroupSerialiser(std::function<void(Node& node, const Property& prop)> serialiseChild)
        : serialiseChild_{serialiseChild}
    {
    }

    std::unique_ptr<Property> deserialise(Node& raw) override
    {
        return std::unique_ptr<Property>();
    }

    void serialiseInternals(JSONNode& node, const Property& prop) override
    {
        const value_type& group = prop.cast<value_type>();
        const size_t size = group.size();
        node.node_["children"] = std::vector<json>(size);
        auto it = group.begin();
        for (size_t i = 0; i < size; ++i) {
            JSONNode child(node.node_["children"][i]);
            serialiseChild_(child, *it);
        }
    }

private:
    std::function<void(Node& node, const Property& prop)> serialiseChild_;
};

JSONSerialiser::JSONSerialiser()
    : Serialiser(
          Mapper<JSONBasicSerialiser<StringProperty>, JSONBasicSerialiser<BooleanProperty>, JSONGroupSerialiser>())
{
}

std::unique_ptr<Property> JSONSerialiser::deserialise(const std::string& jsonString) const
{
    json root = json::parse(jsonString);
    JSONNode node{root};
    return deserialiseNode(node);
}

std::string JSONSerialiser::serialise(const Property& prop) const
{
    json root;
    JSONNode node{root};
    serialiseNode(node, prop);
    return root.dump();
}
}
