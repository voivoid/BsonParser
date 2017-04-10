#include "Bson/Details/parsers.h"

#include "Bson/Details/calcsize.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

template <typename T>
size_t getValueSize(const T& value)
{
    return Bson::Details::calcValueSize(value);
}

template <typename T>
bool testMemory(const T& value)
{
    std::vector<Bson::Byte> data(getValueSize(value), 0);

    boost::iostreams::basic_array<Bson::Byte> arr(data.data(), data.size());
    boost::iostreams::stream<decltype(arr)> istream(arr);
    boost::iostreams::stream<decltype(arr)> ostream(arr);

    Bson::Details::write(value, ostream);
    const auto result = Bson::Details::read<T>(istream);

    return result == value;
}

template <typename T>
bool testStream(const T& val)
{
    std::basic_stringstream<Bson::Byte> ss(std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    Bson::Details::write(val, ss);
    const auto result = Bson::Details::read<T>(ss);

    return val == result;
}

template <typename T>
void test(const T& value)
{
    bool result = testMemory(value) && testStream(value);

    if(!result)
    {
        throw std::runtime_error("Test failed");
    }
}

int main()
{

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
    test<Bson::Int32>(0x00ff00ff);
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
    test<Bson::Double>(-0.1);
    test<Bson::Double>(1.0);
    test<Bson::Double>(-1.0);
    test<Bson::Double>(std::numeric_limits<Bson::Double>::min());
    test<Bson::Double>(std::numeric_limits<Bson::Double>::max());

    test<Bson::Decimal>(0.0);
    test<Bson::Decimal>(0.1);
    test<Bson::Decimal>(-0.1);
    test<Bson::Decimal>(1.0);
    test<Bson::Decimal>(-1.0);
    test<Bson::Decimal>(std::numeric_limits<Bson::Decimal>::min());
    test<Bson::Decimal>(std::numeric_limits<Bson::Decimal>::max());

    test(Bson::CString{{""}});
    test(Bson::CString{{"hello"}});
    test(Bson::CString{{"hello world"}});
    test(Bson::String(""));
    test(Bson::String("hello"));
    test(Bson::String("hello world"));
    test(Bson::String("hello\0world", 11));

    test(Bson::Element{{"test double"}, Bson::Double{42.0}});
    test(Bson::Element{{"test string"}, Bson::String{"Hello world"}});
    test(Bson::Element{{"test true"}, Bson::True{}});
    test(Bson::Element{{"test false"}, Bson::False{}});
    test(Bson::Element{{"test int32"}, Bson::Int32{42}});
    test(Bson::Element{{"test uint64"}, Bson::Uint64{42}});
    test(Bson::Element{{"test int64"}, Bson::Int64{42}});
    test(Bson::Element{{"test Decimal"}, Bson::Decimal{42.0}});

    test(Bson::List{});

    {
        Bson::List l;
        l.push_back(Bson::Element{{"element 1"}, Bson::Double{42.0}});
        test(l);
    }

    {
        Bson::List l;
        l.push_back(Bson::Element{{"element 1"}, Bson::String{"Hello world"}});
        l.push_back(Bson::Element{{"element 2"}, Bson::Int32{42}});
        l.push_back(Bson::Element{{"element 3"}, Bson::Int64{42}});
        test(l);
    }

    return 0;
}
