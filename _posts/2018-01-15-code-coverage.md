---
layout: post
title: Adding C++ code coverage with CMake and GCC or Clang
date: 2018-01-15 12:00 UTC
tags: cmake c++
summary: Code coverage gives the ability to determine which sections of code are used, and how much, helping to prevent issues from sections of code that aren't run during tests and thus could potentially fail in production.
---

After the back of adding the basic [clang tools](/2018/01/01/cmake-clang-tools.html), the ball began rolling to see what else could be integrated into the project infrastructure using CMake. The code coverage tools are pretty unique in that they can be linked into a build just by adding extra compiler flags.

## Building

There are two main methods for creating a code-coverage build, the first is by specifying an option that hooks up the current build with the code coverage tools, and the second is a core build just for code coverage.

### As an option

The first mode of inclusion, and the preferred way is to add an option, in this case named `CODE_COVERAGE` then, when selected to ON either via the command line with `-DCODE_COVERAGE=ON` or any CMake UI, will hook in the coverage support to whatever core build type is currently selected in CMake, whether Debug, Release, or any other.

### As a core build

Another option added, as was requested by some was the ability to just select coverage as the actual CMake build type, such as by `-DCMAKE_BUILD_TYPE=coverage`, where it builds automatically as a Release with it's flags and the coverage hooked in, essentially the same as doing `-DCMAKE_BUILD_TYPE=Release -DCODE_COVERAGE=ON`.

## GCC vs Clang

Now, while both GCC and Clang have code coverage capabilities, there is a difference between them. The Clang-based tool can generate HTML output by itself using the llvm-cov tool, which is automatically part of the llvm toolset, where as to convert GCC's coverage files into a nice readable/exportable HTML format requires two tools, lcov and genhtml, which are less commonly found.

## Compilation setup

So, when coverage tools are requested, the first determination to make is if a required compiler is found. I work on Linux, so I haven't found a real good tool for MSVC that really stood out unfortunately, but GCC and Clang both are common enough, and their requirements for programs can be checked for and appropriate flags added.
```
if(CMAKE_BUILD_TYPE STREQUAL "coverage" OR CODE_COVERAGE)
    if("${CMAKE_C_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang" OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
        message("Building with llvm Code Coverage Tools")

        # Warning/Error messages
        if(NOT LLVM_COV_PATH)
            message(FATAL_ERROR "llvm-cov not found! Aborting.")
        endif()

        # set Flags
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        message("Building with lcov Code Coverage Tools")

        # Warning/Error messages
        if(NOT (CMAKE_BUILD_TYPE STREQUAL "Debug"))
            message(WARNING "Code coverage results with an optimized (non-Debug) build may be misleading")
        endif()
        if(NOT LCOV_PATH)
            message(FATAL_ERROR "lcov not found! Aborting...")
        endif()
        if(NOT GENHTML_PATH)
            message(FATAL_ERROR "genhtml not found! Aborting...")
        endif()

        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    else()
        message(FATAL_ERROR "Code coverage requires Clang or GCC. Aborting.")
    endif()
endif()
```

Of course if the programs required for the chosen compiler aren't found, then the script error's out fatally, letting the user know what it needs to complete.

## The actual targets

In order to generate the coverage information, programs must be run to count the code usage, and thus much like the clang tools, functions are used to generate new targets based off the original, which automatically run the target program. Several new target types are then made available, each displaying the information in a different way.

```
# llvm-cov
add_custom_target(${TARGET_NAME}-ccov-preprocessing
    COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}>
    COMMAND llvm-profdata merge -sparse ${TARGET_NAME}.profraw -o ${TARGET_NAME}.profdata
    DEPENDS ${TARGET_NAME})

add_custom_target(${TARGET_NAME}-ccov-show
    COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions
    DEPENDS ${TARGET_NAME}-ccov-preprocessing)

add_custom_target(${TARGET_NAME}-ccov-report
    COMMAND llvm-cov report $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata
    DEPENDS ${TARGET_NAME}-ccov-preprocessing)

add_custom_target(${TARGET_NAME}-ccov
    COMMAND llvm-cov show $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -show-line-counts-or-regions -output-dir=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}-llvm-cov -format="html"
    DEPENDS ${TARGET_NAME}-ccov-preprocessing)

add_custom_command(TARGET ${TARGET_NAME}-ccov POST_BUILD
    COMMAND ;
    COMMENT "Open ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TARGET_NAME}-llvm-cov/index.html in your browser to view the coverage report."
)
```

### Show

The show variant displays on the command line the coverage information.

### Report

The report shows very quick per-file information about the code.

<span class="image fit">
    <img src="/assets/posts/code-coverage/coverage-report.png" />
</span>

### Regular

This generates an HTML per-file report that can be perused showing in a friendly manner the useage of the code in the run program.

This includes showing how many times each statement was executed when hovered over, and highlights items in red that were never executed. As well as expansions for each template type that was instantiated.

<span class="image">
    <img src="/assets/posts/code-coverage/coverage-full.png" />
</span>

## Source Code

Full fledged (and possibly updated) sources of this can be found under the Apache 2.0 license [here](https://github.com/StableCoder/cmake-scripts).