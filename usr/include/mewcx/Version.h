// $Id$
// $URL$
// http://mew.cx/

#ifndef MEWCX_VERSION
#define MEWCX_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

#define MEWCX_EXPORT

extern MEWCX_EXPORT const char* mewcxGetVersion();
extern MEWCX_EXPORT const char* mewcxGetLibraryName();
extern MEWCX_EXPORT const char* mewcxGetBuildDateTime();

extern MEWCX_EXPORT const char* mewcxSvnKeywordId();
extern MEWCX_EXPORT const char* mewcxSvnKeywordDate();
extern MEWCX_EXPORT const char* mewcxSvnKeywordAuthor();
extern MEWCX_EXPORT const char* mewcxSvnKeywordUrl();
extern MEWCX_EXPORT const char* mewcxSvnKeywordRevision();

extern MEWCX_EXPORT unsigned int mewcxSvnRevision();

#undef MEWCX_EXPORT

#ifdef __cplusplus
}
#endif

#endif

// vim: set sw=4 ts=8 et ic ai:
