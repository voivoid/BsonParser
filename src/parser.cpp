#include "Bson/parser.h"

#include <cassert>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

namespace Bson
{

size_t calcDocumentSize(const Document&)
{
    // TODO: calc size
    return 0x16;
}

std::vector<Byte> encode(const Document& document)
{
    const auto documentSize = calcDocumentSize(document);
    std::vector<Bson::Byte> data(documentSize, 0);

    boost::iostreams::basic_array_sink<Byte> arr(data.data(), data.size());
    boost::iostreams::stream<decltype(arr)> ostream(arr);

    write(static_cast<Int32>(documentSize), ostream);
    write(document._list, ostream);
    write(static_cast<Byte>(0), ostream);

    return data;
}

Document decode(const std::vector<Byte>& data)
{
    boost::iostreams::basic_array_source<Byte> arr(data.data(), data.size());
    boost::iostreams::stream<decltype(arr)> istream(arr);

    /*const Int32 documentSize = */ read<Int32>(istream);

    Document document;
    document._list = read<List>(istream);
    const auto endByte = read<Byte>(istream);
    assert(endByte == 0);

    return document;
}

} // namespace Bson
