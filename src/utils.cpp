#include "Bson/Details/utils.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>

#include <boost/hana/fold_left.hpp>
#include <boost/hana/at_key.hpp>

namespace Bson
{

namespace
{

const size_t MaxCStringBuff = 16384;

template <typename T, T(*f)(Istream&)>
Element::Value adapt(Istream& stream)
{
    return {f(stream)};
}

using Reader = Element::Value(*)(Istream&);

struct GetElementIndexVisitor : boost::static_visitor<size_t> {
    template <typename T>
    size_t operator()(const T&) const
    {
        return 0;
    }
};

struct WriteElementVisitor : boost::static_visitor<void> {

    WriteElementVisitor(Ostream& stream) : stream(stream) {}

    template <typename T>
    void operator()(const T& value) const
    {
        write<T>(value, stream);
    }

private:
    Ostream& stream;
};

}

const auto IndexToReaderMap = boost::hana::fold_left(Element::TypeInfoMap, std::map<int, Reader>{}, [](auto map, const auto pair) {
    const auto type = boost::hana::first(pair);
    const auto index = boost::hana::second(pair);

    using Type = typename decltype(type)::type;

    map[index] = &adapt<Type, &read<Type>>;

    return map;
});



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
    const auto elementTypeIdx = read<Byte>(stream);
    const auto reader = IndexToReaderMap.at(elementTypeIdx);

    auto name = readCString(stream);

    Element elem;
    elem.name = std::move(name);
    elem.value = reader(stream);

    return elem;
}

void writeElement(const Element& element, Ostream& stream)
{
    const size_t index = boost::apply_visitor(GetElementIndexVisitor{}, element.value);
    assert(index <= 255);
    write<Byte>(index, stream);

    writeCString(element.name, stream);

    boost::apply_visitor(WriteElementVisitor{stream}, element.value);
}

} // namespace Bson
