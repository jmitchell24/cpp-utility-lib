// Copyright 2013, James Mitchell, All rights reserved.

#define _CRT_SECURE_NO_WARNINGS
#include "Timer.hpp"
using namespace util;
using namespace std;

Duration::Duration()
    : time(DefaultDuration::zero())
{}

Duration::Duration(std::time_t const time)
    : time(Timer::Clock::from_time_t(time) - Timer::Clock::time_point())
{}

Duration::CountType Duration::nanoseconds () const { return chrono::duration_cast<Nanoseconds> (time).count(); }
Duration::CountType Duration::microseconds() const { return chrono::duration_cast<Microseconds>(time).count(); }
Duration::CountType Duration::milliseconds() const { return chrono::duration_cast<Milliseconds>(time).count(); }
Duration::CountType Duration::seconds     () const { return chrono::duration_cast<Seconds>     (time).count(); }
Duration::CountType Duration::minutes     () const { return chrono::duration_cast<Minutes>     (time).count(); }
Duration::CountType Duration::hours       () const { return chrono::duration_cast<Hours>       (time).count(); }
Duration::CountType Duration::days        () const { return chrono::duration_cast<Days>        (time).count(); }

Duration Duration::nanoseconds (CountParam c) { return (Duration)Nanoseconds (c); }
Duration Duration::microseconds(CountParam c) { return (Duration)Microseconds(c); }
Duration Duration::milliseconds(CountParam c) { return (Duration)Milliseconds(c); }
Duration Duration::seconds     (CountParam c) { return (Duration)Seconds     (c); }
Duration Duration::minutes     (CountParam c) { return (Duration)Minutes     (c); }
Duration Duration::hours       (CountParam c) { return (Duration)Hours       (c); }
Duration Duration::days        (CountParam c) { return (Duration)Days        (c); }

time_t Duration::getTime() const { return Timer::Clock::to_time_t(Timer::Clock::time_point(chrono::duration_cast<Timer::Clock::duration>(time))); }
Duration::DefaultDuration Duration::getDuration() const { return time; }

Duration Duration::operator+ (Duration const& d) const { return Duration(time + d.time); }
Duration Duration::operator- (Duration const& d) const { return Duration(time - d.time); }

Duration& Duration::operator+=(Duration const& d) { return *this = *this + d; }
Duration& Duration::operator-=(Duration const& d) { return *this = *this - d; }

bool Duration::operator== (Duration const& d) const { return time == d.time; }
bool Duration::operator!= (Duration const& d) const { return time != d.time; }
bool Duration::operator<  (Duration const& d) const { return time <  d.time; }
bool Duration::operator>  (Duration const& d) const { return time >  d.time; }
bool Duration::operator<= (Duration const& d) const { return time <= d.time; }
bool Duration::operator>= (Duration const& d) const { return time >= d.time; }

string_t Duration::str() const
{
    time_t const      tme = getTime();
    char const* const str = ctime(&tme);
    size_t const      len = strlen(str);
    return string_t(str,str+len-1);
}

Duration Duration::durationFromCount(CountType count)
{
    return Duration(DefaultDuration(count));
}

Timer::Timer()
: start_time(), last_duration(), is_recording(false)
{ }

Timer::Timer(AutoStart)
    : Timer()
{ start(); }

Timer::~Timer()
{}

void Timer::start()
{
    is_recording = true;
    start_time = Clock::now();
}

Duration Timer::stop()
{
    if (is_recording)
    {
        is_recording = false;
        last_duration = Duration(Clock::now() - start_time);
        return last_duration;
    }
    return Duration();
}

Duration Timer::reset()
{
    Duration const d = stop();
    start();
    return d;
}

Duration Timer::getElapsedTime() const
{
    return (is_recording) ? Duration(Clock::now() - start_time) : Duration();
}

Duration Timer::getLastTime() const
{
    return last_duration;
}

bool Timer::isRecording() const
{
    return is_recording;
}

Duration Timer::getTimeSinceEpoch()
{
    return Duration(Clock::now().time_since_epoch());
}

ostream_t& util::formatToString(ostream_t& os, Segment const& format, Duration const& duration)
{
    char_t       buffer[64];
    time_t const timeinfo = duration.getTime();
    if (format.null_terminated) os.write(buffer, s_ftime(buffer, sizeof(buffer)/sizeof(char), format.begin        , localtime(&timeinfo)));
    else                        os.write(buffer, s_ftime(buffer, sizeof(buffer)/sizeof(char), format.str().c_str(), localtime(&timeinfo)));
    return os;
}
