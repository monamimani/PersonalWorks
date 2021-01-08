
ProjectName = path.getbasename(os.getcwd());

project(ProjectName)
  kind "SharedLib"
  location(os.getcwd())

  files {
    "%{prj.name}/**.h",
    "%{prj.name}/**.cpp",
    --"Resources/**.config",
    --"Resources/**.manifest",
  }

  includedirs {
    ProjectsDir .. "%{prj.name}",
    ProjectsDir .. "Core",
    ExternalsDir .. "Microsoft.ML.OnnxRuntime.DirectML.1.6.0/build/native/include",
  }

  libdirs { 
      ExternalsDir .. "Microsoft.ML.OnnxRuntime.DirectML.1.6.0/runtimes/win-x64/native"
  }

  links { 
      "Core",
      "onnxruntime" 
  }

  defines
  {
    "ONNXRT_DLL"
  }

  dllToCopy = {"Microsoft.ML.OnnxRuntime.DirectML.1.6.0/runtimes/win-x64/native/onnxruntime.dll"}
  copyCmds = CreateCopyCmdsFromExternalsDirToCfgTargertDir(dllToCopy)
  postbuildcommands
  {
    copyCmds
  }

