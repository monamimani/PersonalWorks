
group "Gfx"
project(path.getbasename(os.getcwd()))
  kind "StaticLib"
  location(os.getcwd())
  
  files {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
  }

  includedirs {
    "%{prj.location}",
    ProjectsDir .. "Core",
    ProjectsDir .. "Gfx/GfxHAL"
  }

  links {
    "Core",
    "GfxHAL"
  }