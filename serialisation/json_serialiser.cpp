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
    void serialise(Serialiser& serialiser, Node& raw, const Property& prop) override
    {
        JSONNode& node = raw.cast<JSONNode>();
        node.node_["id"] = prop.id();
        node.node_["name"] = prop.name();
        node.node_["display"] = prop.displayName();

        serialiseInternals(serialiser, node, prop);
    }

    virtual void serialiseInternals(Serialiser& serialiser, JSONNode& node, const Property& prop) = 0;
};

template <class T>
class JSONBasicSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = T;

    void serialiseInternals(Serialiser& serialiser, JSONNode& node, const Property& prop) override
    {
        node.node_["value"] = prop.cast<value_type>().value();
    }
};

class JSONGroupSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = GroupProperty;

    void serialiseInternals(Serialiser& serialiser, JSONNode& node, const Property& prop) override
    {
        const value_type& group = prop.cast<value_type>();
        const size_t size = group.size();
        node.node_["children"] = std::vector<json>(size);
        auto it = group.begin();
        for (size_t i = 0; i < size; ++i) {
            JSONNode child(node.node_["children"][i]);
            serialiser.serialiseNode(child, *it);
        }
    }
};

JSONSerialiser::JSONSerialiser()
    : Serialiser(
          Mapper<JSONBasicSerialiser<StringProperty>, JSONBasicSerialiser<BooleanProperty>, JSONGroupSerialiser>())
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
