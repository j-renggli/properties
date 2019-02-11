#pragma once

#include "../basic_property.h"
#include "../group_property.h"
#include "../numeric_property.h"
#include "../property.h"

#include <map>
#include <memory>

namespace property
{

struct Node {
    virtual ~Node() = default;

    virtual std::string id() = 0;

    template <class T>
    T& cast()
    {
        return dynamic_cast<T&>(*this);
    }
};

class PropertySerialiser
{
public:
    virtual std::unique_ptr<Property> deserialise(Node& node) = 0;
    virtual void serialise(Node& node, const Property& prop) = 0;
};

template <class String, class Bool, class Int, class Double, class Group>
struct Mapper {
    void fill(std::map<std::string, std::unique_ptr<PropertySerialiser>>& serialisers,
              std::function<std::unique_ptr<Property>(Node& node)> deserialiseChild,
              std::function<void(Node& node, const Property& prop)> serialiseChild) const
    {
        map<String, StringProperty>(serialisers);
        map<Bool, BooleanProperty>(serialisers);
        map<Int, IntProperty>(serialisers);
        map<Double, DoubleProperty>(serialisers);

        // Group is special
        static_assert(std::is_same<typename Group::value_type, GroupProperty>::value,
                      "Invalid serialiser for group property");
        serialisers[GroupProperty::identifier] = std::unique_ptr<Group>(new Group(deserialiseChild, serialiseChild));
    }

    template <class S, class P>
    void map(std::map<std::string, std::unique_ptr<PropertySerialiser>>& serialisers) const
    {
        static_assert(std::is_same<typename S::value_type, P>::value, "Invalid serialiser");
        serialisers[P::identifier] = std::unique_ptr<S>(new S);
    }
};

class Serialiser
{
public:
    template <class Map>
    Serialiser(const Map& mapper)
    {
        mapper.fill(serialisers_,
                    [this](Node& node) { return deserialiseNode(node); },
                    [this](Node& node, const Property& prop) { serialiseNode(node, prop); });
    }

protected:
    std::unique_ptr<Property> deserialiseNode(Node& node) const;
    void serialiseNode(Node& node, const Property& prop) const;

private:
    std::map<std::string, std::unique_ptr<PropertySerialiser>> serialisers_;
};
}
