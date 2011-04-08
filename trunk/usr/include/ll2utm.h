// $Id$
// $URL$
// http://mew.cx/

#ifndef LL2UTM_H
#define LL2UTM_H


class Coord
{
public:
    Coord() {}
    virtual ~Coord() {}
};

class WGS84 : public Coord
{
public:
    WGS84() : _latitude(0), _longitude(0), _elevation(0) {}
    WGS84( double lat, double lon, double elev=0 ) : _latitude(lat), _longitude(lon), _elevation(elev) {}
    ~WGS84() {}

private:
    double _latitude;
    double _longitude;
    double _elevation;
};

class UTM : public Coord
{
public:
    UTM() : _easting(0), _northing(0), _zone(0), _elevation(0) {}
    UTM( double e, double n, int zone, double elev=0 ) : _easting(e), _northing(n), _zone(zone), _elevation(elev) {}
    ~UTM() {}

private:
    double _easting;
    double _northing;
    int _zone;
    double _elevation;
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
