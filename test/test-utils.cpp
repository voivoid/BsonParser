#include "Bson/Details/utils.h"

#include <cassert>
#include <sstream>
#include <iterator>
#include <limits>
#include <iostream>
#include <string>
#include <vector>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

template <typename T, typename WriteFunc, typename ReadFunc>
bool testMemory(const T& value, size_t buffSize, WriteFunc write, ReadFunc read)
{
    std::vector<Bson::Byte> data(buffSize, 0);

    boost::iostreams::basic_array<Bson::Byte> arr(data.data(), data.size());
    boost::iostreams::stream<boost::iostreams::basic_array<Bson::Byte>> istream(arr);
    boost::iostreams::stream<boost::iostreams::basic_array<Bson::Byte>> ostream(arr);

    write(value, ostream);
    const auto result = read(istream);

    return result == value;
}

template <typename T, typename WriteFunc, typename ReadFunc>
bool testStream(T val, WriteFunc write, ReadFunc read)
{
    std::basic_stringstream<Bson::Byte> ss(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    write(val, ss);
    const auto result = read(ss);

    return val == result;
}

template <typename T>
void test(T value)
{
    assert(testMemory(value, sizeof(value), Bson::write<T>, Bson::read<T>));
    assert(testStream(value, Bson::write<T>, Bson::read<T>));
}

void testString(const std::string& value)
{
    assert(testMemory(value, value.length() + sizeof(Bson::Int32) + 1, Bson::writeString, Bson::readString));
    assert(testStream(value, Bson::writeString, Bson::readString));
}

void test(const std::string& value)
{
    assert(testMemory(value, value.length() + 1, Bson::writeCString, Bson::readCString));
    assert(testStream(value, Bson::writeCString, Bson::readCString));
    testString(value);
}

int main() {

    test<Bson::Byte>(0);
    test<Bson::Byte>(1);
    test<Bson::Byte>(42);
    test<Bson::Byte>(255);

    test<Bson::Int32>(0);
    test<Bson::Int32>(1);
    test<Bson::Int32>(-1);
    test<Bson::Int32>(42);
    test<Bson::Int32>(-42);
    test<Bson::Int32>(0x01020304);
    test<Bson::Int32>(0xff0000ff);
    test<Bson::Int32>(std::numeric_limits<Bson::Int32>::min());
    test<Bson::Int32>(std::numeric_limits<Bson::Int32>::max());

    test<Bson::Int64>(0);
    test<Bson::Int64>(1);
    test<Bson::Int64>(-1);
    test<Bson::Int64>(42);
    test<Bson::Int64>(-42);
    test<Bson::Int64>(0x01020304);
    test<Bson::Int64>(std::numeric_limits<Bson::Int64>::min());
    test<Bson::Int64>(std::numeric_limits<Bson::Int64>::max());

    test<Bson::Uint64>(0);
    test<Bson::Uint64>(1);
    test<Bson::Uint64>(42);
    test<Bson::Uint64>(0x01020304);
    test<Bson::Uint64>(std::numeric_limits<Bson::Uint64>::min());
    test<Bson::Uint64>(std::numeric_limits<Bson::Uint64>::max());

    test<Bson::Double>(0.0);
    test<Bson::Double>(0.1);
    test<Bson::Double>(0.-1);
    test<Bson::Double>(1.0);
    test<Bson::Double>(-1.0);
    test<Bson::Double>(std::numeric_limits<Bson::Double>::min());
    test<Bson::Double>(std::numeric_limits<Bson::Double>::max());

    test<Bson::Decimal>(0.0);
    test<Bson::Decimal>(0.1);
    test<Bson::Decimal>(0.-1);
    test<Bson::Decimal>(1.0);
    test<Bson::Decimal>(-1.0);
    test<Bson::Decimal>(std::numeric_limits<Bson::Decimal>::min());
    test<Bson::Decimal>(std::numeric_limits<Bson::Decimal>::max());

    test(std::string(""));
    test(std::string("hello"));
    test(std::string("hello world"));
    testString(std::string("hello\0world", 11));

    return 0;
}
