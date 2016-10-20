// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "string/Types.hpp"
#include "string/Print.hpp"
#include <exception>

namespace util
{
    class Exception
    {
    public:
        string_t const message;

        Exception(Segment const& message);
        ~Exception() throw();

        void apply() const;
        void apply(Segment const& additional_message) const;

        char const* what() const throw();

	private:
		std::string const _what;
    };

    class DocumentException : public Exception
    {
    public:
        DocumentException(Segment const&    message,
                          Segment const&    name,
                          Segment const&    document,
                          Segment::Location current);
        DocumentException(Segment const&    message,
                          Segment const&    name,
                          unsigned          line);

        template <typename C>
        unsigned static getLineCount(SegmentT<C> const& segment, typename SegmentT<C>::Location current)
        {
            ASSERT(current <= segment.end);
            unsigned count = 1;
            for (typename SegmentT<C>::Location l = segment.begin; l != segment.end && l != current; ++l)
                if (*l == NEWLINE)
                    ++count;
            return count;
        }
    };

    class FileNotFoundException : public Exception
    {
    public:
        FileNotFoundException(Segment const& filename);
    };

    class InvalidSignalException : public Exception
    {
    public:
        InvalidSignalException();
    };
}
