#pragma once

#include "Bson/Details/types.h"
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace Bson
{
namespace Details
{

template <typename T>
T read(Istream& stream);

template <typename T>
typename std::enable_if_t<std::is_pod<T>::value> write(T value, Ostream& stream);

template <>
String read(Istream& stream);
void write(const String& string, Ostream& stream);

template <>
True read(Istream& stream);
void write(True, Ostream& stream);

template <>
False read(Istream& stream);
void write(False, Ostream& stream);

template <>
CString read(Istream& stream);
void write(const CString& string, Ostream& stream);

template <>
Element read(Istream& stream);
void write(const Element& element, Ostream& stream);

template <>
List read(Istream& stream);
void write(const List& list, Ostream& stream);

template <>
Document read(Istream& stream);
void write(const Document& doc, Ostream& stream);
void write(const Document& doc, Ostream& stream, size_t documentSize);

} // namespace Details
} // namespace Bson
