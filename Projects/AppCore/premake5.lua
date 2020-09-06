
  project(path.getbasename(os.getcwd()))
    kind "StaticLib"
    location(os.getcwd())

    files {
      "%{prj.name}/**.h",
      "%{prj.name}/**.cpp",
    }
    local tmp = premake.findProjectScript("Core");
    printf("%s", tmp)

    includedirs {
      "%{prj.location}",
      ProjectsDir .. "Core"
    }

    links {
      "Core"
    }