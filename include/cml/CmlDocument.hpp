#pragma once

#include "CmlGroup.hpp"
#include "CmlReader.hpp"
#include <fstream>

namespace util
{
namespace cml
{
    class Document : public Group
    {
        string_t filename;
        bool writeback;

    public:
        Document(Segment const& filename, bool const writeback = false)
            : filename(filename.str()), writeback(writeback)
        {
            ifstream_t stream(str_cast<char>(this->filename));
            if (!stream.is_open())
                throw FileNotFoundException(filename);

            string_t const content = streamString(stream);
            Reader(content).read(*this);
        }

        ~Document()
        {
            if (writeback)
            {
                ofstream_t stream(str_cast<char>(this->filename));
                stream << *this;
            }
        }
    };
}
}
