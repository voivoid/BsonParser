#pragma once

#include "Bson/Details/parsers.h"
#include "Bson/Details/types.h"

#include <string>

namespace Bson
{

Bytes encode(const Document&);
Document decode(const Bytes&);

} // namespace Bson
