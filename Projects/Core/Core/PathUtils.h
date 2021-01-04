#pragma once

#include <filesystem>
#ifdef _WIN32
  #include <windows.h> //GetModuleFileNameW
#else
  #include <limits.h>
  #include <unistd.h> //readlink
#endif

static std::filesystem::path getExePath()
{
#ifdef _WIN32
  wchar_t path[MAX_PATH] = {0};
  GetModuleFileNameW(nullptr, path, MAX_PATH);

  auto exeDir = std::filesystem::weakly_canonical(path).parent_path();
  return exeDir;
#else
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  return std::string(result, (count > 0) ? count : 0);
#endif
}

static inline std::filesystem::path getDataPath()
{
  auto exePath = getExePath();
  auto dataPath = exePath.parent_path().parent_path() / "Data";

  return dataPath;
}

static inline std::filesystem::path getCachePath()
{
  auto exePath = getExePath();
  auto dataPath = exePath / "Cache";

  return dataPath;
}