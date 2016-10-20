// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../String.hpp"

#define IDENTIFIER_TYPE  ':'
#define GROUP_OPEN       '{'
#define GROUP_CLOSE      '}'
#define ITEM_ASSIGNMENT  '='
#define ITEM_END         ';'
#define NEWLINE          '\n'

namespace util
{
namespace cml
{
    class Item;
    class Value;
    class Group;

    class Reader;
    class Writer;
}
}
