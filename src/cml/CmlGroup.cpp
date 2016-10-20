// Copyright 2013, James Mitchell, All rights reserved.

#include "CmlGroup.hpp"
#include "CmlReader.hpp"

#include "../Exception.hpp"
using namespace util;
using namespace util::cml;
using namespace std;

Group::Group()
{}

Group::Group(Value const& type)
    : type(type)
{}

Group::Group(Group const& group)
	: type(group.type), items_named(group.items_named), groups_named(), groups_anonymous(group.groups_anonymous.size())
{
	if (!group.groups_anonymous.empty())
		for (size_t i = 0; i < groups_anonymous.size(); ++i)
			groups_anonymous[i] = new Group(*group.groups_anonymous[i]);
	if (!group.groups_named.empty())
		for (NamedGroups::const_iterator i = group.groups_named.begin(); i != group.groups_named.end(); ++i)
			groups_named[i->first] = new Group(*i->second);
}

Group::~Group()
{
	clear();
}

bool Group::doesGroupExist(Segment const& name) const
{
    return groups_named.find(name.str()) != groups_named.end();
}

bool Group::doesItemExist(Segment const& name) const
{
    return items_named.find(name.str()) != items_named.end();
}

bool Group::tryGetNamedGroup(Segment const& name, Group& group) const
{
    NamedGroups::const_iterator const i = groups_named.find(name.str());
    if (i != groups_named.end())
    {
        group = *i->second;
        return true;
    }
    return false;
}

Group Group::getNamedGroup(Segment const& name) const
{
    NamedGroups::const_iterator const i = groups_named.find(name.str());
    if (i != groups_named.end())
        return *i->second;
    return Group();
}

Group& Group::getNamedGroup(Segment const& name)
{
    string_t const trimmed_name = Trim::trim(name).str();
    if (trimmed_name.empty())
        throw Exception(STR_LITERAL("cml group name cannot be empty"));

	NamedGroups::iterator const i = groups_named.find(trimmed_name);
	if (i == groups_named.end())
		return *(groups_named[trimmed_name] = new Group());
	return *i->second;
}

Group& Group::getAnonymousGroup()
{
    groups_anonymous.push_back(new Group());
    return *groups_anonymous.back();
}

Group::FoundGroups Group::getGroups(Segment const& type)
{
    FoundGroups found;
    for (AnonymousGroups::iterator i = groups_anonymous.begin(); i != groups_anonymous.end(); ++i)
        if (type.same((*i)->type.get<string_t>()))
            found.push_back(*i);
    for (NamedGroups::iterator i = groups_named.begin(); i != groups_named.end(); ++i)
        if (type.same(i->second->type.get<string_t>()))
            found.push_back(i->second);
    return found;
}

Group::FoundGroupsConst Group::getGroups(Segment const& type) const
{
    FoundGroupsConst found;
    for (AnonymousGroups::const_iterator i = groups_anonymous.begin(); i != groups_anonymous.end(); ++i)
        if (type.same((*i)->type.get<string_t>()))
            found.push_back(*i);
    for (NamedGroups::const_iterator i = groups_named.begin(); i != groups_named.end(); ++i)
        if (type.same(i->second->type.get<string_t>()))
            found.push_back(i->second);
    return found;
}

bool Group::tryGetNamedItem(Segment const& name, Item& item) const
{
    NamedItems::const_iterator const i = items_named.find(name.str());
    if (i != items_named.end())
    {
        item = i->second;
        return true;
    }
    return false;
}

Item Group::getNamedItem(Segment const& name) const
{
    NamedItems::const_iterator const i = items_named.find(name.str());
    if (i != items_named.end())
        return i->second;
    return Item();
}

Item& Group::getNamedItem(Segment const& name)
{
    Segment const trimmed_name = Trim::trim(name);
    if (trimmed_name.empty())
        throw Exception(STR_LITERAL("cml item name cannot be empty"));
    return items_named[trimmed_name.str()];
}

Group& Group::addNamedGroup(Segment const& name, Group const& group)  { return getNamedGroup(name) = group; }
Group& Group::addNamedGroup(Segment const& name, Segment const& type) { return getNamedGroup(name) = Group(type); }
Group& Group::addNamedGroup(Segment const& name)                      { return getNamedGroup(name) = Group(); }

Group& Group::addAnonymousGroup()                    { return getAnonymousGroup() = Group(); }
Group& Group::addAnonymousGroup(Group const& group)  { return getAnonymousGroup() = group; }
Group& Group::addAnonymousGroup(Segment const& type) { return getAnonymousGroup() = Group(type); }

Item& Group::addNamedItem(Segment const& name, Item const& item)    { return getNamedItem(name) = item; }
Item& Group::addNamedItem(Segment const& name, Segment const& type) { return getNamedItem(name) = Item(Segment(), type); }
Item& Group::addNamedItem(Segment const& name)                      { return getNamedItem(name) = Item(); }

Group::NamedGroups     const& Group::getNamedGroups    () const { return groups_named; }
Group::NamedItems      const& Group::getNamedItems     () const { return items_named; }
Group::AnonymousGroups const& Group::getAnonymousGroups() const { return groups_anonymous; }

void Group::clear()
{
	for (NamedGroups::iterator i = groups_named.begin(); i != groups_named.end(); ++i)
	{
		i->second->clear();
		delete i->second;
	}

	for (AnonymousGroups::iterator i = groups_anonymous.begin(); i != groups_anonymous.end(); ++i)
	{
		(*i)->clear();
		delete *i;
	}

    groups_named.clear();
    groups_anonymous.clear();
    items_named.clear();
}

bool Group::empty() const
{
    return groups_named.empty() && items_named.empty() && groups_anonymous.empty();
}

string_t Group::str() const
{
    ostringstream_t oss;
    printGroup(*this, 0, oss);
    return oss.str();
}

bool Group::tryParse(istream_t& is)
{
    try
    {
        Reader(streamString(is)).read(*this);
    }
    catch (DocumentException const&)
    {
        return false;
    }
    return true;
}

bool Group::tryParse(Segment const& s)
{
    try
    {
        Reader(s).read(*this);
    }
    catch (DocumentException const&)
    {
        return false;
    }
    return true;
}

Group& Group::operator = (Group const& group)
{
	clear();
	new (this) Group(group);
	return *this;
}

void Group::printGroup(Group const& group, size_t const depth, ostream_t& os)
{
    string_t const indent(depth*4, (char_t)' ');

	for (NamedGroups::const_iterator i = group.groups_named.begin(); i != group.groups_named.end(); ++i)
	{
		Group const& group = *i->second;

		os << indent << i->first;
		if (!group.type.empty())
            os << IDENTIFIER_TYPE << group.type.get<string_t>();
		os << ' ' << GROUP_OPEN << endl;
		printGroup(*i->second, depth + 1, os);
		os << endl << indent << GROUP_CLOSE << endl;
	}

	for (AnonymousGroups::const_iterator i = group.groups_anonymous.begin(); i != group.groups_anonymous.end(); ++i)
    {
        os << indent;
        if(!(*i)->type.empty())
            os << IDENTIFIER_TYPE << (*i)->type.get<string_t>();
        os << GROUP_OPEN << endl;
        printGroup(**i, depth+1, os);
        os << endl << indent << GROUP_CLOSE << endl;
    }

	if (!group.items_named.empty())
	{
        size_t max_identifier_size = 0;
        size_t max_type_size = 0;
        for (NamedItems::const_iterator i = group.items_named.begin(); i != group.items_named.end(); ++i)
        {
            if (i->second.type.size() > max_type_size)
                max_type_size = i->second.type.size();

            if (i->first.size() > max_identifier_size)
                max_identifier_size = i->first.size();
        }

		for (NamedItems::const_iterator i = group.items_named.begin(); i != group.items_named.end(); ++i)
		{
			Item const& item = i->second;
            string_t const type = item.type.empty() ? ""
            : sprint("##",IDENTIFIER_TYPE,item.type.get<string_t>());

            os << indent << left << setw(max_identifier_size) << i->first;
            os << left << setw(max_type_size+1) << type;
			os << ITEM_ASSIGNMENT << item << ITEM_END << endl;
		}
	}
}
