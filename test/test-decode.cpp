#include "Bson/parser.h"

int main()
{

    const auto input = Bson::Bytes{0x16, 0x00, 0x00, 0x00, 0x02, 'h', 'e', 'l', 'l', 'o', 0x00, 0x06, 0x00, 0x00, 0x00, 'w', 'o', 'r', 'l', 'd', 0x00, 0x00};
    Bson::decode(input);

    return 0;
}
