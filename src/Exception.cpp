// Copyright 2013, James Mitchell, All rights reserved.

#include "Exception.hpp"
#include "Assertion.hpp"
using namespace util;

Exception::Exception(Segment const& message)
    : message(message.str()), _what(message.begin, message.end)
{}

Exception::~Exception() throw()
{}

void Exception::apply() const
{ println(message); }

void Exception::apply(Segment const& additional_message) const
{
    println(STR_LITERAL("#:#"),message,additional_message);
}

char const* Exception::what() const throw()
{
    return _what.c_str();
}

DocumentException::DocumentException(Segment const& message,
                                     Segment const& name,
                                     unsigned line)
    : Exception(sprint(STR_LITERAL("(# ln: #) #"), name, line, message))
{}

DocumentException::DocumentException(Segment const& message,
                                     Segment const& name,
                                     Segment const& document,
                                     Segment::Location current)
    : Exception(sprint(STR_LITERAL("(# ln: #) #"), name, getLineCount(document, current), message))
{}

FileNotFoundException::FileNotFoundException(Segment const& filename)
    : Exception(sprint(STR_LITERAL("file was not found : #"), filename))
{}

InvalidSignalException::InvalidSignalException()
    : Exception(STR_LITERAL("signal is invalid"))
{

}
