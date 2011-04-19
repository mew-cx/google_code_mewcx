// $Id$
// $URL$
// http://mew.cx/

#include "mewcx/Coord.h"
#include "mewcx/UsgsGridfloat.h"
#include "mewcx/private/latlon_home.h"

#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////

#define MEWCX_ROOT      "../../../"
#define NODATA_value    (-9999)

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
    VertexFunctor( const mewcx::UTM& origin ) : _origin(origin), _vertId(0)
    {
        printf( "static const float vertices[][3] = {\n" );
        printf( "\t// easting, northing, elevation [m]\n" );
    }

    void rowStart( unsigned int row )
    {
        printf( "\t// row %d vertId %d\n", row, _vertId );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const mewcx::UTM& utm( gridfloat.currentUtm() );
        printf( "\t{%.2f,%.2f,%.2f},\n",
            utm.easting() - _origin.easting(),
            utm.northing() - _origin.northing(),
            utm.elevation() - _origin.elevation() );
        ++_vertId;
    }

    void rowEnd( unsigned int row )
    {
        printf( "\n" );
    }

    ~VertexFunctor()
    {
        printf( "};\t// vertices[][3]\n" );
        printf( "\n" );
    }

private:
    mewcx::UTM _origin;
    unsigned int _vertId;
};


class HeightFieldFunctor : public mewcx::GridfloatFunctor
{
public:
    HeightFieldFunctor( const mewcx::UTM& origin, unsigned int numRows, unsigned int numCols ) :
        _origin(origin)
    {
        printf( "HeightField {\n" );
        printf( "  DataVariance DYNAMIC\n" );
        //printf( "  Origin %f %f %f\n", -_origin.easting(), -_origin.northing(), -_origin.elevation() );
        printf( "  Origin %f %f %f\n", 0.0, 0.0, 0.0 );
        printf( "  XInterval 80.0\n" );
        printf( "  YInterval 80.0\n" );
        printf( "  SkirtHeight 0\n" );
        printf( "  BorderWidth 0\n" );
        printf( "  NumColumnsAndRows %d %d\n", numRows, numCols );
        printf( "  Heights\n" );
        printf( "  {\n" );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const double elevationScale( 5.0 );
        const double elev( gridfloat.currentElev() );
        printf( "%.2f ", ( elev - _origin.elevation() ) * elevationScale );
    }

    void rowEnd( unsigned int row )
    {
        printf( "\n" );
    }

    ~HeightFieldFunctor()
    {
        printf( "  }\n" );
        printf( "}\n" );
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

    void rowStart( unsigned int row )
    {
        printf( "\t// row %d\n", row );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const double yFract( (double)gridfloat.currentRow() / (gridfloat.numRows()-1) );
        const double xFract( (double)gridfloat.currentCol() / (gridfloat.numCols()-1) );
        printf( "\t{%.5f,%.5f},\n", xFract, yFract );
    }

    void rowEnd( unsigned int row )
    {
        printf( "\n" );
    }

    ~TexcoordFunctor()
    {
        printf( "};\t// texcoords[][2]\n" );
        printf( "\n" );
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

    void rowStart( unsigned int row )
    {
        printf( "\t/*%d*/\t\n", row );
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
        const unsigned int a( gridfloat.currentIndex() );
        const unsigned int b( a + gridfloat.numCols() );
        printf( "%d,%d,", b, a );
    }

    void rowEnd( unsigned int row )
    {
        printf( "\n" );
    }

    ~IndexFunctor()
    {
        printf( "};\t// indices[]\n" );
        printf( "\n" );
    }
};


/////////////////////////////////////////////////////////////////////////////

class RawImageFunctor : public mewcx::GridfloatFunctor
{
public:
    RawImageFunctor()
    {
    }

    void rowStart( unsigned int row )
    {
    }

    void operator()( const mewcx::Gridfloat& gridfloat )
    {
    }

    void rowEnd( unsigned int row )
    {
    }

    ~RawImageFunctor()
    {
    }
};

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

class App
{
public:
    App( int argc, char** argv );
    ~App();

    int  main();

private:
    int _argc;
    char** _argv;

private:
    App();
};


/////////////////////////////////////////////////////////////////////////////

App::App( int argc, char** argv ) : _argc(argc), _argv(argv)
{
}

App::~App()
{
}

int App::main()
{
    const int refEllipsoid(23);        // WGS-84
    double  homeEasting;
    double  homeNorthing;
    char    homeZone[4];

    mewcx::LLtoUTM( refEllipsoid,
        HOME_LATITUDE, HOME_LONGITUDE,
        homeNorthing, homeEasting, homeZone );

    mewcx::UTM origin( homeEasting, homeNorthing, HOME_UTM_ZONE, HOME_ELEVATION );

#if 1
    const char* sourceName( "National Elevation Database 1 arcsecond" );
    const char* fileName( MEWCX_ROOT "usr/data/gilpin_3x3/seamless.usgs.gov/dem/63870190/ned_63870190.flt" );
    const mewcx::WGS84 southWest( 39.6249999994491, -105.749999998776 );
    const mewcx::WGS84 northEast( 39.9999999994743, -105.37499999875 );
    const unsigned int numRows(1350);
    const unsigned int numCols(1350);
    //NODATA_value(-9999);
    // Resolution in x direction:  0.000277777777796473 Degree
    // Resolution in y direction:  0.000277777777796473 Degree
#elif 0
    const char* sourceName( "National Elevation Database 1/3 arcsecond" );
    const char* fileName( MEWCX_ROOT "big/data/gilpin_3x3/seamless.usgs.gov/dem/89875513/89875513.flt" );
    const mewcx::WGS84 southWest( 39.6249999992044, -105.750092591257 );
    const mewcx::WGS84 northEast( 40.0000925918319, -105.374999998629 );
    const unsigned int numRows(4051);
    const unsigned int numCols(4051);
    //NODATA_value(-9999);
    // Resolution in x direction:  9.2592592601191E-05 Degree
    // Resolution in y direction:  9.25925926011928E-05 Degree
#else
    const char* sourceName( "testpattern" );
    const char* fileName( "testpattern.flt" );
    const mewcx::WGS84 southWest( 39.625, -105.75 );
    const mewcx::WGS84 northEast( 40.000, -105.375 );
    const unsigned int numRows(10);
    const unsigned int numCols(10);
#endif

    printf( "sourceName: \"%s\"\n", sourceName );
    printf( "\n" );

    mewcx::Gridfloat grid( southWest, northEast );
    grid.referenceEllipsoid( refEllipsoid );
    grid.readData( numRows, numCols, fileName );
    if( ! grid )
    {
        printf( "cant load datafile \"%s\"\n", fileName );
        return(1);
    }

    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LAT", HOME_LATITUDE, "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LON", HOME_LONGITUDE, "degrees" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "ORIGIN_ELEV", HOME_ELEVATION, "meters" );
    printf( "\n" );
    printf( "#define %s\t\"%s\"\n",            "ORIGIN_UTMZONE", homeZone );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "ORIGIN_EASTING", homeEasting, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "ORIGIN_NORTHING", homeNorthing, "meters" );
    printf( "\n" );
    //printf( "#define %s\t\"%s\"\n",            "DATA_FILE", _demGridFloat.fileName().c_str() );
    printf( "#define %s\t\t(%d)\n",            "NCOLS", numCols );
    printf( "#define %s\t\t(%d)\n",            "NROWS", numRows );
    printf( "#define %s\t\t(%d)\n",            "NUM_VERTS", numRows * numCols );
    printf( "\n" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "SOUTH_LAT", southWest.latitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "WEST_LON", southWest.longitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "NORTH_LAT", northEast.latitude(), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "EAST_LON", northEast.longitude(), "degrees" );
    printf( "\n" );

    {
    HeightFieldFunctor makeHF( origin, numRows, numCols );
    grid.apply( makeHF );
    }

#if 0
    {
    MinMaxFunctor minmax;
    grid.apply( minmax );

    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MINIMUM_ELEV", minmax._elevMin, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MAXIMUM_ELEV", minmax._elevMax, "meters" );
    printf( "#define %s\t(%d)\n",              "NODATA_COUNT", minmax._nodataCount );
    printf( "\n" );
    }

    {
    VertexFunctor makeVerts( origin );
    grid.apply( makeVerts );
    }

    {
    TexcoordFunctor makeTexcoords;
    grid.apply( makeTexcoords );
    }

    {
    IndexFunctor makeIndices;
    grid.apply( makeIndices );
    }
#endif
}


/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    App app( argc, argv );
    int returnCode = app.main();
    return returnCode;
}

// vim: set sw=4 ts=8 et ic ai:
