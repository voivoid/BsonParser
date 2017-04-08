#pragma once

#include "Bson/Details/parsers.h"
#include "Bson/Details/types.h"

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>

namespace Bson
{

namespace Details
{

class SizeGetterSink
{
public:
    using char_type = Byte;
    using category = boost::iostreams::sink_tag;

    SizeGetterSink()
        : _byteCounter(0)
    {
    }

    std::streamsize write(const Byte* /*s*/, std::streamsize n)
    {
        _byteCounter += n;
        return n;
    }

    std::streamsize getSize() const
    {
        return _byteCounter;
    }

private:
    std::streamsize _byteCounter;
};

template <typename T>
inline std::streamsize calcValueSize(const T& value)
{
    SizeGetterSink sink;
    boost::iostreams::stream<decltype(sink)> stream(sink);

    Bson::Details::write(value, stream);
    stream.flush();

    return (*stream).getSize();
}

inline std::streamsize calcValueSize(const Document& document)
{
    return calcValueSize(document.getList()) + sizeof(Int32) + sizeof(Byte);
}

} // namespace Details
} // namespace Bson
