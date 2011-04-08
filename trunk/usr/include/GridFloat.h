// $Id$
// $URL$
// http://mew.cx/

#include <string>

class GridFloat
{
public:
    GridFloat();
    ~GridFloat();

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
    GridFloat( const GridFloat& );
    const GridFloat& operator=( const GridFloat& );
};

// vim: set sw=4 ts=8 et ic ai:
