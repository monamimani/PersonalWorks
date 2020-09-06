
group "Executables"
project(path.getbasename(os.getcwd()))
  kind "WindowedApp"
  location(os.getcwd())
  
  files {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
  }

  includedirs {
    "%{prj.location}",
    ProjectsDir .. "Core",
    ProjectsDir .. "AppCore",
    ProjectsDir .. "Gfx/GfxHAL"
  }

  links {
    "Core",
    "AppCore",
    "GfxHAL"
  }