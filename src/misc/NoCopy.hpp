// Copyright 2013, James Mitchell, All rights reserved.

#pragma once

namespace util
{
    class NoCopy
    {
    protected:
        NoCopy(){}
        virtual ~NoCopy(){}
    private:
        NoCopy(const NoCopy&) = delete;
        NoCopy& operator = (const NoCopy&) = delete;
    };
}
