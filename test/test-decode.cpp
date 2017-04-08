#include "Bson/bson.h"
#include "test-input.h"

bool test1()
{
    // clang-format on
    const auto document = Bson::decode(getTest1Input());
    const auto expectedElement = Bson::Element{{"hello"}, Bson::String{"world"}};

    const bool result = document.getList().size() == 1 && document.getList()[0] == expectedElement;

    return result;
}

bool test2()
{
    const auto document = Bson::decode(getTest2Input());
    const bool result = document.getList().size() == 1;

    return result;
}

int main()
{
    const bool result = test1() && test2();

    return result ? 0 : -1;
}
