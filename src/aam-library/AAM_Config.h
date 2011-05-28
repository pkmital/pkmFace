/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#ifndef AAM_CONFIG_H
#define AAM_CONFIG_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <opencv2/opencv.hpp>

#ifdef WIN32
#include <direct.h>
#include <io.h>
#define AAM_MKDIR(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define AAM_MKDIR(dir) mkdir(dir, 0644)
#endif

#ifdef WIN32
#define AAM_EXTERN_C extern "C"
#define AAM_EXPORTS __declspec(dllexport)
#else
#define AAM_EXTERN_C
#define AAM_EXPORTS
#endif
#define AAM_API AAM_EXTERN_C AAM_EXPORTS

#ifndef byte
#define byte unsigned char
#endif

#define TYPE_AAM_BASIC	0
#define TYPE_AAM_IC		1

typedef std::vector<std::string> file_lists;

extern char errmsg[];

#define AAM_ERROR(msg)	AAM_Error(msg, __FILE__, __LINE__)

#ifdef __cplusplus
extern "C"{
#endif

AAM_API double AAM_GetTime();

AAM_API void AAM_FormatMSG(const char* format, ...);

AAM_API void AAM_Error(const char *msg, const char* file, int line);

AAM_API std::ostream& operator<<(std::ostream &os, const CvMat* mat);

AAM_API std::istream& operator>>(std::istream &is, CvMat* mat);


#ifdef __cplusplus
}
#endif

#endif //AAM_CONFIG_H
