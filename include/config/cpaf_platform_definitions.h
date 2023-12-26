#ifndef CPAF_PLATFORM_DEFINITIONS_H
#define CPAF_PLATFORM_DEFINITIONS_H


// IMPORTANT: Must match the definitions set in [cpaf/]tools/cmake/platform_definitions.cmake
//            The reason for the redundancy is to make it easier for users that do not want
//            to compile this using cmake!

#define CPAF_PLATFORM_TYPE_ID_DESKTOP       1
#define CPAF_PLATFORM_TYPE_ID_EMBEDDED      2
#define CPAF_PLATFORM_TYPE_ID_DESKTOP_WEB   3
#define CPAF_PLATFORM_TYPE_ID_MOBILE        4
#define CPAF_PLATFORM_TYPE_ID_TABLET        5
#define CPAF_PLATFORM_TYPE_ID_TABLET_WEB    6
#define CPAF_PLATFORM_TYPE_ID_MOBILE_WEB    7


#define CPAF_SYSTEM_ID_LINUX			1
#define CPAF_SYSTEM_ID_WINDOWS			2
#define CPAF_SYSTEM_ID_OSX				3
#define CPAF_SYSTEM_ID_FREERTOS			4
#define CPAF_SYSTEM_ID_BROWSER			5
#define CPAF_SYSTEM_ID_ANDROID			6
#define CPAF_SYSTEM_ID_IOS				7
#define CPAF_SYSTEM_ID_SDL2				30
#define CPAF_SYSTEM_ID_GODOT			31
#define CPAF_SYSTEM_ID_QT				32

#endif // CPAF_PLATFORM_DEFINITIONS_H
