#include <catch2/catch.hpp>

#include <basic_property.h>
#include <group_property.h>

#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace property
{

class Node
{
public:
    virtual void setId(const std::string& id) = 0;
    virtual void setName(const std::string& name) = 0;
    virtual void setDisplayName(const std::string& name) = 0;
    virtual void setValue(const std::string& key, const std::string& value) = 0;
};

class PropertySerialiser
{
public:
    virtual void serialise(Node& node, const Property& prop)
    {
        node.setId(prop.id());
        node.setName(prop.name());
        node.setDisplayName(prop.name());
    }
};

class StringSerialiser : public PropertySerialiser
{
public:
    using value_type = StringProperty;

    void serialise(Node& node, const Property& prop) override
    {
        PropertySerialiser::serialise(node, prop);
        node.setValue("value", prop.cast<value_type>().value());
    }
};

class Serialiser
{
public:
    Serialiser() { map<StringSerialiser>(); }

protected:
    void serialiseNode(Node& node, const Property& prop)
    {
        auto it = serialisers_.find(prop.id());
        assert(it != serialisers_.end());
        it->second->serialise(node, prop);
    }

private:
    template <class T>
    void map()
    {
        serialisers_[T::value_type::identifier] = std::unique_ptr<T>(new T);
    }

private:
    std::map<std::string, std::unique_ptr<PropertySerialiser>> serialisers_;
};

class JSONNode : public Node
{
public:
    JSONNode(json& node) : node_{node} {}

    void setId(const std::string& id) override { node_["id"] = id; }
    void setName(const std::string& name) override { node_["name"] = name; }
    void setDisplayName(const std::string& name) override { node_["display"] = name; }
    void setValue(const std::string& key, const std::string& value) override { node_[key] = value; }

private:
    json& node_;
};

class JSONSerialiser : public Serialiser
{
public:
    std::string serialise(const Property& prop)
    {
        json root;
        JSONNode node{root};
        serialiseNode(node, prop);
        return root.dump();
    }
};

TEST_CASE("serialise")
{
    StringProperty prop("name", "Value");

    JSONSerialiser serialiser;
    CHECK(serialiser.serialise(prop) == "");

    //    json j;
    //    j["a"] = 3.2;
    //    j["b"] = L"asdf";

    //    for (auto& el : j.items()) {
    //        CHECK(el.key() == "q");
    //        CHECK(el.value() == 1);
    //    }

    //    CHECK(j.dump() == "");

    FAIL("Not implemented");
}
}
