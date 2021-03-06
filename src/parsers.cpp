#include "Bson/Details/parsers.h"

#include "Bson/Details/calcsize.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <map>

#include <boost/hana/at_key.hpp>
#include <boost/hana/fold_left.hpp>

#define UNUSED __attribute__((unused))

namespace Bson
{
namespace Details
{

namespace
{

const size_t MaxCStringBuff = 16384;

struct GetElementIndexVisitor : boost::static_visitor<Byte>
{
    template <typename T>
    Byte operator()(const T&) const
    {
        return Element::TypeInfoMap[boost::hana::type_c<T>];
    }
};

struct WriteElementVisitor : boost::static_visitor<void>
{
    WriteElementVisitor(Ostream& stream)
        : stream(stream)
    {
    }

    template <typename T>
    void operator()(const T& value) const
    {
        write(value, stream);
    }

private:
    Ostream& stream;
};

bool hasElement(Istream& stream)
{
    const auto peek = stream.peek();
    return peek != std::char_traits<Byte>::eof() && peek != 0;
}

template <typename T, T (*reader)(Istream&)>
Element::Value adaptReader(Istream& stream)
{
    return {reader(stream)};
}

using Reader = Element::Value (*)(Istream&);
}

const auto IndexToReaderMap = boost::hana::fold_left(Element::TypeInfoMap, std::map<int, Reader>{}, [](auto map, const auto pair) {
    const auto type = boost::hana::first(pair);
    const auto index = boost::hana::second(pair);

    using Type = typename decltype(type)::type;

    map[index] = &adaptReader<Type, &read<Type>>;

    return map;
});

// TODO: handle endianness

template <typename T>
T read(Istream& stream)
{
    static_assert(std::is_pod<T>::value);

    T value;
    stream.read(reinterpret_cast<Byte*>(&value), sizeof(value));
    return value;
}

template <>
String read(Istream& stream)
{
    Int32 strLen = read<Int32>(stream);

    assert(strLen > 0);

    String s;
    s.resize(static_cast<size_t>(strLen) - 1);
    stream.read(reinterpret_cast<Byte*>(&s[0]), strLen - 1);
    stream.ignore(1);

    return s;
}

template <>
CString read(Istream& stream)
{
    Byte buff[MaxCStringBuff];
    stream.getline(buff, MaxCStringBuff, 0);

    return {String(reinterpret_cast<char*>(buff))};
}

template <>
False read(Istream& stream)
{
    UNUSED const auto byte = read<Byte>(stream);
    assert(byte == 0x00);

    return {};
}

template <>
True read(Istream& stream)
{
    UNUSED const auto byte = read<Byte>(stream);
    assert(byte == 0x01);

    return {};
}

template <>
Element read(Istream& stream)
{
    const auto elementTypeIdx = read<Byte>(stream);
    const auto reader = IndexToReaderMap.at(elementTypeIdx);

    auto name = read<CString>(stream);

    Element elem;
    elem.name = std::move(name);
    elem.value = reader(stream);

    return elem;
}

template <>
List read(Istream& stream)
{
    List elements;

    while (hasElement(stream))
    {
        elements.push_back(read<Element>(stream));
    }

    return elements;
}

template <>
Document read(Istream& stream)
{
    UNUSED const Int32 documentSize = Details::read<Int32>(stream);
    assert(documentSize >= static_cast<Int32>(sizeof(Int32) + sizeof(Byte)));

    Document document;
    document.getList() = Details::read<List>(stream);
    UNUSED const auto endByte = Details::read<Byte>(stream);
    assert(endByte == 0);

    return document;
}

template <typename T>
typename std::enable_if_t<std::is_pod<T>::value> write(const T value, Ostream& stream)
{
    static_assert(std::is_pod<T>::value);
    stream.write(reinterpret_cast<const Byte*>(&value), sizeof(value));
}

void write(const String& string, Ostream& stream)
{
    write(static_cast<Int32>(string.length() + 1), stream);
    stream.write(reinterpret_cast<const Byte*>(string.c_str()), string.length() + 1);
}

void write(const CString& string, Ostream& stream)
{
    stream.write(reinterpret_cast<const Byte*>(string.s.c_str()), string.s.length() + 1);
}

void write(True, Ostream& stream)
{
    write<Byte>(0x01, stream);
}

void write(False, Ostream& stream)
{
    write<Byte>(0x00, stream);
}

void write(const Element& element, Ostream& stream)
{
    const Byte index = boost::apply_visitor(GetElementIndexVisitor{}, element.value);
    write<Byte>(index, stream);
    write(element.name, stream);

    boost::apply_visitor(WriteElementVisitor{stream}, element.value);
}

void write(const List& list, Ostream& stream)
{
    for (const auto& elem : list)
    {
        write(elem, stream);
    }
}

void write(const Document& document, Ostream& stream)
{
    write(document, stream, Details::calcValueSize(document));
}

void write(const Document& document, Ostream& stream, const size_t documentSize)
{
    Details::write(static_cast<Int32>(documentSize), stream);
    Details::write(document.getList(), stream);
    Details::write(static_cast<Byte>(0), stream);
}

#define BSON_INSTANTIATE_FUNCS(type)    \
     template type read(Istream& iter); \
     template void write(const type value, Ostream& iter)

BSON_INSTANTIATE_FUNCS(Byte);
BSON_INSTANTIATE_FUNCS(Int32);
BSON_INSTANTIATE_FUNCS(Int64);
BSON_INSTANTIATE_FUNCS(Uint64);
BSON_INSTANTIATE_FUNCS(Double);
BSON_INSTANTIATE_FUNCS(Decimal);

} // namespace Details
} // namespace Bson
