// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "Format.hpp"
#include "Segment.hpp"
#include "print/PrintUtil.hpp"
#include "print/PrintFormat.hpp"

#define NEWLINE '\n'

#define RESULT_STRING(exp) (util::string_t(#exp " = ") + util::toString(exp))

namespace util
{
    inline Alignment align(size_t const size)                                                      { return Alignment(size);                }
    inline Alignment align(size_t const size, char_t const fill)                                   { return Alignment(size, fill);          }
    inline Alignment align(size_t const size, Alignment::Justify const justify)                    { return Alignment(size, justify);       }
    inline Alignment align(size_t const size, Alignment::Justify const justify, char_t const fill) { return Alignment(size, justify, fill); }

    inline Alignment alignLeft(size_t const size) { return Alignment(size, Alignment::Justify::LEFT); }
    inline Alignment alignLeft(size_t const size, char_t const fill) { return Alignment(size, Alignment::Justify::LEFT, fill); }

    inline Alignment alignRight(size_t const size) { return Alignment(size, Alignment::Justify::RIGHT); }
    inline Alignment alignRight(size_t const size, char_t const fill) { return Alignment(size, Alignment::Justify::RIGHT, fill); }

    inline Alignment alignCenter(size_t const size) { return Alignment(size, Alignment::Justify::CENTER); }
    inline Alignment alignCenter(size_t const size, char_t const fill) { return Alignment(size, Alignment::Justify::CENTER, fill); }

    // -- print

    template <typename... Params> inline void     print   (Segment const& fmt, Params... params) { Format(fmt).params(std::forward<Params>(params)...).print(standardout); }
    template <typename... Params> inline void     println (Segment const& fmt, Params... params) { print(fmt,std::forward<Params>(params)...); standardout << NEWLINE; }
    template <typename... Params> inline void     error   (Segment const& fmt, Params... params) { Format(fmt).params(std::forward<Params>(params)...).print(standarderr); }
    template <typename... Params> inline void     errorln (Segment const& fmt, Params... params) { error(fmt,std::forward<Params>(params)...); standarderr << NEWLINE; }
    template <typename... Params> inline string_t sprint  (Segment const& fmt, Params... params) { ostringstream_t ss; Format(fmt).params(std::forward<Params>(params)...).print(ss); return ss.str(); }
    template <typename... Params> inline string_t sprintln(Segment const& fmt, Params... params) { ostringstream_t ss; Format(fmt).params(std::forward<Params>(params)...).print(ss); ss << NEWLINE; return ss.str(); }
    template <typename... Params> inline void     fprint  (ostream_t& os, Segment const& fmt, Params... params) { Format(fmt).params(std::forward<Params>(params)...).print(os); }
    template <typename... Params> inline void     fprintln(ostream_t& os, Segment const& fmt, Params... params) { Format(fmt).params(std::forward<Params>(params)...).print(os); os << NEWLINE; }

    // -- Tuple

    template <typename... Params> inline void     printTuple   (Params... params) { buildTuple(standardout, std::forward<Params>(params)...); }
    template <typename... Params> inline void     printTupleln (Params... params) { printTuple(std::forward<Params>(params)...); standardout << NEWLINE; }
    template <typename... Params> inline void     errorTuple   (Params... params) { buildTuple(standarderr, std::forward<Params>(params)...); }
    template <typename... Params> inline void     errorTupleln (Params... params) { errorTuple(std::forward<Params>(params)...); standarderr << NEWLINE; }
    template <typename... Params> inline string_t sprintTuple  (Params... params) { ostringstream_t ss; buildTuple(ss, std::forward<Params>(params)...); return ss.str(); }
    template <typename... Params> inline string_t sprintTupleln(Params... params) { ostringstream_t ss; buildTuple(ss, std::forward<Params>(params)...); ss << NEWLINE; return ss.str(); }
    template <typename... Params> inline void     fprintTuple  (ostream_t& os, Params... params) { buildTuple(os, std::forward<Params>(params)...); }
    template <typename... Params> inline void     fprintTupleln(ostream_t& os, Params... params) { buildTuple(os, std::forward<Params>(params)...); os << NEWLINE; }

    // -- T

    template <typename... Params> inline void     printT   (Params... params) { buildT(standardout, std::forward<Params>(params)...); }
    template <typename... Params> inline void     printTln (Params... params) { printT(std::forward<Params>(params)...); standardout << NEWLINE; }
    template <typename... Params> inline void     errorT   (Params... params) { buildT(standarderr, std::forward<Params>(params)...); }
    template <typename... Params> inline void     errorTln (Params... params) { errorT(std::forward<Params>(params)...); standarderr << NEWLINE; }
    template <typename... Params> inline string_t sprintT  (Params... params) { ostringstream_t ss; buildT(ss, std::forward<Params>(params)...); return ss.str(); }
    template <typename... Params> inline string_t sprintTln(Params... params) { ostringstream_t ss; buildT(ss, std::forward<Params>(params)...); ss << NEWLINE; return ss.str(); }
    template <typename... Params> inline void     fprintT  (ostream_t& os, Params... params) { buildT(os, std::forward<Params>(params)...); }
    template <typename... Params> inline void     fprintTln(ostream_t& os, Params... params) { buildT(os, std::forward<Params>(params)...); os << NEWLINE; }

    // -- Format

    template <typename... Params> inline void     printFormat   (Segment const& format, Params... params) { buildFormat(standardout, format, std::forward<Params>(params)...); }
    template <typename... Params> inline void     printFormatln (Segment const& format, Params... params) { util::printFormat(format, std::forward<Params>(params)...); standardout << NEWLINE; }
    template <typename... Params> inline void     errorFormat   (Segment const& format, Params... params) { buildFormat(standarderr, format, std::forward<Params>(params)...); }
    template <typename... Params> inline void     errorFormatln (Segment const& format, Params... params) { util::errorFormat(format, std::forward<Params>(params)...); standarderr << NEWLINE; }
    template <typename... Params> inline string_t sprintFormat  (Segment const& format, Params... params) { ostringstream_t ss; buildFormat(ss, format, std::forward<Params>(params)...); return ss.str(); }
    template <typename... Params> inline string_t sprintFormatln(Segment const& format, Params... params) { ostringstream_t ss; buildFormat(ss, format, std::forward<Params>(params)...); ss << NEWLINE; return ss.str(); }
    template <typename... Params> inline void     fprintFormat  (ostream_t& os, Segment const& format, Params... params) { buildFormat(os, format, std::forward<Params>(params)...); }
    template <typename... Params> inline void     fprintFormatln(ostream_t& os, Segment const& format, Params... params) { buildFormat(os, format, std::forward<Params>(params)...); os << NEWLINE; }

    // -- List

    template <typename It> inline void     printList   (It begin, It end) { buildList(standardout, begin, end); }
    template <typename It> inline void     printListln (It begin, It end) { printList(begin, end); standardout << NEWLINE; }
    template <typename It> inline void     errorList   (It begin, It end) { buildList(standarderr, begin, end); }
    template <typename It> inline void     errorListln (It begin, It end) { errorList(begin, end); standarderr << NEWLINE; }
    template <typename It> inline string_t sprintList  (It begin, It end) { ostringstream_t ss; buildList(ss, begin, end); return ss.str(); }
    template <typename It> inline string_t sprintListln(It begin, It end) { ostringstream_t ss; buildList(ss, begin, end); ss << NEWLINE; return ss.str(); }
    template <typename It> inline void     fprintList  (ostream_t& os, It begin, It end) { buildList(os, begin, end); }
    template <typename It> inline void     fprintListln(ostream_t& os, It begin, It end) { buildList(os, begin, end); os << NEWLINE; }

    template <typename T, size_t N> inline void     printList   (std::array<T,N> const& array) { buildList(standardout, array); }
    template <typename T, size_t N> inline void     printListln (std::array<T,N> const& array) { printList(array); standardout << NEWLINE; }
    template <typename T, size_t N> inline void     errorList   (std::array<T,N> const& array) { buildList(standarderr, array); }
    template <typename T, size_t N> inline void     errorListln (std::array<T,N> const& array) { errorList(array); standarderr << NEWLINE; }
    template <typename T, size_t N> inline string_t sprintList  (std::array<T,N> const& array) { ostringstream_t ss; buildList(ss, array); return ss.str(); }
    template <typename T, size_t N> inline string_t sprintListln(std::array<T,N> const& array) { ostringstream_t ss; buildList(ss, array); ss << NEWLINE; return ss.str(); }
    template <typename T, size_t N> inline void     fprintList  (ostream_t& os, std::array<T,N> const& array) { buildList(os, array); }
    template <typename T, size_t N> inline void     fprintListln(ostream_t& os, std::array<T,N> const& array) { buildList(os, array); os << NEWLINE; }

    template <typename T, size_t N> inline void     printList   (T const(&array)[N]) { buildList(standardout, array); }
    template <typename T, size_t N> inline void     printListln (T const(&array)[N]) { printList(array); standardout << NEWLINE; }
    template <typename T, size_t N> inline void     errorList   (T const(&array)[N]) { buildList(standarderr, array); }
    template <typename T, size_t N> inline void     errorListln (T const(&array)[N]) { errorList(array); standarderr << NEWLINE; }
    template <typename T, size_t N> inline string_t sprintList  (T const(&array)[N]) { ostringstream_t ss; buildList(ss, array); return ss.str(); }
    template <typename T, size_t N> inline string_t sprintListln(T const(&array)[N]) { ostringstream_t ss; buildList(ss, array); ss << NEWLINE; return ss.str(); }
    template <typename T, size_t N> inline void     fprintList  (ostream_t& os, T const(&array)[N]) { buildList(os, array); }
    template <typename T, size_t N> inline void     fprintListln(ostream_t& os, T const(&array)[N]) { buildList(os, array); os << NEWLINE; }

    // -- std

    void     printStd   (String const fmt, ...);
    void     printStdln (String const fmt, ...);
    void     errorStd   (String const fmt, ...);
    void     errorStdln (String const fmt, ...);
    string_t sprintStd  (String const fmt, ...);
    string_t sprintStdln(String const fmt, ...);
    void     fprintStd  (ostream_t& os, String const fmt, ...);
    void     fprintStdln(ostream_t& os, String const fmt, ...);
}
