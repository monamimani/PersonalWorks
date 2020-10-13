Input System Design
==========================================

Class Overview
==========================================
.. uml::

  set namespaceSeparator ::

  class AppCore::Window::WindowDesc
  class AppCore::App::Win32::Win32AppDesc
  class AppCore::Window::Window
  class AppCore::Window::WindowImpl
  class AppCore::Window::Win32::Win32Window

  AppCore::Window::WindowDesc o- AppCore::App::Win32::Win32AppDesc
  AppCore::Window::Window o- AppCore::Window::WindowImpl : PIMPL
  AppCore::Window::WindowImpl <-- AppCore::Window::Win32::Win32Window
  AppCore::App::Win32::Win32AppDesc -* AppCore::Window::Win32::Win32Window