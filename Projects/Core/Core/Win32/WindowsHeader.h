#pragma once

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif                        // !WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
  #define NOMINMAX
#endif // !NOMINMAX

// Including SDKDDKVer.h defines the highest available Windows platform.
// If you wish to build your application for a previous Windows platform, include WinSDKVer.h and
// set the _WIN32_WINNT macro to the platform you wish to support before including SDKDDKVer.h.
#include <SDKDDKVer.h>

// Windows Header Files:
#include <windows.h>
#undef NOMINMAX
#undef WIN32_LEAN_AND_MEAN