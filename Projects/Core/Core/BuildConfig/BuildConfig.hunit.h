#pragma once

import "BuildConfigArchitecture.hunit.h";
import "BuildConfigCompiler.hunit.h";
import "BuildConfigPlatform.hunit.h";

namespace BuildConfig
{

#ifdef _DEBUG
  #define BUILDCONFIG_DEBUG 1
constexpr auto isDebugBuild = true;
#else
  #define BUILDCONFIG_OPTIMIZED 1
constexpr auto isDebugBuild = false;
#endif

constexpr auto isOptimisedBuild = !isDebugBuild;

#ifdef __cplusplus
constexpr auto isCpp = true;
#else
constexpr auto isCpp = false;
#endif

#if defined __GNUC__
  #define BUILDCONFIG_CDECL __attribute__((cdecl))
  #define BUILDCONFIG_STDCALL __attribute__((stdcall))
  #define BUILDCONFIG_FASTCALL __attribute__((fastcall))
#elif (defined _MSC_VER)
  #define BUILDCONFIG_CDECL __cdecl
  #define BUILDCONFIG_STDCALL __stdcall
  #define BUILDCONFIG_FASTCALL __fastcall
#else
  #error Compiler not supported.
#endif

#if defined __GNUC__
  #define BUILDCONFIG_LIB_EXPORT __attribute__((__visibility__("default")))
  #define BUILDCONFIG_LIB_IMPORT
#elif (defined _MSC_VER)
  #define BUILDCONFIG_LIB_EXPORT __declspec(dllexport)
  #define BUILDCONFIG_LIB_IMPORT __declspec(dllimport)
#else
  #error Compiler not supported.
#endif

} // namespace BuildConfig