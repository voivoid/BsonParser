#pragma once

#include "Bson/Details/common.h"
#include <cstdint>
#include <string>
#include <vector>
#include <boost/variant.hpp>

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

using Element = boost::variant<Double, std::string, False, True, Int32, Uint64, Int64, Uint64, Decimal>;
Element readElement(Istream& stream);
void writeElement(const Element& element, Ostream& stream);

using List = std::vector<Element>;
List readList(Istream& stream);
void writeList(const List& list, Ostream& stream);


} // namespace Bson
