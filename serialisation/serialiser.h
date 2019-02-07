#pragma once

#include "../basic_property.h"
#include "../group_property.h"
#include "../property.h"

#include <map>
#include <memory>

namespace property
{

struct Node {
    virtual ~Node() = default;

    template <class T>
    T& cast()
    {
        return dynamic_cast<T&>(*this);
    }
};

class PropertySerialiser
{
public:
    virtual void serialise(Node& node, const Property& prop) = 0;
};

template <class String, class Bool, class Group>
struct Mapper {
    void fill(std::map<std::string, std::unique_ptr<PropertySerialiser>>& serialisers,
              std::function<void(Node& node, const Property& prop)> serialiseChild) const
    {
        map<String, StringProperty>(serialisers);
        map<Bool, BooleanProperty>(serialisers);

        // Group is special
        static_assert(std::is_same<typename Group::value_type, GroupProperty>::value,
                      "Invalid serialiser for group property");
        serialisers[GroupProperty::identifier] = std::unique_ptr<Group>(new Group(serialiseChild));
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
        mapper.fill(serialisers_, [this](Node& node, const Property& prop) { serialiseNode(node, prop); });
    }

protected:
    void serialiseNode(Node& node, const Property& prop) const;

private:
    std::map<std::string, std::unique_ptr<PropertySerialiser>> serialisers_;
};
}
