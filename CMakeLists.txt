# If one day someone read this file and is better than me with CMake, please, make some order in this chaos. 
# I still learning and i know my CMakefile will not work in other machine than mine

# Minimum version of Cmake
cmake_minimum_required(VERSION 4.0.3)

set(BUILD_SHARED_LIBS OFF)

if(NOT DEFINED CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
endif()
set(CMAKE_PREFIX_PATH "$ENV{MSYS2_MINGW64_STATIC_LIB}\\cmake" ${CMAKE_PREFIX_PATH})

# Project Name, version and language
project(OvGME VERSION 1.8.0.1 DESCRIPTION "Correction moteur de téléchargement OvGME" LANGUAGES CXX C)

# indicate to CMake that we are in C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(Boost_USE_STATIC_LIBS ON)

# Force CMake to prioritise static lib
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)

# Export compile command for intellisense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Configure GCC in static
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

# indicate that the source file of curl lib is linked and compiled in static
add_definitions("-DCURL_STATICLIB")

add_link_options(-static -static-libgcc -static-libstdc++)
add_compile_options(-Wall)
# add header directories
include_directories(include include/thirdparty/miniz include/thirdparty/pugixml include/thirdparty/xxhash)
include_directories("$ENV{MSYS2}/mingw64/include")
include_directories("$ENV{MSYS2}/mingw64/include/c++/15.2.0")
include_directories("$ENV{VCPKG_ROOT}/installed/x64-mingw-static/include/curl")

find_package(CURL REQUIRED)        
# Link the static libraries into my envioronnement
link_directories("$ENV{MSYS2_MINGW64_STATIC_LIB}")

set_source_files_properties(resource.rc PROPERTIES LANGUAGE RC)

# Command to recursively browse the src folder and all its subfolders to retrieve the list of all files and store this list in
# MY_SOURCES, CONFIGURE_DEPENDS will allow the project to be regenerated automatically during the next build if something 
# changes
file(GLOB_RECURSE OVGME_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.c)

message(STATUS "Sources : ${OVGME_SOURCES}")

# add our .exe compiled
add_executable(${PROJECT_NAME} WIN32 main.cpp resource.rc ${OVGME_SOURCES})
# Prefer static runtime for the target and request static linkage for winpthread
# This tells the linker to prefer static libgcc/libstdc++ and to link winpthread
# statically if available
target_link_options(${PROJECT_NAME} PRIVATE "-static" "-static-libgcc" "-static-libstdc++" "-Wl,-Bstatic")

set(MY_LIBRARIES libwinpthread.a "$ENV{VCPKG_ROOT}/installed/x64-mingw-static/lib/libcurl.a" wsock32 ws2_32 Gdi32 Comctl32 Comdlg32 Shell32 Shlwapi msimg32 windowscodecs Ole32 ssh2 iconv unistring.a idn2.a intl.a crypto.a libz.a bcrypt.lib iphlpapi.lib secur32.lib Crypt32.lib)

# Absolute path of archives
set(LIBSTDCXX_A "$ENV{MSYS2}/mingw64/lib/libstdc++.a")
## libgcc.a is located under the gcc subdirectory (versioned). Use the real path.
set(LIBGCC_A "$ENV{MSYS2}/mingw64/lib/gcc/x86_64-w64-mingw32/15.2.0/libgcc.a")

# link explicitely archives with the right order
target_link_libraries(${PROJECT_NAME} PRIVATE
  ${LIBSTDCXX_A}
  ${LIBGCC_A}
  ${MY_LIBRARIES}
)

add_custom_target(copy_files ALL
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/CHANGELOG.txt ${CMAKE_BINARY_DIR}/$<CONFIG>/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/README.txt ${CMAKE_BINARY_DIR}/$<CONFIG>/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/res/icon.ico ${CMAKE_BINARY_DIR}/$<CONFIG>/
)