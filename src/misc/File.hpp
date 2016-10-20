// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

#include "../string/Types.hpp"

namespace util
{
    struct TemporaryFile
    {
        std::string const filename;

        template <typename T>
        inline TemporaryFile(T const* const data, size_t const size)
        : filename(tmpnam(0))
        {
            FILE* const file = fopen(filename.c_str(),"wb");
            fwrite(data,sizeof(T),size,file);
            fclose(file);
        }

        inline ~TemporaryFile()
        {
            remove(filename.c_str());
        }
    };

    template <typename C> inline std::basic_string<C> basicGetFileContents(StringT<C> const& filepath)
    {
        ifstream_t stream(filepath.begin);
        if (!stream)
            throw FileNotFoundException(filepath);
        return basicStreamString(stream);
    }

    inline string_t getFileContents(String const& filepath)
    {
        return basicGetFileContents(filepath);
    }
}
