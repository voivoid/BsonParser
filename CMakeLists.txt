cmake_minimum_required(VERSION 3.7)
project(BsonParser LANGUAGES CXX)

add_library(BsonParser STATIC
    inc/Bson/Details/types.h
    inc/Bson/Details/calcsize.h
    inc/Bson/bson.h
    src/bson.cpp
    inc/Bson/Details/parsers.h
    src/parsers.cpp
)

find_package(Boost 1.63.0 REQUIRED COMPONENTS filesystem)
target_link_libraries(BsonParser Boost::boost)

target_include_directories(BsonParser PUBLIC inc)
target_compile_options(BsonParser PUBLIC -Wall -Wextra -Werror -pedantic -std=c++1z)

add_executable(BsonParserApp src/app/main.cpp)
target_link_libraries(BsonParserApp BsonParser Boost::boost Boost::filesystem)

enable_testing()

add_executable(BsonParserTestParsers test/test-parsers.cpp)
target_link_libraries(BsonParserTestParsers PRIVATE BsonParser)

add_executable(BsonParserTestEncode test/test-encode.cpp
                                    test/test-input.h)
target_link_libraries(BsonParserTestEncode PRIVATE BsonParser)

add_executable(BsonParserTestDecode test/test-decode.cpp
                                    test/test-input.h)
target_link_libraries(BsonParserTestDecode PRIVATE BsonParser)

add_executable(BsonParserTestEncodeDecode test/test-encode-decode.cpp
                                          test/test-input.h)
target_link_libraries(BsonParserTestEncodeDecode PRIVATE BsonParser)

add_test(NAME BsonParserUtils COMMAND BsonParserTestUtils)
add_test(NAME BsonParserEncode COMMAND BsonParserTestDecode)
add_test(NAME BsonParserDecode COMMAND BsonParserTestEncode)
add_test(NAME BsonParserEncodeDecode COMMAND BsonParserTestEncodeDecode)

