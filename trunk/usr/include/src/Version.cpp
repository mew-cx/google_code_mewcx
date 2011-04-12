// $Id$
// $URL$

#include "Version.h"
#include <string>
#include <sstream>


const char* osgfvGetVersion()
{
    return "0.0.0.0";
}

const char* osgfvGetLibraryName()
{
    return "foo";
}

const char* osgfvGetBuildDateTime()
{
    return "__DATE__ __TIME__";
}

/////////////////////////////////////////////////////////////////////////////
// Subversion's keyword substitution must be enabled on this file using the
// following command to automatically update the string values:
//      svn propset svn:keywords "Id Date Author URL Revision" Version.cpp


const char* osgfvSvnKeywordId()
{
    return "$Id$";
}

const char* osgfvSvnKeywordDate()
{
    return "$Date$";
}

const char* osgfvSvnKeywordAuthor()
{
    return "$Author$";
}

const char* osgfvSvnKeywordUrl()
{
    return "$URL$";
}

const char* osgfvSvnKeywordRevision()
{
    return "$Revision$";
}

unsigned int osgfvSvndRevision()
{
    unsigned int revision(0);
    std::string junk;
    std::istringstream ss( osgfvSvnKeywordRevision() );
    ss >> junk >> revision;
    return revision;
}

// vim: set sw=4 ts=8 et ic ai:
