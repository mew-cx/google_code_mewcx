// $Id$
// $URL$
// http://mew.cx/

#ifndef NED13_89875513_H
#define NED13_89875513_H

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

#if 0
::::::::::::::
na1000_71946602        na1000  National Atlas 1000m
::::::::::::::
Number of columns:  35
Number of rows:  43
Resolution in x direction:  1000 Meter
Resolution in y direction:  1000 Meter
NODATA_value  -9999

Coordinate system ID WGS84:  4326
Top edge WGS84:  40.0153473865912 Degree
Bottom edge WGS84:  39.6081906288839 Degree
Left edge WGS84:  -105.783183592567 Degree
Right edge WGS84:  -105.342282705393 Degree



::::::::::::::
na200_02309874        na200   National Atlas 200m
::::::::::::::
Number of columns:  174
Number of rows:  219
Resolution in x direction:  200 Meter
Resolution in y direction:  200 Meter
NODATA_value  -9999

Coordinate system ID WGS84:  4326
Top edge WGS84:  40.019188983844 Degree
Bottom edge WGS84:  39.6049682273305 Degree
Left edge WGS84:  -105.783187301362 Degree
Right edge WGS84:  -105.344016388146 Degree



::::::::::::::
ned1_63870190    ned1    National Elevation Database 1 arcsecond
::::::::::::::
Number of columns:  1350
Number of rows:  1350
Resolution in x direction:  0.000277777777796473 Degree
Resolution in y direction:  0.000277777777796473 Degree
NODATA_value  -9999

Coordinate system ID WGS84:  4326
Top edge WGS84:  39.9999999994743 Degree
Bottom edge WGS84:  39.6249999994491 Degree
Left edge WGS84:  -105.749999998776 Degree
Right edge WGS84:  -105.37499999875 Degree



::::::::::::::
ned13_89875513        ned13   National Elevation Database 1/3 arcsecond
::::::::::::::
Number of columns:  4051
Number of rows:  4051
Resolution in x direction:  9.2592592601191E-05 Degree
Resolution in y direction:  9.25925926011928E-05 Degree
NODATA_value  -9999

Coordinate system ID WGS84:  4326
Top edge WGS84:  40.0000925918319 Degree
Bottom edge WGS84:  39.6249999992044 Degree
Left edge WGS84:  -105.750092591257 Degree
Right edge WGS84:  -105.374999998629 Degree




/home/weiblenm/data/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/dem
-rw-r--r-- 1 weiblenm weiblenm   152424 Dec 27 18:52 02309874/02309874.flt
-rw-r--r-- 1 weiblenm weiblenm  7290000 Apr 13 08:55 63870190/ned_63870190.flt
-rw-r--r-- 1 weiblenm weiblenm     6020 Dec 27 18:51 71946602/71946602.flt
-rw-r--r-- 1 weiblenm weiblenm 65642404 Dec 27 18:56 89875513/89875513.flt

/home/weiblenm/data/mewcx.googlecode.com/usr/data/gilpin_3x3/seamless.usgs.gov/drg/24k
-rw-r--r-- 1 weiblenm weiblenm 798514 Apr 11 15:03 1024x1024.png

#endif
// vim: set sw=4 ts=8 et ic ai:
