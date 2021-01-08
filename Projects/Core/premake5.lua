
ProjectName = path.getbasename(os.getcwd());

project(ProjectName)
  kind "StaticLib"
  location(os.getcwd())

  files {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
    "fmt/**.h",
    "fmt/**.cc",
  }
  excludes {
    "%{prj.name}/**.test.h",
    "%{prj.name}/**.test.cpp",
  }

  includedirs {
    ProjectsDir .. "%{prj.name}",
    ExternalsDir .. "GoogleTest/include/",
  }



project(ProjectName.. ".Tests")
  kind "ConsoleApp"
  location(os.getcwd())
  
  files {
    "%{ProjectName}/**.h",
    "%{ProjectName}/**.cpp",
    -- This is redundant but is here for clarity that those include the test files
    "%{ProjectName}/**.test.h",
    "%{ProjectName}/**.test.cpp",
  }

  includedirs {
    ProjectsDir .. "%{ProjectName}",
    ExternalsDir .. "GoogleTest/include/",
  }

  filter { "configurations:Debug" }
    libdirs { 
      ExternalsDir .. "GoogleTest/lib/Debug"
    }

    links { 
      "gtestd", "gmockd", "gtest_maind" 
    }

    dllToCopy = {"GoogleTest/bin/Debug/gtestd.dll", "GoogleTest/bin/Debug/gtest_maind.dll"}
    copyCmds = CreateCopyCmdsFromExternalsDirToCfgTargertDir(dllToCopy)
    -- copyCmds = {}
    -- table.foreachi(dllToCopy, function(value) 
    --   table.insert(copyCmds, "{COPY} " .. ExternalsDir .. value .. " " .. "%{cfg.targetdir}")
    -- end)

      --printf("%s", table.concat(copyCmds,"\n "))
    postbuildcommands
    {
      copyCmds
    }

  filter { "configurations:Release or Shipping" }
    libdirs { ExternalsDir .. "GoogleTest/lib/RelWithDebInfo"}

    links { 
      "gtest", "gmock", "gtest_main" 
    }