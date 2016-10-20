// Copyright 2014, James Mitchell, All rights reserved.

#include "PlyDocument.hpp"
#include "../Exception.hpp"
#include "../string/Print.hpp"
using namespace util::ply;
using namespace std;

#define ERR_BAD_ELEMENT STR_LITERAL("element buffer not found: %s")

ElementBuffer::Element::Element(unsigned const begin, unsigned const end)
    : begin(begin), end(end)
{}

ElementBuffer::ElementBuffer(ElementDefinition* element)
    : definition(element)
{}

bool ElementBuffer::operator < (ElementBuffer const& e) const
{ return definition < e.definition; }

ElementDefinitionFinder::ElementDefinitionFinder(util::Segment const& name, unsigned size)
{
	ed.name = name.str();
	ed.size = size;
}

bool ElementDefinitionFinder::isEqualStrict(ElementDefinition const& ed) const
{
    return this->ed.name == ed.name && this->ed.size == ed.size && this->ed.properties == ed.properties;
}

bool ElementDefinitionFinder::isEqualRelaxed(ElementDefinition const& _ed) const
{
    if (ed.name != _ed.name)                          return false;
    if (ed.size > 0 && ed.size != _ed.size)           return false;
    if (ed.properties.size() > _ed.properties.size()) return false;

	for (unsigned i = 0; i < std::min(ed.properties.size(),_ed.properties.size()); ++i)
	{
		PropertyDefinition const& pa = ed.properties[i];
		PropertyDefinition const& pb = _ed.properties[i];
        if (pa.name != pb.name)                                     return false;
        if (pa.type != ANY && pa.type != pb.type)                   return false;
        if (pa.type_index != ANY && pa.type_index != pb.type_index) return false;
	}
	return true;
}

ElementDefinitionFinder& ElementDefinitionFinder::addPropertyList(util::Segment const& name, NumberType type, NumberType type_index)
{
    ed.properties.push_back({name.str(), type, type_index});
	return *this;
}

ElementDefinitionFinder& ElementDefinitionFinder::addProperty(util::Segment const& name, NumberType type)
{
	return addPropertyList(name, type, INVALID);
}

ElementBuffer const& ElementDefinitionFinder::findStrict(Document const& document) const
{
	for (Document::Content::const_iterator i = document.content.begin(); i != document.content.end(); ++i)
		if (isEqualStrict(*i->definition))
			return *i;
	throw Exception(sprintStdln(STR_LITERAL("element buffer not found: %s"), ed.name.c_str()));
    return *document.content.begin();
}

ElementBuffer const& ElementDefinitionFinder::findRelaxed(Document const& document) const
{
	for (Document::Content::const_iterator i = document.content.begin(); i != document.content.end(); ++i)
		if (isEqualRelaxed(*i->definition))
			return *i;
	throw Exception(sprintStdln(STR_LITERAL("element buffer not found: %s"), ed.name.c_str()));
    return *document.content.begin();
}
