#pragma once

#include "Bson/Details/common.h"
#include "Bson/Details/utils.h"

#include <string>

namespace Bson
{

class Document
{
public:
    List _list;
};

Bytes encode(const Document&);
Document decode(const Bytes&);

} // namespace Bson
