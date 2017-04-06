#pragma once

#include "Bson/Details/common.h"
#include <cstdint>
#include <string>
#include <vector>
#include <type_traits>

#include <boost/variant.hpp>
#include <boost/hana/map.hpp>

namespace Bson
{

struct Element
{
    static constexpr auto TypeInfoMap = boost::hana::make_map(boost::hana::make_pair(boost::hana::type_c<Double>, 0x01),
                                                              boost::hana::make_pair(boost::hana::type_c<String>, 0x02),
                                                              boost::hana::make_pair(boost::hana::type_c<False>, 0x08),
                                                              boost::hana::make_pair(boost::hana::type_c<True>, 0x09),
                                                              boost::hana::make_pair(boost::hana::type_c<Int32>, 0x10),
                                                              boost::hana::make_pair(boost::hana::type_c<Uint64>, 0x11),
                                                              boost::hana::make_pair(boost::hana::type_c<Int64>, 0x12),
                                                              boost::hana::make_pair(boost::hana::type_c<Decimal>, 0x13));
    using Value = decltype(boost::hana::unpack(boost::hana::keys(TypeInfoMap), boost::hana::template_<boost::variant>))::type;

    bool operator==(const Element& rhs) const { return name == rhs.name && value == rhs.value; }

    CString name;
    Value value;
};

using List = std::vector<Element>;

template <typename T>
T read(Istream& stream);

template <typename T>
typename std::enable_if_t<std::is_pod<T>::value> write(T value, Ostream& stream);
void write(const String& string, Ostream& stream);
void write(True, Ostream& stream);
void write(False, Ostream& stream);
void write(const CString& string, Ostream& stream);
void write(const Element& element, Ostream& stream);
void write(const List& list, Ostream& stream);


} // namespace Bson
