

get_filename_component(targetName ${CMAKE_CURRENT_LIST_DIR} NAME)

if(BUILD_KIND STREQUAL "Dev")
  set(BUILD_KIND_DEV ON)
  set(CONFIG_DEV ON)
elseif(BUILD_KIND STREQUAL "QA")
  set(BUILD_KIND_QA ON)
  set(CONFIG_QA ON)
elseif(BUILD_KIND STREQUAL "Profile")
  set(BUILD_KIND_PROFILE ON)
  set(CONFIG_PROFILE ON)
elseif(BUILD_KIND STREQUAL "Shipping")
  set(BUILD_KIND_SHIPPING ON)
  set(CONFIG_SHIPPING ON)
endif()


if(CMAKE_TESTING_ENABLED)
    set(CONFIG_TESTS ON)
endif()

#configure_file("${targetName}/ConfigGenerated.h.in" "${targetName}/ConfigGenerated.h" ESCAPE_QUOTES)

file(READ ${targetName}/ConfigGenerated.h.in configContents)
string(CONFIGURE "${configContents}" configContents ESCAPE_QUOTES)
file(GENERATE
    OUTPUT ${targetName}/ConfigGenerated.h
    CONTENT "${configContents}"
)

list(APPEND interfaceSources
  ${targetName}/Config.h
)

add_library(${targetName} INTERFACE)
add_library(${PROJECT_NAME}::${targetName} ALIAS ${targetName})


target_compile_definitions(${targetName} INTERFACE CONFIG_$<IF:$<CONFIG:Debug>,DEBUG,RELEASE>)

target_include_directories(${targetName}
  INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  "${CMAKE_CURRENT_BINARY_DIR}"
)

target_sources(${targetName}
  INTERFACE ${interfaceSources}
)
