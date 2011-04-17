// $Id$
// $URL$
// Copyright 2011 Mike Weiblen http://mew.cx/ -- All rights reserved.
// A container for the USGS "gridfloat" elevation data format, as provided by
// http://seamless.usgs.gov

#ifndef MEWCX_GRIDFLOAT_H
#define MEWCX_GRIDFLOAT_H

#include "mewcx/Coord.h"

namespace mewcx {

class Gridfloat;

class GridfloatFunctor
{
public:
    GridfloatFunctor() {}
    ~GridfloatFunctor() {}

    virtual void rowStart( unsigned int row ) {}
    virtual void operator()( const Gridfloat& gridfloat ) = 0;
    virtual void rowEnd( unsigned int row ) {}
};


class Gridfloat
{
public:
    Gridfloat( const mewcx::WGS84& southWest, const mewcx::WGS84& northEast );
    ~Gridfloat();

    void referenceEllipsoid( int re ) { _referenceEllipsoid = re; }
    bool readData( unsigned int rows, unsigned int cols, const char* fileName );

    bool valid() const { return( _data != 0 ); }
    bool operator!() const { return !valid(); }

    unsigned int numRows() const { return _numRows; }
    unsigned int numCols() const { return _numCols; }

    unsigned int apply( GridfloatFunctor& functor );
    // the following only valid during apply():
    unsigned int currentRow() const { return _currentRow; }
    unsigned int currentCol() const { return _currentCol; }
    const double currentElev() const;
    mewcx::UTM currentUtm() const;

    //const float* data() const { return _data; }

private:
    const mewcx::WGS84 _southWest;
    const mewcx::WGS84 _northEast;
    int _referenceEllipsoid;
    unsigned int _numRows;
    unsigned int _numCols;
    float* _data;
    double _latitudeStep;
    double _longitudeStep;

    // the following only valid during apply():
    unsigned int _currentRow;
    unsigned int _currentCol;

private:        // disallowed
    Gridfloat();
    Gridfloat( const Gridfloat& );
    const Gridfloat& operator=( const Gridfloat& );
};

}

#endif

// vim: set sw=4 ts=8 et ic ai:


