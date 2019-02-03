#pragma once

#include "properties_export.h"

#include <sstream>
#include <string>

namespace property
{

namespace stream
{
template <class V>
std::ostream& convert(std::ostream& out, const V& value)
{
    out << value;
    return out;
}
template <>
PROPERTIES_EXPORT std::ostream& convert<std::wstring>(std::ostream& out, const std::wstring& value);
template <class V>
std::wostream& convert(std::wostream& out, const V& value)
{
    out << value;
    return out;
}
template <>
PROPERTIES_EXPORT std::wostream& convert<std::string>(std::wostream& out, const std::string& value);
}

#define STR(body)                                                                                                      \
    std::ostream& str(std::ostream& out) const override                                                                \
    {                                                                                                                  \
        Property::str(out);                                                                                            \
        body;                                                                                                          \
        return out;                                                                                                    \
    }                                                                                                                  \
    std::wostream& str(std::wostream& out) const override                                                              \
    {                                                                                                                  \
        Property::str(out);                                                                                            \
        body;                                                                                                          \
        return out;                                                                                                    \
    }

class Property
{
public:
    Property(const std::string& name, const std::string& displayName)
        : name_{name}, displayName_{displayName.empty() ? name : displayName}
    {
    }
    virtual ~Property() {}

    operator std::string() const
    {
        std::stringstream ss;
        str(ss);
        return ss.str();
    }
    operator std::wstring() const
    {
        std::wstringstream ss;
        str(ss);
        return ss.str();
    }

    virtual const std::string& id() const = 0;
    const std::string& name() const { return name_; }
    const std::string& displayName() const { return displayName_; }

    template <class T>
    T& cast()
    {
        return dynamic_cast<T&>(*this);
    }
    template <class T>
    const T& cast() const
    {
        return dynamic_cast<const T&>(*this);
    }

    virtual std::ostream& str(std::ostream& out) const
    {
        out << displayName_;
        return out;
    }
    virtual std::wostream& str(std::wostream& out) const { return stream::convert(out, displayName_); }

protected:
    const std::string name_;
    const std::string displayName_;
};

inline std::ostream& operator<<(std::ostream& out, const Property& prop)
{
    prop.str(out);
    return out;
}

inline std::wostream& operator<<(std::wostream& out, const Property& prop)
{
    prop.str(out);
    return out;
}
}
