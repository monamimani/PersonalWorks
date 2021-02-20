
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
  }

  libdirs { 
  }

  links {
    "Core",
    "AppCore",
  }
group ""