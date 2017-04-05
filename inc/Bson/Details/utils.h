#pragma once

#include "Bson/Details/common.h"
#include <cstdint>
#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/hana/map.hpp>

namespace Bson
{

template <typename T>
T read(Istream& stream);

template <typename T>
void write(T value, Ostream& stream);

std::string readCString(Istream& stream);
std::string readString(Istream& stream);

void writeCString(const std::string& string, Ostream& stream);
void writeString(const std::string& string, Ostream& stream);

struct True {};
struct False {};

struct Element
{
    static constexpr auto TypeInfoMap = boost::hana::make_map(boost::hana::make_pair(boost::hana::type_c<Double>, 0x01),
                                                              boost::hana::make_pair(boost::hana::type_c<Int32>, 0x10));
    using Value = decltype(boost::hana::unpack(boost::hana::keys(TypeInfoMap), boost::hana::template_<boost::variant>))::type;

    std::string name;
    Value value;
};

Element readElement(Istream& stream);
void writeElement(const Element& element, Ostream& stream);

using List = std::vector<Element>;
List readList(Istream& stream);
void writeList(const List& list, Ostream& stream);


} // namespace Bson
