// $Id$
// $URL$
// http://mew.cx/

#include <string>
#include <fstream>
#include <cassert>
#include <stdio.h>

// usr/include
#include "ll2utm.h"
#include "GridFloat.h"
#include "private/latlon_home.h"

// usr/data/seamless.usgs.gov/dem
#include "89875513/dem_info.h"


/////////////////////////////////////////////////////////////////////////////

class App
{
public:
    App( int argc, char** argv );
    ~App();

    bool loadFile( const std::string path, const std::string root, const std::string ext );
    void setOrigin( Latitude lat, Longitude lon, Elevation elev );
    void findMinMax();

    bool makeVerts() const;
    bool makeTexCoords() const;
    bool makeNormals() const;
    bool makeRawImage() const;

private:
    const DATA_TYPE* demData() const { return reinterpret_cast< const DATA_TYPE* >( _demGridFloat.data() ); }

private:
    int _argc;
    char** _argv;

    const int _refEllipsoid;
    std::string _fileRoot;
    GridFloat _demGridFloat;

    Latitude _originLat;
    Longitude _originLon;
    Elevation  _originElev;

    Easting  _originEasting;
    Northing  _originNorthing;
    char    _originZone[4];

    Elevation _elevMin;
    Elevation _elevMax;
    unsigned int _nodataCount;


private:
    App();
};


/////////////////////////////////////////////////////////////////////////////

App::App( int argc, char** argv ) :
    _argc(argc), _argv(argv),
    _refEllipsoid(23),  // WGS-84
    _originLat(0), _originLon(0), _originElev(0),
    _originEasting(0), _originNorthing(0),
    _elevMin(1e6), _elevMax(-1e6), _nodataCount(0)
{
    _originZone[0] = '\0';
}

App::~App()
{
}

bool App::loadFile( const std::string path, const std::string root, const std::string ext )
{
    _fileRoot = root;
    return _demGridFloat.read( path + root + ext );
}

void App::setOrigin( Latitude lat, Longitude lon, Elevation elev )
{
    _originLat = lat;
    _originLon = lon;
    _originElev = elev;
    LLtoUTM( _refEllipsoid, _originLat, _originLon, _originNorthing, _originEasting, _originZone );
}

void App::findMinMax()
{
    const DATA_TYPE* demPtr( demData() );
    for( int row = 0; row < NROWS; ++row )
    {
        const DATA_TYPE* rowPtr( &demPtr[row * NCOLS] );
        for( int col = 0; col < NCOLS; ++col )
        {
            const DATA_TYPE dem( *rowPtr );
            if( dem > NODATA_VALUE )
            {
                if( _elevMin > dem )   _elevMin = dem;
                if( _elevMax < dem )   _elevMax = dem;
            }
            else
            {
                ++_nodataCount;
            }
            ++demPtr;
        }
    }
}

bool App::makeVerts() const
{
    std::string fileName( _fileRoot + "_vertices.h" );
    std::ofstream outFile;
    outFile.open( fileName.c_str() );
    if( ! outFile )
        return false;

    outFile << "static const float vertices[][3] = {" << std::endl;
    outFile << "\t// easting, northing, elevation [m]" << std::endl;

    unsigned int vertId(0);
    const DATA_TYPE* demPtr( demData() );
    for( int row = 0; row < NROWS; ++row )
    {
        const Latitude lat( CELLSIZE * row + YLLCORNER );
        outFile << "\t// row " << row
            << " latitude " << lat
            << " vertId " << vertId
            << std::endl;

        for( int col = 0; col < NCOLS; ++col )
        {
            const Longitude lon( CELLSIZE * col + XLLCORNER );
            const DATA_TYPE dem( *demPtr );

            // UTM coordinates
            Easting easting(0);
            Northing northing(0);
            char zone[4];
            LLtoUTM( _refEllipsoid, lat, lon, northing, easting, zone );
            Elevation elevation(dem);

            // translate data to origin
            easting   -= _originEasting;
            northing  -= _originNorthing;
            elevation -= _originElev;

            outFile << "\t{" << easting << "," << northing << "," << elevation << "}," << std::endl;

            ++demPtr;
            ++vertId;
        }
    }
    outFile << "};\n" << std::endl;
    outFile.close();

    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LAT", _originLat, "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "ORIGIN_LON", _originLon, "degrees" );
    printf( "#define %s\t(%.2f)\t// [%s]\n\n", "ORIGIN_ELEV", _originElev, "meters" );
    printf( "#define %s\t\"%s\"\n",            "ORIGIN_UTMZONE", _originZone );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "ORIGIN_EASTING", _originEasting, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n\n", "ORIGIN_NORTHING", _originNorthing, "meters" );
    printf( "#define %s\t\"%s\"\n",            "DATA_FILE", _demGridFloat.fileName().c_str() );
    printf( "#define %s\t\t(%d)\n",            "NCOLS", NCOLS );
    printf( "#define %s\t\t(%d)\n\n",          "NROWS", NROWS );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "SOUTH_LAT", YLLCORNER, "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "WEST_LON", XLLCORNER, "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n",   "NORTH_LAT", ( CELLSIZE * (NROWS-1) + YLLCORNER ), "degrees" );
    printf( "#define %s\t(%.5f)\t// [%s]\n\n", "EAST_LON", ( CELLSIZE * (NCOLS-1) + XLLCORNER ), "degrees" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MINIMUM_ELEV", _elevMin, "meters" );
    printf( "#define %s\t(%.2f)\t// [%s]\n",   "MAXIMUM_ELEV", _elevMax, "meters" );
    printf( "#define %s\t(%d)\n\n",            "NODATA_COUNT", _nodataCount );

    return true;
}

bool App::makeTexCoords() const
{
    std::string fileName( _fileRoot + "_texcoords.h" );
    std::ofstream outFile;
    outFile.open( fileName.c_str() );
    if( ! outFile )
        return false;

    outFile << "static const float texcoords[][2] = {" << std::endl;
    for( int row = 0; row < NROWS; ++row )
    {
        const double yFract( (double)row / (NROWS-1) );

        for( int col = 0; col < NCOLS; ++col )
        {
            const double xFract( (double)col / (NCOLS-1) );
            outFile << "\t{" << xFract << "," << yFract << "}," << std::endl;
        }
    }
    outFile << "};\n" << std::endl;
    outFile.close();
    return true;
}

bool App::makeNormals() const
{
    // TODO
    return false;
}

bool App::makeRawImage() const
{
    const Elevation elevDelta( _elevMax - _elevMin );
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


/////////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
    App app( argc, argv );
    app.loadFile( FILE_PATH, FILE_ROOT, FILE_EXT );
    app.setOrigin( HOME_LAT, HOME_LON, HOME_ELEV );

    app.findMinMax();
    app.makeVerts();
    app.makeTexCoords();
    app.makeNormals();
    app.makeRawImage();
}


// vim: set sw=4 ts=8 et ic ai:
