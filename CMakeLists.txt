cmake_minimum_required(VERSION 3.7)
project(BsonParser LANGUAGES CXX)

add_library(BsonParser STATIC
    inc/Bson/Details/common.h
    inc/Bson/parser.h
    src/parser.cpp
    inc/Bson/Details/utils.h
    src/utils.cpp
)

find_package(Boost 1.63.0 REQUIRED)
target_link_libraries(BsonParser Boost::boost)

target_include_directories(BsonParser PUBLIC inc)

if(NOT MSVC)
    target_compile_options(BsonParser PUBLIC -Wall -Wextra -Werror -pedantic -std=c++1z)
endif()

enable_testing()

add_executable(BsonParserTestUtils test/test-utils.cpp)
target_link_libraries(BsonParserTestUtils PRIVATE BsonParser)

add_executable(BsonParserTestEncode test/test-encode.cpp)
target_link_libraries(BsonParserTestEncode PRIVATE BsonParser)

add_executable(BsonParserTestDecode test/test-decode.cpp)
target_link_libraries(BsonParserTestDecode PRIVATE BsonParser)

add_executable(BsonParserTestEncodeDecode test/test-encode-decode.cpp)
target_link_libraries(BsonParserTestEncodeDecode PRIVATE BsonParser)

add_test(NAME BsonParserUtils COMMAND BsonParserTestUtils)
add_test(NAME BsonParserEncode COMMAND BsonParserTestDecode)
add_test(NAME BsonParserDecode COMMAND BsonParserTestEncode)
add_test(NAME BsonParserEncodeDecode COMMAND BsonParserTestEncodeDecode)

