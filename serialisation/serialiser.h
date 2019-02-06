#pragma once

#include "../basic_property.h"
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

template <class String, class Bool>
struct Mapper {
    void fill(std::map<std::string, std::unique_ptr<PropertySerialiser>>& serialisers) const
    {
        map<String, StringProperty>(serialisers);
        map<Bool, BooleanProperty>(serialisers);
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
        mapper.fill(serialisers_);
    }

protected:
    void serialiseNode(Node& node, const Property& prop);

private:
    std::map<std::string, std::unique_ptr<PropertySerialiser>> serialisers_;
};
}
