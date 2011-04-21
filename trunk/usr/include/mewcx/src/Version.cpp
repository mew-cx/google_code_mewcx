// $Id$
// $URL$
// http://mew.cx/

#include "mewcx/Version.h"

#include <string>
#include <sstream>


const char* mewcxGetVersion()
{
    return "0.0.1.109";
}

const char* mewcxGetLibraryName()
{
    return "mumble";
}

const char* mewcxGetBuildDateTime()
{
    return "__DATE__ __TIME__";
}

/////////////////////////////////////////////////////////////////////////////
// Subversion's keyword substitution must be enabled on this file using the
// following command to automatically update the string values:
//      svn propset svn:keywords "Id Date Author URL Revision" Version.cpp


const char* mewcxSvnKeywordId()
{
    return "$Id$";
}

const char* mewcxSvnKeywordDate()
{
    return "$Date$";
}

const char* mewcxSvnKeywordAuthor()
{
    return "$Author$";
}

const char* mewcxSvnKeywordUrl()
{
    return "$URL$";
}

const char* mewcxSvnKeywordRevision()
{
    return "$Revision$";
}

unsigned int mewcxSvnRevision()
{
    unsigned int revision(0);
    std::string junk;
    std::istringstream ss( mewcxSvnKeywordRevision() );
    ss >> junk >> revision;
    return revision;
}

// vim: set sw=4 ts=8 et ic ai:
