#pragma once

#ifdef GFXHAL_DLL
#define GFXHAL_EXPORT _declspec(dllexport) //NOLINT
#else
#define GFXHAL_EXPORT _declspec(dllimport)  //NOLINT
#endif