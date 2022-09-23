#pragma once

#include <string>
#include <cstdint>
#include <any>

namespace ApplicationCore::Window
{

struct WindowDesc
{

  bool m_isVisible = true;
  bool m_isFullscreen = false;
  bool m_isResizable = true;
  uint32_t m_monitorIndex = 0;

  std::string m_title = "";
  std::string m_id = "";
  std::string m_iconPath = "";

  bool m_isCentered = false;
  int32_t m_x = 0;
  int32_t m_y = 0;
  uint32_t m_width = 0;
  uint32_t m_height = 0;

  std::any m_nativeAppDesc;
};

} // namespace ApplicationCore::Window