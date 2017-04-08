#include "Bson/bson.h"
#include "test-input.h"

#include <cassert>
#include <string>

bool test1()
{
    Bson::Document doc;
    doc.getList().push_back({{"hello"}, Bson::String{"world"}});
    const auto result = Bson::encode(doc);
    const auto expected = getTest1Input();

    return result == expected;
}

int main()
{
    const auto result = test1();
    return result ? 0 : -1;
}
