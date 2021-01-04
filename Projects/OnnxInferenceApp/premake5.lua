
group "Executables"
project(path.getbasename(os.getcwd()))
  kind "ConsoleApp"
  location(os.getcwd())
  
  files {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
  }

  includedirs {
    "%{prj.location}",
    ProjectsDir .. "Core",
    ProjectsDir .. "OnnxRT",
    ExternalsDir .. "Microsoft.ML.OnnxRuntime.DirectML.1.6.0/build/native/include",
  }

  libdirs { 
      ExternalsDir .. "Microsoft.ML.OnnxRuntime.DirectML.1.6.0/runtimes/win-x64/native"
  }

  links {
    "Core",
    "OnnxRT",
    "onnxruntime" 
  }

group ""