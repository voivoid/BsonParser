#pragma once

#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include <boost/hana/map.hpp>
#include <boost/variant.hpp>

namespace Bson
{

using Byte = unsigned char;
using Int32 = std::int32_t;
using Int64 = std::int64_t;
using Uint64 = std::uint64_t;
using Double = double;

#ifdef __GNUC__
using Decimal = __float128;
#else
#error "Unsupported platform"
#endif

using Bytes = std::vector<Byte>;

using Istream = std::basic_istream<Byte>;
using Ostream = std::basic_ostream<Byte>;

using String = std::string;

struct CString
{
    bool operator==(const CString& rhs) const
    {
        return s == rhs.s;
    }
    std::string s;
};

struct True
{
    bool operator==(const True&) const
    {
        return true;
    }
};
struct False
{
    bool operator==(const False&) const
    {
        return true;
    }
};

struct Element;
using List = std::vector<Element>;

class Document
{
public:
    Document();
    ~Document();

    Document(Document&&) = default;
    Document& operator=(Document&&) = default;

    List& getList()
    {
        return *_list;
    }
    const List& getList() const
    {
        return *_list;
    }

    bool operator==(const Document& rhs) const
    {
        return getList() == rhs.getList();
    }

private:
    std::unique_ptr<List> _list;
};

struct Element
{
    static constexpr auto TypeInfoMap = boost::hana::make_map(boost::hana::make_pair(boost::hana::type_c<Double>, 0x01),
                                                              boost::hana::make_pair(boost::hana::type_c<String>, 0x02),
                                                              boost::hana::make_pair(boost::hana::type_c<Document>, 0x04),
                                                              boost::hana::make_pair(boost::hana::type_c<False>, 0x08),
                                                              boost::hana::make_pair(boost::hana::type_c<True>, 0x09),
                                                              boost::hana::make_pair(boost::hana::type_c<Int32>, 0x10),
                                                              boost::hana::make_pair(boost::hana::type_c<Uint64>, 0x11),
                                                              boost::hana::make_pair(boost::hana::type_c<Int64>, 0x12),
                                                              boost::hana::make_pair(boost::hana::type_c<Decimal>, 0x13));
    using Value = decltype(boost::hana::unpack(boost::hana::keys(TypeInfoMap), boost::hana::template_<boost::variant>))::type;

    bool operator==(const Element& rhs) const
    {
        return name == rhs.name && value == rhs.value;
    }

    CString name;
    Value value;
};

inline Document::Document()
    : _list(new List())
{
}

inline Document::~Document()
{
}

} // namespace Bson
