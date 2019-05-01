---
layout: post
title: Using CMake to help build Catch tests
date: 2018-12-23 00:00:00 -0400
tags : cmake c c++ catch
summary: Using CMake to automatically detect, or clone fresh, and even pre-compile Catch for you.
---

In many of my newer projects, I make extensive use of the Catch2 test framework. This is a handy-dandy single header library that encompasses many testing needs, and can be found [here](https://github.com/catchorg/Catch2.git).

This isn't about that, though. Not directly. Rather, this post is about effectively pulling in a copy of the Catch2 library and effectively using it, whether it's already available as a pre-installed on the system, needs to be cloned raw from the git repository, or even having it pre-compiled once for use across many test programs. Of course, one *could* include a copy of the single header in a project's repository, but that'd be too easy.

Rather, here we'll use CMake to do this work for us, and however it is found, CMake can make it available for use, simply by presenting us with the `catch` target to link to.

## Local Find

The simplest case is that of checking for the header on the local machine. No need to involve the network unless we don't have it, eh? This is simply enough, where we can make use of CMake's `find_file` function, giving it the name of the file we're looking for, and the typical paths it can be found on.

If found, then the `catch` target then includes the directory where it was found, and that'll be transferred to any other targets that link to it as well.

```cmake
find_file(HAVE_CATCH_HPP catch.hpp PATH_SUFFIXES catch2 catch)

if(HAVE_CATCH_HPP)
    get_filename_component(CATCH_PATH ${HAVE_CATCH_HPP} DIRECTORY)
    target_include_directories(catch INTERFACE ${CATCH_PATH})
endif()
```

## Cloning

But what about the case where it *isn't* found locally? In that case it's time to clone it instead. This can be beneficial, as this would ensure that it always builds with the latest version of Catch.

In any case, this uses the `ExternalProject_Add` command to clone the Catch2 repository from GitHub, grabs that location, and then includes the location of the included single-include header from it.

```cmake
ExternalProject_Add(
    git_catch
    PREFIX ${CMAKE_BINARY_DIR}/catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(git_catch source_dir)
add_dependencies(catch git_catch)
target_include_directories(catch PUBLIC ${source_dir}/single_include  ${source_dir}/single_include/catch2)
```

## Pre-Compilation

As with any other extensive project, single-header includes typically have some form of integrated source, requiring some special define to compile some code (In this case, the macro of `CATCH_CONFIG_MAIN`). On smaller projects, or projects that have only a single testing binary, it's not much of a problem. You were going to have to compile it sometime anyways.

However, if you practice some form of test segmentation or fragmentation, then the time taken to re-compile that same code across all those binaries can start to add up.

Thus, what we *could* do is just compile it once in the `catch` target we already have, and instead share that compiled form across all the others. Not only that, but this alleviates the worry of possibly forgetting declaring the macro elsewhere, or even figuring out where to call it in the test programs themselves, since it's already taken care of.

```cmake
if(COMPILED_CATCH)
    # A pre-compiled catch library has been requested
    message(STATUS "Generating a pre-compiled Catch2 library")

    if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp)
        # Create the file with the source macro only.
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp "#define CATCH_CONFIG_MAIN\n#include <catch.hpp>\n")
    endif()
    add_library(catch ${CMAKE_CURRENT_BINARY_DIR}/pre_compiled_catch.cpp)
else()
    add_library(catch INTERFACE)
endif()
```

## End Result

Whatever the case, compiled, or no, local or cloned, our end program doesn't need to know nor care. All it needs to do is link to the `catch` target.

```cmake
add_executable(a main.cpp)
target_link_libraries(a PUBLIC catch)
```

Of course, the above setup can be done with practically any other single-header project that can be found, such as the [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) without much hassle.

## Implementation

A ready-to-go form of this can be found [here](https://github.com/StableCoder/cmake-scripts) as the tests.cmake file.