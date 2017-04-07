#include "Bson/parser.h"

#include <cassert>
#include <string>

int main()
{
    Bson::Document doc;
    doc._list.push_back({{"hello"}, Bson::String{"world"}});
    const auto result = Bson::encode(doc);
    const auto expected =
        Bson::Bytes{0x16, 0x00, 0x00, 0x00, 0x02, 'h', 'e', 'l', 'l', 'o', 0x00, 0x06, 0x00, 0x00, 0x00, 'w', 'o', 'r', 'l', 'd', 0x00, 0x00};

    return result == expected ? 0 : -1;
}
