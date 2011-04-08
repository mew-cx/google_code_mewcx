// FileContents.h
// $Id$
// $URL$
// http://mew.cx/

#include <types.h>
#include <string>

class FileContents
{
public:
    FileContents();
    ~FileContents();

    bool read( const std::string& fileName );

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
    FileContents( const FileContents& );
    const FileContents& operator=( const FileContents& );
};

// vim: set sw=4 ts=8 et ic ai:
