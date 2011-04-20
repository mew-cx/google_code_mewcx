// $Id$
// $URL$
// Copyright 2011 Mike Weiblen http://mew.cx/ -- All rights reserved.

#include "mewcx/UsgsGridfloat.h"
#include <fstream>
#include <cassert>


mewcx::Gridfloat::Gridfloat( const mewcx::WGS84& southWest, const mewcx::WGS84& northEast ) :
        _southWest(southWest), _northEast(northEast),
        _numRows(0), _numCols(0), _data(0),
        _latitudeStep(0), _longitudeStep(0),
        _currentRow(0), _currentCol(0)
{
}


mewcx::Gridfloat::~Gridfloat()
{
    _numCols = 0;
    _numRows = 0;

    delete [] _data;
    _data = 0;
}

bool mewcx::Gridfloat::readData( unsigned int cols, unsigned int rows, const char* fileName )
{
    std::ifstream infile;
    infile.open( fileName, std::ios::binary );
    if( ! infile )
        return false;

    infile.seekg( 0, std::ios::end );
    const size_t fileNumBytes( infile.tellg() );
    const size_t gridNumBytes( cols * rows * sizeof(float) );

    assert( fileNumBytes >= gridNumBytes );

    _numCols = cols;
    _numRows = rows;
    _data = new float[ _numCols * _numRows ];

    infile.seekg( 0, std::ios::beg );
    infile.read( reinterpret_cast<char*>(_data), fileNumBytes );
    infile.close();

    _latitudeStep  = (_northEast.latitude() - _southWest.latitude()) / (_numRows - 1);
    _longitudeStep = (_northEast.longitude() - _southWest.longitude()) / (_numCols - 1);
    return true;
}


void mewcx::Gridfloat::apply( GridfloatFunctor& functor )
{
    for( _currentRow = 0; _currentRow < _numRows; ++_currentRow )
    {
        _currentCol = 0;
        functor.rowStart( _currentRow );
        for( _currentCol = 0; _currentCol < _numCols; ++_currentCol )
        {
            functor( *this );
        }
        _currentCol = 0;
        functor.rowEnd( _currentRow );
    }
    _currentRow = 0;
}


unsigned int mewcx::Gridfloat::currentIndex() const
{
    //return (_currentRow * _numCols) + _currentCol;
    return (((_numRows-1)-_currentRow) * _numCols) + _currentCol;
}


double mewcx::Gridfloat::currentElev() const
{
    return _data[ currentIndex() ];
}


mewcx::UTM mewcx::Gridfloat::currentUtm() const
{
    const double lat( (_latitudeStep  * _currentRow) + _southWest.latitude() );
    const double lon( (_longitudeStep * _currentCol) + _southWest.longitude() );

    double easting(0);
    double northing(0);
    char zone[4];
    LLtoUTM( _referenceEllipsoid, lat, lon, northing, easting, zone );

    return UTM( easting, northing, 13, currentElev() );
}

// vim: set sw=4 ts=8 et ic ai:

