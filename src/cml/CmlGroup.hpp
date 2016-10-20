// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "CmlItem.hpp"

#include <map>
#include <deque>
#include <memory>

namespace util
{
namespace cml
{
    class Group
    {
    public:
        typedef std::map<string_t,Item>   NamedItems;
        typedef std::vector<Item*>        FoundItems;
        typedef std::vector<Item const*>  FoundItemsConst;

        typedef std::map<string_t,Group*> NamedGroups;
        typedef std::vector<Group*>       AnonymousGroups;
        typedef std::vector<Group*>       FoundGroups;
        typedef std::vector<Group const*> FoundGroupsConst;

        Value type;

        Group();
        Group(Value const& value);
		Group(Group const& group);
		~Group();

        bool doesGroupExist(Segment const& name) const;
        bool doesItemExist (Segment const& name) const;

        bool tryGetNamedGroup(Segment const& name, Group& group) const;
        Group   getNamedGroup(Segment const& name) const;
        Group&  getNamedGroup(Segment const& name);
        Group&  getAnonymousGroup();

        bool tryGetNamedItem(Segment const& name, Item& item) const;
        Item    getNamedItem(Segment const& name) const;
        Item&   getNamedItem(Segment const& name);

        Group& addNamedGroup(Segment const& name, Group const& group);
        Group& addNamedGroup(Segment const& name, Segment const& type);
        Group& addNamedGroup(Segment const& name);
        Group& addAnonymousGroup();
        Group& addAnonymousGroup(Group const& group);
        Group& addAnonymousGroup(Segment const& type);

        Item& addNamedItem(Segment const& name, Item const& item);
        Item& addNamedItem(Segment const& name, Segment const& type);
        Item& addNamedItem(Segment const& name);

        void clear();
        bool empty() const;

        NamedItems&  getNamedItems();
        NamedGroups& getNamedGroups();
        AnonymousGroups& getAnonymousGroups();
        FoundItems getItems(Segment const& type);
        FoundGroups getGroups(Segment const& type);

        NamedItems const& getNamedItems() const;
        NamedGroups const& getNamedGroups() const;
        AnonymousGroups const& getAnonymousGroups() const;
        FoundItemsConst getItems(Segment const& type) const;
        FoundGroupsConst getGroups(Segment const& type) const;

        string_t str() const;
        bool tryParse(istream_t& is);
        bool tryParse(Segment const& s);

		Group& operator= (Group const& group);

        DEFINE_OBJECT_OSTREAM_OPERATOR(Group)
        DEFINE_OBJECT_ISTREAM_OPERATOR(Group)

    private:        
        NamedItems      items_named;
        NamedGroups     groups_named;
        AnonymousGroups groups_anonymous;

        static void printGroup(Group const& g, size_t const depth, ostream_t& os);
    };
}
}
