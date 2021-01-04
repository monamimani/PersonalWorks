#pragma once

#ifdef ONNXRT_DLL
#define ONNXRT_API _declspec(dllexport) //NOLINT
#else
  #define ONNXRT_API _declspec(dllimport) //NOLINT
#endif