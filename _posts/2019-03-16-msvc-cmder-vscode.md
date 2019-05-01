---
layout: post
title: Using Cmder for MSVC development (in VSCode too!)
date: 2019-03-16 00:00:00 -0400
tags : windows vscode cmder msvc c++ cmake
summary: You too can develop for MSVC without using the full Visual Studio IDE!
---

So, you're trying to do easy development using the command line on Windows, and have tried out that fancy [Cmder](https://github.com/cmderdev/cmder) tool and found it really nice. You even got it [integrated into VSCode](https://github.com/cmderdev/cmder/wiki/Seamless-VS-Code-Integration) so it pops up instead of the Command Prompt or Powershell.

Then, you wanted to use more than two cores when compiling your C/C++ code on Windows, which means you *can't* use Microsoft's MSBuild environment, which doesn't scale, *at all*. So, you decide to try the [Ninja](https://ninja-build.org/) build tools on Windows to use *all* your threads without issue, by default. But, uh-oh, trying to run CMake Ninja generator on Windows for MSVC *can't* find the MSVC tools, or their include or library directories. What to do?

Well, luckily there's a solution. Everytime Microsoft's programs run, they have a 'Developer's Terminal Environment' which has all those items enumerated in the environmental variables, and it does so by calling a small batch file, typically located at `<MSVS_ROOT_DIR>/VC/Auxiliary/Build/vcvars64.bat` (Typically C:\Program Files (x86)\Microsoft Visual Studio\2017\ and then the installation type such as BuildTools\, Professional\, etc). Calling this in a regular command prompt will magically add everything you need to develop for MSVC. Cool.

### Cmder

But of course, you want this to be called everytime you start Cmder, and there's an easy way to do this in the settings tab of Startup->Tasks, where you can add to the initial bat script the `cmd /k ""%ConEmuDir%\..\init.bat" && call "<MSVS_ROOT_DIR>/VC/Auxiliary/Build/vcvars64.bat"`. And this works great, on the regular Cmder shell. But, this *doesn't* work in the integrated VSCode terminal with Cmder. Phooey.

### VSCode Integrated Terminal

Well, the secret is that initially called `init.bat` file. That file is called regardless of whether from the regular CMder shell or VSCode, and that file itself will call the `%CMDER_ROOT%\config\user_profile.cmd` file (which is the file that isn't replaced on CMder update). Find that file, and add the `call "<MSVS_ROOT_DIR>/VC/Auxiliary/Build/vcvars64.bat"`, and open the shell anywhere, and you can have the MSVC variables always available for easy use all the time!

<span class="image fit">
    <img src="/assets/posts/msvc-cmder-vscode/finally.png" />
</span>

### VSCode Tasks

Now, while that seems to work fine for the regular integrated terminal, attempting to use VSCode tasks may still fail with a bunch of verbose output and then a regular prompt instead of doing the job. This can be  dealt with by manually defining the task run environment in the `tasks.json` file:

```
{
    "version": "2.0.0",
    "windows": {
        "options": {
            "shell": {
                "executable": "cmd.exe",
                "args": [
                    "/d", "/c", "<CMDER_ROOT>\\vendor\\init.bat", "&&"
                ]
            }
        }
    },
    "tasks": [
        {
            "label": "Build",
            "type": "shell",
            "command": "cd ${workspaceFolder}/build && make"
        },
...
```

This overrides VSCode's default behaviour, alowing us to initialize it ourselves and then in the tasks use our commands. A bit hacky, but it works.