// ll2utm.h
// mew 2011-03-27
// $Id$

#ifndef LL2UTM_H
#define LL2UTM_H

class Ellipsoid
{
public:
    Ellipsoid( int id, const char* name, double radius, double ecc );

public:
    int _id;
    const char* _ellipsoidName;
    double _equatorialRadius; 
    double _eccentricitySquared;  

private:
    Ellipsoid() {}
};

extern Ellipsoid ellipsoids[];

/////////////////////////////////////////////////////////////////////////////

class Coord
{
};

class Wgs84Coord : public Coord
{
};

class UtmCoord : public Coord
{
};


void LLtoUTM( int ReferenceEllipsoid, const double Lat, const double Long, double &UTMNorthing, double &UTMEasting, char* UTMZone );

void UTMtoLL( int ReferenceEllipsoid, const double UTMNorthing, const double UTMEasting, const char* UTMZone, double& Lat,  double& Long );

char UTMLetterDesignator(double Lat);

#endif

// vim: set sw=4 ts=8 et ic ai:
