#ifndef __VER_H__
#define __VER_H__

// major
#ifdef __MAJOR_VER
#define MAjOR __MAJOR_VER
#else
#define MAJOR 0
#endif

// minor
#ifdef __MINOR_VER
#define MINOR __MINOR_VER
#else
#define MINOR 1
#endif

// build number
#ifdef __BUILD_NUMBER
#define BUILD __BUILD_NUMBER
#else
#define BUILD 0
#endif

#endif