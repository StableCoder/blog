---
layout: post
title: Making CMake work for you
date: 2018-10-30 00:00:00 -0400
tags : cmake c c++
summary: Built over some time now, is a fair sized suite of tools and utilities to improve the CMake compilation experience.
---
Note: This is a followup to the posts '[Integrating clang-tools with CMake]({{ "2018/01/01/cmake-clang-tools.html" | relative_url}})', '[Adding C++ code coverage with CMake and GCC or Clang]({{ "2018/01/15/code-coverage.html" | relative_url}})' and '[Integrating sanitizer tools to CMake builds]({{ "2018/02/01/analyzer-build-types.html" | relative_url}})', which details older versions of the same thing.

##### Note: [All code below can be found in better pre-made scripts found here under the Apache 2.0 license.](https://git.stabletec.com/other/cmake-scripts)

## C++ Standards

These obviously force the standard to be required, and also disables compiler-specific extensions, ie `--std=gnu++11`. This helps to prevent fragmenting the code base with items not available elsewhere, adhering to the agreed C++ standards from the committee only.

#### Basic CMake Implementation

```
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
```

## clang-tidy, cppcheck and include-what-you-use

> clang-tidy is a clang-based C++ “linter” tool. Its purpose is to provide an extensible framework for diagnosing and fixing typical programming errors, like style violations, interface misuse, or bugs that can be deduced via static analysis. clang-tidy is modular and provides a convenient interface for writing new checks.
>
> [clang-tidy page](https://clang.llvm.org/extra/clang-tidy/)

cppcheck is a similar tool to clang-tidy. include-what-you-use helps to organize headers for all files encompass all items being used in that file, without accidentally relying upon headers deep down a chain of other headers.

For any of the tools, they can be setup by setting the respective variables with the desired options. As examples, the below code checks for the executable, and if found, setups the full command line options to use.

#### Basic CMake Implementation (clang-tidy only)

```
find_program (CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-format-style=file;-header-filter='${CMAKE_SOURCE_DIR}/*'" CACHE STRING "" FORCE)
else()
    message(STATUS "clang-tidy NOT found!")
    set(CMAKE_CXX_CLANG_TIDY "" CACHE STRING "" FORCE) # clear it
endif()
```

## Sanitizers (LLVM/clang)

Sanitizers are tools that perform checks during an executable's runtime and returns issues, and as such, along with unit testing, code coverage and static analysis, is another tool to add to the programmers toolbox. And of course, like the previous tools, are tragically simple to add into any project using CMake, allowing any project and developer to quickly and easily use. These sanitizers work by appending flags to the compilation of the code, and are currently only best compatible with clang/LLVM.
- [LeakSanitizer](https://clang.llvm.org/docs/LeakSanitizer.html) detects memory leaks, or issues where memory is allocated and never deallocated, causing programs to slowly consume more and more memory, eventually leading to a crash.
- [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html) is a fast memory error detector. It is useful for detecting most issues dealing with memory, such as:
    - Out of bounds accesses to heap, stack, global
    - Use after free
    - Use after return
    - Use after scope
    - Double-free, invalid free
    - Memory leaks (using LeakSanitizer)
- [ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html) detects data races for multi-threaded code.
- [UndefinedBehaviourSanitizer](https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html) detects the use of various features of C/C++ that are explicitly listed as resulting in undefined behaviour. Most notably:
    - Using misaligned or null pointer.
    - Signed integer overflow
    - Conversion to, from, or between floating-point types which would overflow the destination
    - Division by zero
    - Unreachable code
- [MemorySanitizer](https://clang.llvm.org/docs/MemorySanitizer.html) detects uninitialized reads.

Take for example the following program:
<pre class="brush: cpp">
int main(int argc, char **argv) {
    int *array = new int[100];
    delete[] array;
    return array[argc]; // BOOM
}
</pre>
This obviously has a glaring error, where the array is being used after it has already been freed. Compiling and running the program however all seems fine. It compiles, runs, and exits without letting on any issue. However, add the AddressSanitizer to the build, compile and run, then BAM:
<span class="image fit">
    <img src="/assets/posts/cmake-helper-suite/asan.png" />
</span>

Problem detected, and detailed for further investigation. What's not to like?

These sanitizers can be instrumented to any binary. Got a really rare problem that rarely comes up? Instrument the program with the appropriate sanitizer, then just use the runtime until the problem comes up again, no matter how long it takes.

#### Basic CMake Implementation

```
if (USE_SANITIZER STREQUAL "Address")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
elseif (USE_SANITIZER STREQUAL "Thread")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
elseif (USE_SANITIZER STREQUAL "Undefined")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
elseif (USE_SANITIZER STREQUAL "Leak")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
elseif (USE_SANITIZER STREQUAL "Memory")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=memory")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
endif()
```

## Code Coverage (LLVM/clang/GCC)

> In computer science, test coverage is a measure used to describe the degree to which the source code of a program is executed when a particular test suite runs. A program with high test coverage, measured as a percentage, has had more of its source code executed during testing, which suggests it has a lower chance of containing undetected software bugs compared to a program with low test coverage. Many different metrics can be used to calculate test coverage; some of the most basic are the percentage of program subroutines and the percentage of program statements called during execution of the test suite. 
>
> [Wikipedia, Code Coverage](https://en.wikipedia.org/wiki/Code_coverage)

Code coverage is the detailing of, during the execution of a binary, which regions, functions, or lines of code are *actually* executed. This can be used in a number of ways, from figuring out areas that automated testing is lacking or not touching, to giving a user an instrumented binary to determine which areas of code are used most/least to determine which areas to focus on. Although this does come with the caveat that coverage is no guarantee of good testing, just of what code has been.

There are implementations for both clang(llvm-cov) and GCC(gcov) available, although llvm-cov is the better of the two.

Generating code coverage during a run of a program can help determine which blocks, regions, or even lines of code are being used, and for how many times. Both on the command line:
<span class="image fit">
    <img src="/assets/posts/cmake-helper-suite/ccov-report.png" />
</span>
... and in a fully browsable HTML report, detailing how many times each function, region, and line was run through, number of evaluations inside of comparison statements, and through proper expansions of templated code:
<span class="image fit">
    <img src="/assets/posts/cmake-helper-suite/ccov-html-report.png" />
</span>
<span class="image fit">
    <img src="/assets/posts/cmake-helper-suite/ccov-html-detail.png" />
</span>

#### Basic CMake Implementation

```
FIND_PROGRAM(LCOV_PATH lcov)

# Where TARGET_NAME is the executable target's name
add_custom_target(ccov-${TARGET_NAME}
    COMMAND LLVM_PROFILE_FILE=${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}>
    COMMAND llvm-profdata merge -sparse ${TARGET_NAME}.profraw -o ${TARGET_NAME}.profdata
    COMMAND llvm-cov report $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata
    DEPENDS ${TARGET_NAME}
)
```

## Ready-to-go implementation

Located [here](https://git.stabletec.com/other/cmake-scripts), is a full advanced implementation of all the above and more, made easily accessible through a few functions and CMake switches/options. It contains all required documentation for use, as well as examples of code and a CI/CD pipeline that uses them.