// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../String.hpp"
#include "../typetraits/CallTraits.hpp"

#include <chrono>
#include <string>

/*
%a		Abbreviated weekday name *													Thu
%A		Full weekday name *															Thursday
%b		Abbreviated month name *													Aug
%B		Full month name *															August
%c		Date and time representation *												Thu Aug 23 14:55:02 2001
%C		Year divided by 100 and truncated to integer (00-99)						20
%d		Day of the month, zero-padded (01-31)										23
%D		Short MM/DD/YY date, equivalent to %m/%d/%y									08/23/01
%e		Day of the month, space-padded ( 1-31)										23
%F		Short YYYY-MM-DD date, equivalent to %Y-%m-%d								2001-08-23
%g		Week-based year, last two digits (00-99)									01
%G		Week-based year																2001
%h		Abbreviated month name * (same as %b)										Aug
%H		Hour in 24h format (00-23)													14
%I		Hour in 12h format (01-12)													02
%j		Day of the year (001-366)													235
%m		Month as a decimal number (01-12)											08
%M		Minute (00-59)																55
%n		New-line character ('\n')
%p		AM or PM designation														PM
%r		12-hour clock time *														02:55:02 pm
%R		24-hour HH:MM time, equivalent to %H:%M										14:55
%S		Second (00-61)																02
%t		Horizontal-tab character ('\t')
%T		ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S						14:55:02
%u		ISO 8601 weekday as number with Monday as 1 (1-7)							4
%U		Week number with the first Sunday as the first day of week one (00-53)		33
%V		ISO 8601 week number (00-53)												34
%w		Weekday as a decimal number with Sunday as 0 (0-6)							4
%W		Week number with the first Monday as the first day of week one (00-53)		34
%x		Date representation *														08/23/01
%X		Time representation *														14:55:02
%y		Year, last two digits (00-99)												01
%Y		Year																		2001
%z		ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100)
        If timezone cannot be termined, no characters								+100
%Z		Timezone name or abbreviation *
        If timezone cannot be termined, no characters								CDT
%%		A % sign																	%
*/

namespace util
{
    class Duration
    {
    public:
        typedef double                CountType;
        typedef best_param<CountType> CountParam;

        typedef std::chrono::duration<CountType,std::nano>  Nanoseconds;
        typedef std::chrono::duration<CountType,std::micro> Microseconds;
        typedef std::chrono::duration<CountType,std::milli> Milliseconds;
        typedef std::chrono::duration<CountType>                      Seconds;
        typedef std::chrono::duration<CountType,std::ratio<60>>       Minutes;
        typedef std::chrono::duration<CountType,std::ratio<60*60>>    Hours;
        typedef std::chrono::duration<CountType,std::ratio<60*60*24>> Days;

        typedef Seconds DefaultDuration;

        Duration();
        Duration(Duration const&)=default;
        explicit Duration(std::time_t const time);

        template <class Rep, class Period>
        inline explicit Duration(std::chrono::duration<Rep,Period> const& time)
            : time(std::chrono::duration_cast<DefaultDuration>(time))
        {}

        CountType nanoseconds () const;
        CountType microseconds() const;
        CountType milliseconds() const;
        CountType seconds     () const;
        CountType minutes     () const;
        CountType hours       () const;
        CountType days        () const;

        static Duration nanoseconds (CountParam c);
        static Duration microseconds(CountParam c);
        static Duration milliseconds(CountParam c);
        static Duration seconds     (CountParam c);
        static Duration minutes     (CountParam c);
        static Duration hours       (CountParam c);
        static Duration days        (CountParam c);

        time_t          getTime    () const; // interpret duration as time since epoch
        DefaultDuration getDuration() const;

        Duration operator+ (Duration const& d) const;
        Duration operator- (Duration const& d) const;

        Duration& operator =(Duration const&)=default;
        Duration& operator+=(Duration const& d);
        Duration& operator-=(Duration const& d);

        bool operator== (Duration const& d) const;
        bool operator!= (Duration const& d) const;
        bool operator<  (Duration const& d) const;
        bool operator>  (Duration const& d) const;
        bool operator<= (Duration const& d) const;
        bool operator>= (Duration const& d) const;

        string_t str() const;

        DEFINE_OBJECT_OSTREAM_OPERATOR(Duration)

    private:
        static Duration durationFromCount(CountType count);

        DefaultDuration time;
    };

    struct AutoStart { constexpr AutoStart(){} } constexpr autostart;

    class Timer
    {
    public:
        typedef std::chrono::high_resolution_clock Clock;

        Timer();
        Timer(AutoStart);
        virtual ~Timer();

        void start();
        Duration stop();
        Duration reset();
        Duration getLastTime   () const;
        Duration getElapsedTime() const;
        bool     isRecording   () const;

        static Duration getTimeSinceEpoch();

    protected:
        Clock::time_point start_time;
        Duration last_duration;
        bool is_recording;
    };

    ostream_t& formatToString(ostream_t& os, Segment const& format, Duration const& duration);
}
