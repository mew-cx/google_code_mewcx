// $Id$
// $URL$
// http://mew.cx/

#include "GridFloat.h"
#include <fstream>


GridFloat::GridFloat() :
        _data(0), _numBytes(0)
{
}


GridFloat::~GridFloat()
{
    delete [] _data;
    _data = 0;
    _numBytes = 0;
}


bool GridFloat::read( const std::string& fileName )
{
    _fileName = fileName;
    std::ifstream infile;
    infile.open( _fileName.c_str(), std::ios::binary );
    if( ! infile )
        return false;

    infile.seekg( 0, std::ios::end );
    _numBytes = infile.tellg();
    _data = new char[ _numBytes ];
    infile.seekg( 0, std::ios::beg );
    infile.read( _data, _numBytes );
    infile.close();
    return true;
}


// vim: set sw=4 ts=8 et ic ai:
