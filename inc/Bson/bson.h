#pragma once

#include "Bson/Details/types.h"
#include "Bson/Details/parsers.h"

#include <string>

namespace Bson
{

Bytes encode(const Document&);
Document decode(const Bytes&);

} // namespace Bson
