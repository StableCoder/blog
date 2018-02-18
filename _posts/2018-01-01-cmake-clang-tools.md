---
layout: post
title: Integrating clang-tools with CMake
brief: CMake and clang-tools. A match made in heaven?
date: 2018-01-01 12:00 UTC
tags: software, cmake, clang, c++, c, format, tidy
summary: The clang tools, clang-format and clang-tidy can be immensly useful, both for software development on local machines and as passes in a CI pipeline. This is especially true when they are integrated into a CMake script, where new targets 'format' and 'tidy' can be exist.
---

## clang-format

This tool is useful for having to *not* worry about formatting anymore. When run, clang-format takes in a set of formatting rules that it will apply to all of the files given to it. I have used a multitude of programs before, typically those shipped with IDE's but they've always suffered from the issue of being IDE specific, whereas I change tools over time as new ones gain favour and the others lose it for whatever reason.  To learn more, checkout the documentation [here](https://clang.llvm.org/docs/ClangFormat.html).

Regardless, this tool has gained enough prominance recently to become a pretty viable option, as it's fairly simple to use, fully cross platform and easy to integrate into any environment. Not only that, but providing the formatting options is as simple as including a .clang-format file in the base directory. In this manner the formatting can be consistent across developers, especially if it included in a project's version control.

First is to check if clang-format can be found using cmake's find_program() functionality, and only add the new 'format' target if the program is found. This is so that the CMake script and program can still work on platforms without the tool. Next is to collect all of the files to be formatted. In this case, making use of the GLOB_RECURSE to collect all files ending in h/c/hpp/cpp in the include/ or src/ directories. Then, the custom target 'format' is created, calling the aforementioned program with the options, in this case specifying that the options are in a .clang-format file, and the files to apply it to. The `-i` option indicates to apply the formatting options to the files *in-place*, rather than spitting out the formatted version of the files to the command line.

```
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/*.[ch]pp
    )

    add_custom_target(
        TARGET format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_CXX_SOURCE_FILES}
    )
endif()
```

## clang-tidy

This is a static analyzer. It's goes over the code and tries to spot problems without performing a full time-consuming compilation. To learn more, check out the documentation [here](https://clang.llvm.org/extra/clang-tidy/).

Much of the same that applies to clang-format applies to the tidy tool as well, including the ability to store the tidy options in a file.

The same items apply to tidy as did for format, however to perform an analysis, the tidy tool needs to know from where to check for header files for the sources. These are appended with `-I*include dir*`. In this trivial example, using the project's root directory works.

```
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.[ch]pp
        ${PROJECT_SOURCE_DIR}/src/*.[ch]
        ${PROJECT_SOURCE_DIR}/include/*.[h]pp
        ${PROJECT_SOURCE_DIR}/include/*.[h]
    )

    add_custom_target(
        TARGET tidy
        COMMAND clang-tidy
        ${ALL_CXX_SOURCE_FILES}
        -format-style=file
        --
        -std=c++17
        -I{PROJECT_ROOT_DIR}/include
    )
endif()
```

## Reuse and multiple-project solutions

The above works, but only for simple projects. If there's more than a single project or place to use the tools, the above scripts simply fail. So here are three changes to make their use even easier:

### Multi-project targeting

The simplest, is to simply make the created custom targets on a per-project basis. So, rather than having just a single wide-ranging 'format' or 'tidy' target, one can have many sub targets that use them as a main target. As an example, having a solution with two projects, a and b, rework of the script to prefix the targets with the project name and a single overarching target could leave with:
- a_format
- b_format
- format

Now, calling `make format` once generate will call both `a_format` and `b_format` as part of the `format` target, or one could save time and concentrate on just the single a_project. Regardless, this way a multitude of projects can coexist without necessarily stepping on each others toes.

```
if(NOT CLANG_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
endif()
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.[ch]pp
        ${PROJECT_SOURCE_DIR}/src/*.[ch]
        ${PROJECT_SOURCE_DIR}/include/*.[h]pp
        ${PROJECT_SOURCE_DIR}/include/*.[h]
    )

    add_custom_target(
        TARGET ${PROJECT_NAME}_format
        COMMAND clang-format
        -i
        -style=file
        ${ALL_CXX_SOURCE_FILES}
    )

    if(NOT TARGET format)
        add_custom_target(format)
    endif()

    add_dependencies(format ${PROJECT_NAME}_format)
endif()
```
The `NOT TARGET format` guard prevents more than one 'format' target from existing in the whole solution no matter the number of times it is called, and instead all the sub-targets are made dependencies of the single `format` target.


### Automating tidy's includes

Unfortunately, tidy needs the include directories to be able to function properly. For a single project, it's not too bad. But if there's multiple places to be searching for header files to include, then the project's own include directory may not be enough.

This can be mitigated using two tactics:
- Modify the script to run inside of a macro, where extra include folders can be given.
- Get the target's include folders directly from it's CMake properties. This does come with the caveat that the macro MUST be called after the target is fully defined, otherwise the target's  All of these are then processed to prefix them with the required `-I`.

```
# Generates a 'tidy' target using a custom name, files, and include directories all being parameters.
#
# TIDY_TARGET_NAME - The name of the target to create. If it's a real target name, then the files for it will
#   be inherited, and the include directories as well, and the target name will have the suffix of '_tidy' added.
# ARGN - The list of files to process, and any items prefixed by '-I' will become an include directory instead.
#
# Do note that in order for sources to be inherited properly, the source paths must be reachable from where the macro
# is called, or otherwise require a full path for proper inheritance.
function(_ClangTidy TIDY_TARGET_NAME)
    if(NOT CLANG_TIDY)
        find_program(CLANG_TIDY "clang-tidy")
    endif()
    if(CLANG_TIDY)
        # Process the target if it is a real target files attached with it.
        if(TARGET ${TIDY_TARGET_NAME})
            get_target_property(_TARGET_TYPE ${TIDY_TARGET_NAME} TYPE)
            # Sources
            if(NOT _TARGET_TYPE STREQUAL "INTERFACE_LIBRARY")
                get_property(_TEMP
                    TARGET ${TIDY_TARGET_NAME}
                    PROPERTY SOURCES
                )
                foreach(iter IN LISTS _TEMP)
                    if(EXISTS ${iter})
                        set(TIDY_CODE_FILES "${TIDY_CODE_FILES}" "${iter}")
                    elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${iter})
                        set(TIDY_CODE_FILES "${TIDY_CODE_FILES}" "${CMAKE_CURRENT_SOURCE_DIR}/${iter}")
                    endif()
                endforeach()
            endif()

            # Includes
            _GetLibraryIncludeDirectories(_TEMP ${TIDY_TARGET_NAME})
            foreach(iter IN LISTS _TEMP)
                    set(TIDY_INCLUDE_DIRS "${TIDY_INCLUDE_DIRS}" "-I${iter}")
            endforeach()

            set(TIDY_TARGET_NAME ${TIDY_TARGET_NAME}_tidy)
        endif()

        # Go through the parameters and figure out which are code files and which are include directories
        set(params "${ARGN}")
        foreach(param IN LISTS params)
            string(SUBSTRING ${param} 0 2 TIDY_TEMP_STRING)
            if(TIDY_TEMP_STRING STREQUAL "-I")
                set(TIDY_INCLUDE_DIRS "${TIDY_INCLUDE_DIRS}" "${param}")
            else()
                set(TIDY_CODE_FILES "${TIDY_CODE_FILES}" "${param}")
            endif()
        endforeach()

        if(NOT TIDY_CODE_FILES STREQUAL "")
            add_custom_target(
                ${TIDY_TARGET_NAME}
                COMMAND clang-tidy
                ${TIDY_CODE_FILES}
                -format-style=file
                --
                -std=c++${CMAKE_CXX_STANDARD}
                ${TIDY_INCLUDE_DIRS}
            )

            if(NOT TARGET tidy)
                add_custom_target(tidy)
            endif()

            add_dependencies(tidy ${TIDY_TARGET_NAME})
        endif()
    endif()
endfunction()
```

This result in a much more robust implementation that is much easier to use, as long as the macro is called. If there are folders not included with the target, or manual folder inclusion is desired, then they can be passed in as either variables or strings e.g. `_ClangTools(targetA "/home/x/include /usr/include /usr/local/include")`.

## Integrating into Version Control and CI

The greatest benefit here is allowing for the use of version control hooks to automatically format files, or reject them if tidy fails, without requiring a full compilation. This is as simple as generating the makefiles through CMake and running `make format` or `make tidy` and seeing what returns. Same holds true for CI use, where calling the appropriate targets is nice and easy.

## Source Code

Full fledged (and possibly updated) sources of this can be found under the Apache 2.0 license [here](https://github.com/StableTec/cmake-scripts).