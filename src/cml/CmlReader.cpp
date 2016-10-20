// Copyright 2013, James Mitchell, All rights reserved.

#include "CmlReader.hpp"
#include "CmlGroup.hpp"
#include "CmlItem.hpp"
#include "../String.hpp"

#include <map>
#include <stack>

using namespace util;
using namespace util::cml;
using namespace std;

#define ERR_UNASSCOCIATED_IDENTIFIER STR_LITERAL("identifier has no associated group or item")
#define ERR_NO_IDENTIFIER            STR_LITERAL("no identifier specified")

#define ERR_UNEXPECTED_EOF   STR_LITERAL("unexpected end of document")
#define ERR_UNEXPECTED_ITEM  STR_LITERAL("unexpected item")
#define ERR_UNEXPECTED_GROUP STR_LITERAL("unexpected closing brace")

#define DOC_NAME STR_LITERAL("cml")

#define EXCEPTION(err) DocumentException(literal(err), DOC_NAME, segment, i)

bool isReservedChar(char_t const c)
{
    return c == GROUP_OPEN      ||
           c == GROUP_CLOSE     ||
           c == ITEM_ASSIGNMENT ||
           c == ITEM_END        ||
           c == IDENTIFIER_TYPE;
}

Reader::Reader(Segment const& segment)
    : segment(segment)
{}

void Reader::implementation(Group& root) const
{
	stack<Segment> types;
    stack<Segment> identifiers;
    stack<Group*>  groups;

    char_t            c = *segment.begin;
    Segment::Location i =  segment.begin;

	groups.push(&root);

#define SEEK(exp) { while (i != segment.end && exp) c = *++i; }
    while (i != segment.end)
    {
        SEEK(isspace(c))

        if (i == segment.end)
			break;
        else if (c == GROUP_OPEN)
        {
			if (identifiers.size() > 1)
				throw EXCEPTION(ERR_UNASSCOCIATED_IDENTIFIER);

			if (!identifiers.empty())
			{
				if (!types.empty())
				{
					groups.push(&groups.top()->addNamedGroup(identifiers.top(), types.top()));
					types.pop();
				}
				else 
					groups.push(&groups.top()->addNamedGroup(identifiers.top()));
				identifiers.pop();
			}
			else
			{
				if (!types.empty())
				{
					groups.push(&groups.top()->addAnonymousGroup(types.top()));
					types.pop();
				}
				else
					groups.push(&groups.top()->addAnonymousGroup());
			}
        }
        else if (c == GROUP_CLOSE)
        {
            if (groups.size() <= 1)
				throw EXCEPTION(ERR_UNEXPECTED_GROUP);
            groups.pop();
        }
        else if (c == ITEM_END)
        {
            throw EXCEPTION(ERR_UNEXPECTED_ITEM);
        }
        else if (c == ITEM_ASSIGNMENT)
        {
            Segment::Location const value_begin = i+1;

			SEEK(c != ITEM_END)

			if (i == segment.end)
				throw EXCEPTION(ERR_UNEXPECTED_EOF);

			if (identifiers.size() > 1)
				throw EXCEPTION(ERR_UNASSCOCIATED_IDENTIFIER);

            if (identifiers.empty())
                throw EXCEPTION(ERR_NO_IDENTIFIER);

			if (!types.empty())
			{
				groups.top()->addNamedItem(identifiers.top(), Item(Trim::trim(Segment(value_begin, i)), types.top()));
				types.pop();
			}
			else
				groups.top()->addNamedItem(identifiers.top(), Item(Trim::trim(Segment(value_begin, i))));
			identifiers.pop();
        }
        else if (c == IDENTIFIER_TYPE)
		{
			c = *++i;
			SEEK(isspace(c))

			Segment::Location const type_begin = i;
            SEEK(!isReservedChar(c) && !isspace(c))
			if (i == segment.end)
				throw EXCEPTION(ERR_UNEXPECTED_EOF);

			if (type_begin < i)
            {
				types.push(Segment(type_begin, i));
                continue;
            }
		}
        else
        {
            Segment::Location const identifier_begin = i;

            SEEK(!isReservedChar(c) && !isspace(c))
			if (i == segment.end)
				throw EXCEPTION(ERR_UNEXPECTED_EOF);
			if (identifier_begin < i)
            {
				identifiers.push(segment.sub(identifier_begin, i));
                continue;
            }
        }

        c = *++i;
    }

	if (groups.size() > 1 || !types.empty() || !identifiers.empty())
		throw EXCEPTION(ERR_UNEXPECTED_EOF);

#undef SEEK
}

Group Reader::read() const
{
    Group root;
    implementation(root);
    return root;
}

Group& Reader::read(Group& root) const
{
    implementation(root);
    return root;
}
