#include "property.h"

#include <codecvt>
#include <locale>

namespace property
{

namespace stream
{

static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf;

template <>
std::ostream& convert<std::wstring>(std::ostream& out, const std::wstring& value)
{
    out << utf.to_bytes(value);
    return out;
}

template <>
std::wostream& convert<std::string>(std::wostream& out, const std::string& value)
{
    out << utf.from_bytes(value);
    return out;
}
}
}
