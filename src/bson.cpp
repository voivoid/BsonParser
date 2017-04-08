#include "Bson/bson.h"

#include "Bson/Details/calcsize.h"

#include <cassert>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace Bson
{

std::vector<Byte> encode(const Document& document)
{
    const auto documentSize = Details::calcValueSize(document);
    std::vector<Bson::Byte> data(documentSize, 0);

    boost::iostreams::basic_array_sink<Byte> arr(data.data(), data.size());
    boost::iostreams::stream<decltype(arr)> ostream(arr);

    Details::write(document, ostream, documentSize);
    assert(data.back() == 0);

    return data;
}

Document decode(const std::vector<Byte>& data)
{
    boost::iostreams::basic_array_source<Byte> arr(data.data(), data.size());
    boost::iostreams::stream<decltype(arr)> istream(arr);

    return Details::read<Document>(istream);
}

} // namespace Bson
