// $Id$
// $URL$

#ifndef OSGFV_VERSION
#define OSGFV_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

#define OSGFV_EXPORT

extern OSGFV_EXPORT const char* osgfvGetVersion();
extern OSGFV_EXPORT const char* osgfvGetLibraryName();
extern OSGFV_EXPORT const char* osgfvGetBuildDateTime();

extern OSGFV_EXPORT const char* osgfvSvnKeywordId();
extern OSGFV_EXPORT const char* osgfvSvnKeywordDate();
extern OSGFV_EXPORT const char* osgfvSvnKeywordAuthor();
extern OSGFV_EXPORT const char* osgfvSvnKeywordUrl();
extern OSGFV_EXPORT const char* osgfvSvnKeywordRevision();

extern OSGFV_EXPORT unsigned int osgfvSvnRevision();

#undef OSGFV_EXPORT

#ifdef __cplusplus
}
#endif

#endif

// vim: set sw=4 ts=8 et ic ai:
