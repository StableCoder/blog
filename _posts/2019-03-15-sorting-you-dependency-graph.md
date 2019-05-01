---
layout: post
title: Simplifying your C/C++ dependency graph (with CMake)
date: 2019-03-15 12:00 UTC
tags: cmake c++
summary: Sometimes, you'll come upon a set of libraries that just decide to include everything directly, always. This can help minimize and visualize those dependencies.
---

## The Before

Typically first, looking at how the dependencies are laid out is the first objective. Luckily CMake, has integrated support for graphviz for building a visual graph showing this information, at least for the libraries under it's own management.

CMake can generate a `.dot` file that can be used by the dot program from graphviz to output a file (of which there are [*many*](https://graphviz.gitlab.io/_pages/doc/info/output.html) possible target types) to view, where directly *added* dependencies are represented by arrows.

<pre class="brush: bash">
$ cmake &lt;SOURCE_DIR> --graphviz=test.dot
$ dot -Tpng test.dot -o out.png
</pre>

Of course, like many things, this can be automated and turned into a CMake target to make the whole process just a *tad* easier:

```cmake
find_program(DOT_EXE "dot")
if(DOT_EXE)
    message(STATUS "dot found: ${DOT_EXE}")
else()
    message(STATUS "dot not found!")
endif()

set(DOT_OUTPUT_TYPE "" CACHE STRING "Build a dependency graph. Options are dot output types: ps, png, pdf..." )

if(DOT_EXE)
    add_custom_target(dependency-graph
        COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} --graphviz=${CMAKE_BINARY_DIR}/graphviz/${PROJECT_NAME}.dot
        COMMAND ${DOT_EXE} -T${DOT_OUTPUT_TYPE} ${CMAKE_BINARY_DIR}/graphviz/${PROJECT_NAME}.dot -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}
    )

    add_custom_command(
        TARGET dependency-graph POST_BUILD
        COMMAND ;
        COMMENT
        "Dependency graph generated and located at ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.${DOT_OUTPUT_TYPE}"
    )
endif()
```

Then calling it, such as `make dependency-graph` will generate something like below:

<span class="image fit">
    <img src="/assets/posts/dependency-simplification/before.png" />
</span>

As can be seen it's a little bit of a mess. Seemingly everyone decided to include most other items, Such as demosSandbox including dMath directly itself, through dCustomJoins, dNewton and dVehicle as grand children anyways. In one case includes the same library twice (dScene -> dContainers). Surely this can be simplified?

## Simplification

Now, given the above image, one's first step would normally be to go around through the make files and start removing direct dependencies wherever they may be, and this is normally effective. However, there can be times when the dependencies aren't quite clear, for example where a library may list one that is higher up the tree than is required. For this, one can check what libraries a shared executable or shared object actually uses, by checking the inverse, or what is included but not used and can be removed. This can be done with the `ldd` tool, and using the `-u` option for 'unused' items:

<pre class="brush: bash">
$ ldd -u libname.so
Unused direct dependencies:
        /lib/lib1.so
        /lib/lib2.so
</pre>

Armed with this information, you can prune down the imported items even further, and even more accurately determine how to structure links.

## The After

So, doing the above for a while, one will be able to sort out who actually needs what, and simplify, and make clear the dependency ordering, and hopefully come up with something not only more visually appealing, but easier to keep in mind and use:

<span class="image fit">
    <img src="/assets/posts/dependency-simplification/after.png" />
</span>

## Implementation

A ready-to-go form of this can be found [here](https://github.com/StableCoder/cmake-scripts) as the dependency-graph.cmake file.