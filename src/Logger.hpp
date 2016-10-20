// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "Types.hpp"
#include "Exception.hpp"
#include "func/Event.hpp"
#include "string/Print.hpp"
#include "typetraits/CallTraits.hpp"
#include "misc/NoCopy.hpp"
#include "misc/Timer.hpp"

#include <deque>
#include <mutex>

#define logm       util::log::Manager::self()                                                       // NOT thread-safe
#define logr       util::log::Manager::createLogger(STR_CURRENT_FUNCTION,STR_CURRENT_FILE,__LINE__) // thread-safe
#define logrl(loc) util::log::Manager::createLogger(loc                 ,STR_CURRENT_FILE,__LINE__) // thread-safe

#define MAX_ENTRY_BUFFER_SIZE 100

namespace util
{
namespace log
{
    struct Entry;
    struct OutStreamLog;
    struct OutFileLog;

    class Manager;
    class Logger;

    struct Entry
    {
        enum Type
        {
            Message=0, Warning, Error, Abort, Stamp
        }type;

        Duration time;
        string_t file;
        string_t function;
        string_t message;
    };

    static inline ostream_t& operator<< (ostream_t& os, Entry::Type const& t)
    {
        switch (t)
        {
            case log::Entry::Type::Error   : return os << STR_LITERAL("error");
            case log::Entry::Type::Abort   : return os << STR_LITERAL("abort");
            case log::Entry::Type::Warning : return os << STR_LITERAL("warning");
            case log::Entry::Type::Message : return os << STR_LITERAL("message");
            case log::Entry::Type::Stamp   : return os << STR_LITERAL("stamp");
        }
        return os << STR_LITERAL("unknown");
    }

    struct OutStreamLog
    {
        ostream_t& stream;
        OutStreamLog(ostream_t& stream);
        void operator() (Entry const& entry) const;
    };

    struct OutFileLog : public OutStreamLog
    {
        OutFileLog(Segment const& filename);
    private:
        ofstream_t file;
    };

    class Manager : NoCopy
    {
    public:
        typedef Event<void(Entry const&)> OnLogEvent;
        OnLogEvent onlog;

        void addEntry(Logger const& logger, Entry::Type const& type, Segment const& message);
        void addEntry(Entry const& entry);

        static Manager& self();
        static Logger createLogger(char_t const* function,
                                   char_t const* file,
                                   unsigned      line);

    private:
        std::deque<Entry> entries;
        std::mutex mutex;

        Manager();
    };

    class Logger
    {
        Manager& manager;

        string_t current_function;
        string_t current_file;
        unsigned current_line;

        Entry createEntry(Entry::Type const type, Segment const& message);

    public:
        Logger(Manager&            manager,
               char_t const* const function,
               char_t const* const file,
               unsigned      const line);

        Logger& operator= (Logger const&)=delete;

        string_t const& getCurrentFunction() const;
        string_t const& getCurrentFile    () const;
        unsigned const& getCurrentLine    () const;

        template <typename... Params>
        inline Entry entry(Entry::Type const type, Segment const& msg, Params&&... params)
        { return createEntry(type, sprint(msg,std::forward<Params>(params)...)); }

        template <typename... Params> inline void abort  (Segment const& msg, Params&&... params) { manager.addEntry(createEntry(Entry::Type::Abort  ,sprint(msg,std::forward<Params>(params)...))); ::abort(); }
        template <typename... Params> inline void error  (Segment const& msg, Params&&... params) { manager.addEntry(createEntry(Entry::Type::Error  ,sprint(msg,std::forward<Params>(params)...))); }
        template <typename... Params> inline void warning(Segment const& msg, Params&&... params) { manager.addEntry(createEntry(Entry::Type::Warning,sprint(msg,std::forward<Params>(params)...))); }
        template <typename... Params> inline void message(Segment const& msg, Params&&... params) { manager.addEntry(createEntry(Entry::Type::Message,sprint(msg,std::forward<Params>(params)...))); }
        template <typename... Params> inline void stamp() {  }
        void pause();
    };
}
}
