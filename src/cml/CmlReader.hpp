// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "../Types.hpp"
#include "../Exception.hpp"

namespace util
{
namespace cml
{
    class Reader
    {
    public:
        Reader(Segment const& segment);

		Group read() const;
        Group& read(Group& root) const;

    private:
        Segment segment;

        void implementation(Group& root) const;
	};
}
}
