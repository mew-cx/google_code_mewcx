// $Id$
// $URL$
// http://mew.cx/

#ifndef DEM_INFO_H_89875513
#define DEM_INFO_H_89875513

// USGS "1/3 arcsecond" National Elevation Database (NED) Digital
// Elevation Model (DEM)

#define DEMFILE_ROOT "89875513"
#define DEMFILE_EXT  ".flt"        // == USGS gridfloat (not OpenFlight)
typedef float DEMDATA_TYPE;

// converted from "89875513.hdr"
#define NCOLS         (4051)
#define NROWS         (4051)
#define XLLCORNER     (-105.75009259126)        // [degrees longitude]
#define YLLCORNER     (39.624999999204)         // [degrees latitude]
#define CELLSIZE      (9.2592592601192e-005)    // [degrees]
#define NODATA_value  (-9999)
#define BYTEORDER     "LSBFIRST"

#endif

// vim: set sw=4 ts=8 et ic ai:
