Application Design
==========================================

Class Overview
==========================================
.. uml::

  set namespaceSeparator ::

  class AppCore::App::App
  class AppCore::App::Win32::Win32App
  class AppCore::App::Win32::Win32AppDesc

  class AppCore::Window::Window

  AppCore::App::App <-- AppCore::App::Win32::Win32App

  AppCore::App::Win32::Win32App o- AppCore::Window::Window


Application sequence
==========================================
.. uml::

  hide footbox

  participant "main()"
  participant Application

  activate "main()"

  "main()" -> Application: <<Construct>>
  activate "Application"
  "main()" -> Application: <<init>>
  "main()" -> Application: <<run>>
  Application -> Application: <<update>>

  "main()" <- Application: <<run return>>
  "main()" -> Application: <<shutdown>>
  deactivate "Application"
  deactivate "main()"