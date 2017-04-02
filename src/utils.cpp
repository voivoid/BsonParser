#include "BSON/Details/utils.h"

#include <algorithm>
#include <cassert>
#include <limits>

#include <boost\bimap\bimap.hpp>

namespace Bson
{

const size_t MaxCStringBuff = 16384;

//enum class ElementType {
//    Double = 0x01,
//    String = 0x02,
//    Int32 = 0x10,
//    Uint32 = 0x10,
//    Int64 = 0x12,    
//    Decimal = 0x13
//};
//const boost::bimaps::bimap<int, ElementType> ElementTypes = {};

// TODO: handle endianness

template <typename T>
T read(Istream& stream)
{
    T value;
    stream.read(reinterpret_cast<Byte*>(&value), sizeof(value));
    return value;
}

template <typename T>
void write(const T value, Ostream& stream)
{
    stream.write(reinterpret_cast<const Byte*>(&value), sizeof(value));
}

#define BSON_INSTANTIATE_FUNCS(type)                      \
    template type read(Istream& iter);                    \
    template void write(const type value, Ostream& iter); \

BSON_INSTANTIATE_FUNCS(Byte);
BSON_INSTANTIATE_FUNCS(Int32);
BSON_INSTANTIATE_FUNCS(Int64);
BSON_INSTANTIATE_FUNCS(Uint64);
BSON_INSTANTIATE_FUNCS(Double);
BSON_INSTANTIATE_FUNCS(Decimal);

std::string readCString(Istream& stream)
{
    Byte buff[MaxCStringBuff];
    stream.getline(buff, MaxCStringBuff, 0);

    return std::string(reinterpret_cast<char*>(buff));
}

std::string readString(Istream& stream)
{
    Int32 strLen = read<Int32>(stream);

    assert(strLen > 0);

    std::string s;
    s.resize(strLen - 1);
    stream.read(reinterpret_cast<Byte*>(&s[0]), strLen - 1);
    stream.ignore(1);

    return s;
}

void writeCString(const std::string& string, Ostream& stream)
{
    stream.write(reinterpret_cast<const Byte*>(string.c_str()), string.length() + 1);
}

void writeString(const std::string& string, Ostream& stream)
{
    write(static_cast<Int32>(string.length() + 1), stream);
    writeCString(string, stream);
}

Element readElement(Istream& stream)
{
    const auto elementType = read<Byte>(stream);

    return {};
}

void writeElement(const Element& element, Ostream& stream)
{

}

} // namespace Bson
