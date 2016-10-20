// Copyright 2013, James Mitchell, All rights reserved.

#include "Logger.hpp"
using namespace util;
using namespace util::log;

OutStreamLog::OutStreamLog(ostream_t& stream)
: stream(stream)
{}

void OutStreamLog::operator() (Entry const& entry) const
{
    fprintln(stream,STR_LITERAL("([#,7])(#)(#)(#) #"),
    toString(entry.type),
    entry.time,
    entry.file,
    entry.function,
    entry.message);
}

OutFileLog::OutFileLog(Segment const& filename)
    : OutStreamLog(file), file(str_cast<char>(filename.str()))
{}

Logger::Logger(Manager&      manager,
               char_t const* function,
               char_t const* file,
               unsigned      line) :
manager         (manager),
current_function(function),
current_file    (file),
current_line    (line)
{}

string_t const& Logger::getCurrentFunction() const { return current_function; }
string_t const& Logger::getCurrentFile    () const { return current_file; }
unsigned const& Logger::getCurrentLine    () const { return current_line; }

Entry Logger::createEntry(Entry::Type const type, Segment const& message)
{
    Entry e;
    e.type     = type;
    e.time     = Timer::getTimeSinceEpoch();
    e.file     = current_file;
    e.function = current_function;
    e.message  = message.str();
    return e;
}

Manager& Manager::self()
{
    static Manager _self;
    return _self;
}

Logger Manager::createLogger(char_t const* function,
                             char_t const* file,
                             unsigned      line)
{
    return Logger(self(), function, file, line);
}

Manager::Manager()
{
    mutex.lock();

    this->onlog.onAdd.add([this](OnLogEvent::Delegate const& d)
    { for (auto e : entries) d(e); });

    this->onlog.add(OutStreamLog(standardout));

    mutex.unlock();
}

void Logger::pause()
{
    manager.addEntry(*this,Entry::Type::Message,STR_LITERAL("press enter to continue...\n"));
    std::cin.get();
}

void Manager::addEntry(Logger const& logger, Entry::Type const& type, Segment const& msg)
{
	Entry e;
	e.type     = type;
    e.time     = Timer::getTimeSinceEpoch();
    e.file     = logger.getCurrentFile();
    e.function = logger.getCurrentFunction();
    e.message  = msg.str();

    addEntry(e);
}

void Manager::addEntry(Entry const& entry)
{
    mutex.lock();

    if (entries.size() == MAX_ENTRY_BUFFER_SIZE)
        entries.pop_front();
    entries.push_back(entry);

    onlog(entry);

    mutex.unlock();
}
