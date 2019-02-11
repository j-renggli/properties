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
        const typename T::value_type value = node.node_["value"];
        return std::make_unique<T>(node.node_["name"], value, node.node_["display"]);
    }

    void serialiseInternals(JSONNode& node, const Property& prop) override
    {
        node.node_["value"] = prop.cast<value_type>().value();
    }
};

template <class T>
class JSONNumericSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = T;

    std::unique_ptr<Property> deserialise(Node& raw) override
    {
        JSONNode& node = raw.cast<JSONNode>();
        const typename T::value_type value = node.node_["value"];
        const typename T::value_type min = node.node_["min"];
        const typename T::value_type max = node.node_["max"];
        return std::make_unique<T>(node.node_["name"], value, min, max, node.node_["display"]);
    }

    void serialiseInternals(JSONNode& node, const Property& prop) override
    {
        const T& numeric = prop.cast<value_type>();
        node.node_["value"] = numeric.value();
        if (numeric.min() != -value_type::max_value)
            node.node_["min"] = numeric.min();
        if (numeric.max() != value_type::max_value)
            node.node_["max"] = numeric.max();
    }
};

class JSONGroupProperty : public GroupProperty
{
public:
    JSONGroupProperty(std::function<std::unique_ptr<Property>(Node& node)> deserialise, json& node)
        : GroupProperty(node["name"], node["display"])
    {
        json& children = node["children"];
        const size_t total = children.size();
        childrenPointers_.resize(total);
        children_.resize(total);
        for (size_t i = 0; i < total; ++i) {
            JSONNode child(children[i]);
            children_[i] = deserialise(child);
            childrenPointers_[i] = children_[i].get();
        }
    }

    GroupPropertyIterator begin() const override { return GroupPropertyIterator{childrenPointers_.data(), 0, size()}; }
    GroupPropertyIterator end() const override
    {
        return GroupPropertyIterator{childrenPointers_.data(), size(), size()};
    }
    size_t size() const override { return children_.size(); }

private:
    std::vector<const Property*> childrenPointers_;
    std::vector<std::unique_ptr<Property>> children_;
};

class JSONGroupSerialiser : public JSONPropertySerialiser
{
public:
    using value_type = GroupProperty;

    JSONGroupSerialiser(std::function<std::unique_ptr<Property>(Node& node)> deserialiseChild,
                        std::function<void(Node& node, const Property& prop)> serialiseChild)
        : deserialiseChild_{deserialiseChild}, serialiseChild_{serialiseChild}
    {
    }

    std::unique_ptr<Property> deserialise(Node& raw) override
    {
        JSONNode& node = raw.cast<JSONNode>();
        return std::unique_ptr<JSONGroupProperty>(new JSONGroupProperty(deserialiseChild_, node.node_));
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
            ++it;
        }
    }

private:
    std::function<std::unique_ptr<Property>(Node& node)> deserialiseChild_;
    std::function<void(Node& node, const Property& prop)> serialiseChild_;
};

JSONSerialiser::JSONSerialiser()
    : Serialiser(Mapper<JSONBasicSerialiser<StringProperty>,
                        JSONBasicSerialiser<BooleanProperty>,
                        JSONNumericSerialiser<IntProperty>,
                        JSONNumericSerialiser<DoubleProperty>,
                        JSONGroupSerialiser>())
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
