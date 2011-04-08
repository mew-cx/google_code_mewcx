// ll2utm.h
// $Id$
// $URL$
// http://mew.cx/

#ifndef LL2UTM_H
#define LL2UTM_H

typedef double  Degrees;
typedef Degrees Latitude;
typedef Degrees Longitude;

typedef double  Meters;
typedef Meters  Easting;
typedef Meters  Northing;
typedef Meters  Elevation;


class Coord2D
{
public:
    Coord2D();
    virtual ~Coord2D();

private:        // disallowed
    const Coord2D& operator=( const Coord2D& );
    Coord2D( const Coord2D& );
};

class WGS84 : public Coord2D
{
};

class UTM : public Coord2D
{
};


class Coord3D
{
public:
    Coord3D();
    virtual ~Coord3D();

private:        // disallowed
    Coord3D( const Coord3D& );
    const Coord3D& operator=( const Coord3D& );
};

class WGS84Elev : public Coord3D
{
};

class UTMElev : public Coord3D
{
};


/////////////////////////////////////////////////////////////////////////////

class Ellipsoid
{
public:
    Ellipsoid( int id, const char* name, double radius, double ecc );

public:
    int _id;
    const char* _ellipsoidName;
    double _equatorialRadius; 
    double _eccentricitySquared;  

private:        // disallowed
    Ellipsoid();
    Ellipsoid( const Ellipsoid& );
    const Ellipsoid& operator=( const Ellipsoid& );
};

extern Ellipsoid ellipsoids[];


/////////////////////////////////////////////////////////////////////////////

void LLtoUTM( int ReferenceEllipsoid,
    const double Lat, const double Long,
    double &UTMNorthing, double &UTMEasting, char* UTMZone );

void UTMtoLL( int ReferenceEllipsoid,
    const double UTMNorthing, const double UTMEasting, const char* UTMZone,
    double& Lat,  double& Long );

char UTMLetterDesignator( double Lat );

#endif

// vim: set sw=4 ts=8 et ic ai:
