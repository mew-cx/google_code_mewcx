// $Id$
// $URL$
// Copyright 2011 Mike Weiblen http://mew.cx/ -- All rights reserved.

#ifndef USGS_SEAMLESS_H
#define USGS_SEAMLESS_H

#include <string>

class UsgsSeamlessGridfloat
{
public:
    UsgsSeamlessGridfloat( const std::string& fileName );
    ~UsgsSeamlessGridfloat();

    bool valid() const { return( _data != 0 ); }
    bool operator!() const { return !valid(); }

    const std::string& fileName() const { return _fileName; }
    const char* data() const { return _data; }
    const size_t numBytes() const { return _numBytes; }

private:
    std::string _fileName;
    char* _data;
    size_t _numBytes;

private:        // disallowed
    UsgsSeamlessGridfloat();
    UsgsSeamlessGridfloat( const UsgsSeamlessGridfloat& );
    const UsgsSeamlessGridfloat& operator=( const UsgsSeamlessGridfloat& );
};

#endif

// vim: set sw=4 ts=8 et ic ai:
