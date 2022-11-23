#pragma once

#ifndef __VIDEO_H_

#define __VIDEO_H_

#include <Windows.h>

#define MAX_VIDEO_STRING_SIZE		255
#define DEFAULT_VIDEO_MODE			VGA
#define DEFAULT_SCALE_COEFFCIENT	1

/*
 Define common screen resolutions

 To set the client area size of your window you just need to define one of the follow resolutions

 To set the vars up with the correct values just #define GraphicsMode to "VIDMODE"

 "VIDMODE" can be with one of the below, either the number or the the abbreviation
 0 = QVGA = 320x240
 1 = VGA = 640x480
 2 = SVGA = 800x600
 3 = WSVGA = 720x480

 4 = XGA = 1024x768
 5 = XGA_Plus = 1152x864
 6 = WXGA = 1280x720
 7 = WXGA_Plus = 1440x900
 8 = WSXGA_Plus = 1680x1050
 9 = WUXGA = 1920x1200

 10 = SD = 720x576
 11 = HD = 1366x768
 12 = HD_Plus = 1600x900
 13 = FHD = 1920x1080
 14 = QHD = 2560x1440
 15 = UHD = 3840x2160
 */

#define QVGA		0
#define VGA			1
#define	SVGA		2
#define WSVGA		3
#define XGA			4
#define XGA_PLUS	5
#define WXGA		6
#define WXGA_PLUS	7
#define WSXGA_PLUS	8
#define WUXGA		9
#define SD			10
#define HD			11
#define HD_PLUS		12
#define FHD			13
#define QHD			14
#define UHD			15
#define CUSTOM1		16
#define CUSTOM2		17
#define CUSTOM3		18

 /* Struct to represent all our video modes, it has been created so you can get the information
	you need by using the above defines. Exmaple HD information,

	VideoDisplayMode[HD].szFullname

	This will return "High Definition" string
 */

typedef struct tagVIDEO_MODE_INFO
{
	unsigned int nIndex;
	unsigned int nLengthX;
	unsigned int nHeightY;
	TCHAR szName[MAX_VIDEO_STRING_SIZE];
	TCHAR szFullName[MAX_VIDEO_STRING_SIZE];
	TCHAR szResolution[MAX_VIDEO_STRING_SIZE];

}VIDEO_MODE_INFO, *PVIDEO_MODE_INFO;

// I cant think of anything else to add more to this, I'm sure I will add as I go along

static VIDEO_MODE_INFO VideoDisplayModes[] =
{
	0,	320,	240,	L"QVGA",	L"Quarter Video Grapics Array",				L"320x240",
	1,	640,	480,	L"VGA",		L"Video Graphics Array",					L"640x480",
	2,	800,	600,	L"SVGA",	L"Super Video Graphics Array",				L"800x600",
	3,	720,	480,	L"WSVGA",	L"Wide Super Video Graphics Array",			L"720x480",
	4,	1024,	768,	L"XGA",		L"Extended Graphics Array",					L"1024x768",
	5,	1152,	864,	L"XGA+",	L"Extended Graphics Array Plus",			L"1152x864",
	6,	1280,	720,	L"WXGA",	L"Wide Extended Graphics Array",			L"1280x720",
	7,	1440,	900,	L"WXGA+",	L"Wide Extended Graphics Array Plus",		L"1440x900",
	8,	1680,	1050,	L"WSXGA+",	L"Wide Super Extended Graphics Array",		L"1680x1050",
	9,	1920,	1200,	L"WUXGA",	L"Wide Ultra Extended Graphics Array",		L"1920x1200",
	10,	720,	576,	L"SD",		L"Standard Definition",						L"720x576",
	11, 1366,	768,	L"HD",		L"High Definition",							L"1366x768",
	12, 1600,	900,	L"HD+",		L"High Definition Plus",					L"1600x900",
	13, 1920,	1080,	L"FHD",		L"Full High Definition",					L"1920x1080",
	14, 2560,	1440,	L"QHD",		L"Quad High Definition",					L"2560x1440",
	15, 3840,	2160,	L"UHD",		L"Ultra High Definition",					L"3840x2160",
	16, 1800,	900,	L"CUSTOM1",	L"Custom High Resolution for Ray Tracer",	L"1800x900",
	17, 900,	450,	L"CUSTOM2",	L"Custom Half Resolution for Ray Tracer",	L"900x450",
	18, 200,	100,	L"CUSTOM3",	L"Custom Low Resolution for Ray Tracer",	L"200x100"
};

#endif

