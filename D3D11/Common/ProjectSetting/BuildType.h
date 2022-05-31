#pragma once

#define PROJECTD3D_BUILD_UNKNOWN			"unknown"
#define PROJECTD3D_BUILD_DEBUG			    "debug"
#define PROJECTD3D_BUILD_RELEASE			"release"

#if defined(PEDEBUG)
#define BUILD_TYPE	PROJECTD3D_BUILD_DEBUG
    #define PROJECTD3D_DEBUG
#elif defined(PERELEASE)
#define BUILD_TYPE	PROJECTD3D_BUILD_RELEASE
    #define PROJECTD3D_RELEASE
#else
#define BUILD_TYPE	PROJECTD3D_BUILD_UNKNOWN
#define PROJECTD3D_UNKNOWN
#endif

#if defined(PROJECTD3D_DEBUG)
#define IS_DEBUG      1

#elif defined(PROJECTD3D_RELEASE)
#define IS_DEBUG      0
#else
#endif