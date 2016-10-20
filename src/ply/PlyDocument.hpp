// Copyright 2014, James Mitchell, All rights reserved.

#pragma once

#include "PlyDefinition.hpp"

#include <set>
#include <vector>

namespace util
{
namespace ply
{    
    struct ElementBuffer
    {
		typedef unsigned Index;

        static constexpr const Index ELEMENT_MAX = (unsigned)-1;

		struct Element
		{ 
			Index begin, end;
            Element(unsigned const begin, unsigned const end);
		};

        typedef std::vector<Element> Elements;
        typedef std::vector<Number> Buffer;

        ElementDefinition* definition;
        Buffer             buffer;
		Elements           elements;

        ElementBuffer(ElementDefinition* definition);
        bool operator < (ElementBuffer const& e) const;
    };

    struct Document
    {
        typedef std::vector<ElementBuffer> Content;
        HeaderDefinition definition;
        Content          content;
    };

	class ElementDefinitionFinder
	{
		ElementDefinition ed;
		
	public:
        ElementDefinitionFinder(util::Segment const& name, unsigned const size = 0);
		bool isEqualStrict(ElementDefinition const& ed) const;
		bool isEqualRelaxed(ElementDefinition const& ed) const;
        ElementDefinitionFinder& addProperty(util::Segment const& name, NumberType const type=ANY);
        ElementDefinitionFinder& addPropertyList(util::Segment const& name, NumberType const type=ANY, NumberType const type_index=ANY);
		ElementBuffer const& findStrict(Document const& document) const;
		ElementBuffer const& findRelaxed(Document const& document) const;
	};
}
}


