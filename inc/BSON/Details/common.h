#pragma once

#include <vector>
#include <iterator>

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

} // namespace Bson
