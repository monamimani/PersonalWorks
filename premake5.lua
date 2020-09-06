-- https://github.com/premake/premake-core/wiki/Project-API
-- https://github.com/premake/premake-core/wiki/Lua-Library-Additions

require('vstudio')
require "export-compile-commands"

local vs = premake.vstudio.vc2010
premake.override(vs, "propertySheets", function(base, cfg)
  base(cfg)
  premake.push('<ImportGroup Label="PropertySheets" %s>', vs.condition(cfg))
  premake.w('<Import Project="$(SolutionDir)CommonProps.props" Condition="exists(\'$(SolutionDir)CommonProps.props\')" />')
  premake.pop('</ImportGroup>')
end)

RootDir = os.realpath(os.getcwd() .. "/");
SolutionDir = os.realpath(RootDir);
ExternalsDir = os.realpath(RootDir .. "Externals/");
ProjectsDir = os.realpath(RootDir .. "Projects/");

printf("ProjectsDir: %s", ProjectsDir)
printf("ExternalsDir: %s", ExternalsDir)

workspace "PersonalWorks"

-- We set the location of the files Premake will generate
  --location("Projects/" .. _ACTION)
  location(SolutionDir)

  language "C++"
  cppdialect "C++latest"
  warnings "Extra"
  staticruntime "Off"
  characterset "ASCII"

  targetdir (RootDir .. "_Bin/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")
  objdir (RootDir .. "_BinIntermediate/%{cfg.architecture}-%{cfg.buildcfg}/%{prj.name}/")

  architecture "x64"
  configurations { "Debug", "Release", "Shipping" }
  -- We use filters to set options, a new feature of Premake5.
  -- We now only set settings for the Debug configuration
  filter { "configurations:Debug" }
    symbols "On"
    defines { "_DEBUG"}

  -- We now only set settings for Release
  filter { "configurations:Release" }
    optimize "Debug"
    symbols "On"
    defines { "NDEBUG"}

  filter { "configurations:Shipping" }
    optimize "Full"
    symbols "Off"
    defines { "NDEBUG"}

  filter { "system:windows" }
    systemversion "latest"

  -- Reset the filter for other settings
  filter { }

  startproject "GfxPlaygroundApp"

  -- This will not work in my monorepo, I'll need to specify manually the projects that are needed
  printf("premake5.lua files found:")
  local premake5Files = os.matchfiles( ProjectsDir .. "**/premake5.lua") 
  -- printf("%s", table.tostring(premake5Files))
  for key, filePath in pairs(premake5Files) do
    printf("[%s]: %s", key, filePath)
    include(filePath)
  end
