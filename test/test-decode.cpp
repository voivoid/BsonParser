#include "Bson/parser.h"

int main()
{

    const auto input =
        Bson::Bytes{0x16, 0x00, 0x00, 0x00, 0x02, 'h', 'e', 'l', 'l', 'o', 0x00, 0x06, 0x00, 0x00, 0x00, 'w', 'o', 'r', 'l', 'd', 0x00, 0x00};
    const auto document = Bson::decode(input);
    const auto expectedElement = Bson::Element{{"hello"}, Bson::String{"world"}};

    const bool result = document._list.size() == 1 && document._list[0] == expectedElement;

    return result ? 0 : -1;
}
