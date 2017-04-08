#include "Bson/bson.h"
#include "test-input.h"

#include <cassert>
#include <string>

bool test1()
{

    const auto input = getTest1Input();
    const auto result = Bson::encode(Bson::decode(input));
    const auto expected = input;

    return result == expected;
}

bool test2()
{
    const auto input = getTest2Input();
    const auto result = Bson::encode(Bson::decode(input));
    const auto expected = input;

    return result == expected;
}

int main()
{
    const bool result = test1() && test2();
    return result ? 0 : 1;
}
