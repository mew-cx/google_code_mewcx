// $Id$
// $URL$
// http://mew.cx/

#include "mewcx/Coord.h"
#include "mewcx/UsgsGridfloat.h"
#include "mewcx/private/latlon_home.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////

#if 0
const char* sourceName( "National Atlas 1000m" );
const char* fileName( "/home/mew/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/dem/71946602/71946602.flt" );
const mewcx::WGS84 southWest( 39.6081906288839, -105.783183592567 );
const mewcx::WGS84 northEast( 40.0153473865912, -105.342282705393 );
const unsigned int numRows(43);
const unsigned int numCols(35);
//Resolution in x direction:  1000 Meter
//Resolution in y direction:  1000 Meter
#define NODATA_value  (-9999)
#endif

#if 0
const char* sourceName( "National Atlas 200m" );
const char* fileName( "/home/mew/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/dem/02309874/02309874.flt" );
const mewcx::WGS84 southWest( 39.6049682273305, -105.783187301362 );
const mewcx::WGS84 northEast( 40.019188983844, -105.344016388146 );
const unsigned int numRows(219);
const unsigned int numCols(174);
//Resolution in x direction:  200 Meter
//Resolution in y direction:  200 Meter
#define NODATA_value  (-9999)
#endif

#if 1
const char* sourceName( "National Elevation Database 1 arcsecond" );
const char* fileName( "/home/mew/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/dem/63870190/ned_63870190.flt" );
const mewcx::WGS84 southWest( 39.6249999994491, -105.749999998776 );
const mewcx::WGS84 northEast( 39.9999999994743, -105.37499999875 );
const unsigned int numRows(1350);
const unsigned int numCols(1350);
// Resolution in x direction:  0.000277777777796473 Degree
// Resolution in y direction:  0.000277777777796473 Degree
#define NODATA_value  (-9999)
#endif

#if 0
const char* sourceName( "National Elevation Database 1/3 arcsecond" );
const char* fileName( "/home/mew/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/dem/89875513/89875513.flt" );
const mewcx::WGS84 southWest( 39.6249999992044, -105.750092591257 );
const mewcx::WGS84 northEast( 40.0000925918319, -105.374999998629 );
const unsigned int numRows(4051);
const unsigned int numCols(4051);
// Resolution in x direction:  9.2592592601191E-05 Degree
// Resolution in y direction:  9.25925926011928E-05 Degree
#define NODATA_value  (-9999)
#endif

/////////////////////////////////////////////////////////////////////////////

class MinMaxFunctor : public mewcx::GridfloatFunctor
{
public:
    MinMaxFunctor() : _elevMin(1e6), _elevMax(-1e6), _nodataCount(0) {}

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const mewcx::UTM& utm( gridfloat.currentUtm() );
        const float elev( utm.elevation() );
        if( elev > NODATA_value )
        {
            if( _elevMin > elev )   _elevMin = elev;
            if( _elevMax < elev )   _elevMax = elev;
        }
        else
        {
            ++_nodataCount;
        }
    }

public:
    double _elevMin;
    double _elevMax;
    unsigned int _nodataCount;
};

/////////////////////////////////////////////////////////////////////////////

class VertexFunctor : public mewcx::GridfloatFunctor
{
public:
    VertexFunctor( const mewcx::UTM& origin ) : _origin(origin)
    {
        printf( "static const float vertices[][3] = {\n" );
        printf( "\t// easting, northing, elevation [m]\n" );
    }

    ~VertexFunctor()
    {
        printf( "};\n\n" );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const mewcx::UTM& utm( gridfloat.currentUtm() );
        printf( "\t{%.2f,%.2f,%.2f},\n",
            utm.easting() - _origin.easting(),
            utm.northing() - _origin.northing(),
            utm.elevation() - _origin.elevation() );
    }
private:
    mewcx::UTM _origin;
};

/////////////////////////////////////////////////////////////////////////////

class TexcoordFunctor : public mewcx::GridfloatFunctor
{
public:
    TexcoordFunctor()
    {
        printf( "static const float texcoords[][2] = {\n" );
    }

    ~TexcoordFunctor()
    {
        printf( "};\n\n" );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const double yFract( (double)gridfloat.currentRow() / (gridfloat.numRows()-1) );
        const double xFract( (double)gridfloat.currentCol() / (gridfloat.numCols()-1) );
        printf( "\t{%.4f,%.4f},\n", xFract, yFract );
    }
};

/////////////////////////////////////////////////////////////////////////////

class IndexFunctor : public mewcx::GridfloatFunctor
{
public:
    IndexFunctor()
    {
        printf( "static const unsigned int indices[] = {\n" );
    }

    ~IndexFunctor()
    {
        printf( "};\n\n" );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const double yFract( (double)gridfloat.currentRow() / (gridfloat.numRows()-1) );
        const double xFract( (double)gridfloat.currentCol() / (gridfloat.numCols()-1) );
        printf( "\t{%.4f,%.4f},\n", xFract, yFract );
    }
};

/////////////////////////////////////////////////////////////////////////////

class App
{
public:
    App( int argc, char** argv );
    ~App();

    void main();
    int result() const { return _result; }

private:
    int _argc;
    char** _argv;
    int _result;

private:
    App();
};


/////////////////////////////////////////////////////////////////////////////

App::App( int argc, char** argv ) : _argc(argc), _argv(argv), _result(0)
{
}

App::~App()
{
}

void App::main()
{
    const int refEllipsoid(23);        // WGS-84
    double  homeEasting;
    double  homeNorthing;
    char    homeZone[4];

    mewcx::LLtoUTM( refEllipsoid,
        HOME_LATITUDE, HOME_LONGITUDE,
        homeNorthing, homeEasting, homeZone );

    mewcx::UTM origin( homeEasting, homeNorthing, HOME_UTM_ZONE, HOME_ELEVATION );

    mewcx::Gridfloat grid( southWest, northEast );
    grid.readData( numRows, numCols, fileName );
    grid.referenceEllipsoid( refEllipsoid );

    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LAT", HOME_LATITUDE, "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LON", HOME_LONGITUDE, "degrees" );
    printf( "#define %s\t(%.2f)\t// [%s]\n\n", "ORIGIN_ELEV", HOME_ELEVATION, "meters" );
    printf( "#define %s\t\"%s\"\n",            "ORIGIN_UTMZONE", homeZone );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "ORIGIN_EASTING", homeEasting, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n\n", "ORIGIN_NORTHING", homeNorthing, "meters" );
    //printf( "#define %s\t\"%s\"\n",            "DATA_FILE", _demGridFloat.fileName().c_str() );
    printf( "#define %s\t\t(%d)\n",            "NCOLS", numCols );
    printf( "#define %s\t\t(%d)\n\n",          "NROWS", numRows );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "SOUTH_LAT", southWest.latitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "WEST_LON", southWest.longitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "NORTH_LAT", northEast.latitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n\n", "EAST_LON", northEast.longitude(), "degrees" );

    MinMaxFunctor minmax;
    //grid.apply( minmax );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MINIMUM_ELEV", minmax._elevMin, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MAXIMUM_ELEV", minmax._elevMax, "meters" );
    printf( "#define %s\t(%d)\n\n",            "NODATA_COUNT", minmax._nodataCount );

    VertexFunctor makeVerts( origin );
    //grid.apply( makeVerts );

    TexcoordFunctor makeTexcoords;
    //grid.apply( makeTexcoords );

    IndexFunctor makeIndices;
    grid.apply( makeIndices );
}

/////////////////////////////////////////////////////////////////////////////

#if 0
bool App::makeRawImage() const
{
    const double elevDelta( _elevMax - _elevMin );
    assert( elevDelta >= 0 );

    typedef unsigned char Pixel;
    const unsigned int NUMPIXELS( NCOLS*NROWS );
    const unsigned int SIZEOF_PIXELBUFFER( NUMPIXELS * sizeof(Pixel) );
    const Pixel* pixelBuffer( new Pixel[ NUMPIXELS ] );

    const DATA_TYPE* demPtr( demData() );
    Pixel* pixelPtr( const_cast< Pixel* >(pixelBuffer) );
    for( int row = 0; row < NROWS; ++row )
    {
        for( int col = 0; col < NCOLS; ++col )
        {
            const DATA_TYPE dem( *demPtr );

            // generate raw pixel intensity based on elevation range
            const double zFract( (dem - _elevMin) / elevDelta );
            Pixel pixel(0);
            if( dem > NODATA_VALUE )
            {
                pixel = (Pixel)( 0x00ff * zFract );
            }
            *(pixelPtr++) = pixel;
            ++demPtr;
        }
    }

    std::string fileName( _fileRoot + "_image.raw" );
    std::ofstream rawImage;
    rawImage.open( fileName.c_str(), std::ios::binary );
    if( rawImage )
    {
        rawImage.write( (const char*) pixelBuffer, SIZEOF_PIXELBUFFER );
    }
    rawImage.close();
    return true;
}
#endif


/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    App app( argc, argv );
    app.main();
    return app.result();
}

// vim: set sw=4 ts=8 et ic ai:
