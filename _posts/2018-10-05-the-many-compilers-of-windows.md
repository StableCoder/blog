---
layout: post
title: Compiler Options of Windows
brief: There's more than you'd think.
date: 2018-10-06 00:00:00 -0000
tags : c++, windows, msvc, gcc, clang, llvm
summary: While Microsoft's MSVC may be synonymous with Windows development, there are others.
---

While there may be other compilers, such as Intel's, we're talking about freely available or of the 'mainstream' variety. This includes the 'big three': MSVC, GCC and clang.

# Windows SDK Based

Microsoft releases the tools and libraries required to build for it's own platform through [Visual Studio](https://visualstudio.microsoft.com/). From this, there are two required items, the MSVC compiler itself, and the Windows SDK.

There are several flavours of Visual Studio to choose from:
- Build Tools. Free, and the most compact of the available sets, this allows for the use through the command line and without the typically bundled Visual Studio IDE. This is also the option MS seems most keen to squirrel away from most people.
- Community. Free, comes with the compiler and bundled IDE.
- Professional/Ultimate. Paid versions of the compiler and bundled IDE with more bells and whistles.

#### Installation

For this purpose, the script just installs the basic Build Tools variant, with the compiler and current Windows 10 SDK. Then the clang compiler.

These install quietly/silently without pop-up windows.

<pre class="brush: powershell">
# MSVS Build Tools
wget https://aka.ms/vs/15/release/vs_buildtools.exe -OutFile vs_buildtools.exe -UseBasicParsing
.\vs_buildtools.exe --quiet --wait --norestart --nocache --add Microsoft.VisualStudio.Workload.VCTools --add Microsoft.VisualStudio.Component.Windows10SDK.17134 | Out-Null
rm .\vs_buildtools.exe

# Clang
wget http://releases.llvm.org/7.0.0/LLVM-7.0.0-win64.exe -OutFile llvm.exe -UseBasicParsing
.\llvm.exe /S
rm .\llvm.exe
</pre>

## Microsoft's MSVC

The compiler that comes free from Microsoft, and usually bundled with Visual Studio installations.

#### Usage

When setting up the environment, calling the vcvars64.bat file sets up the MSVC binaries to the `PATH`, as well as the Windows SDK includes/libraries to the `INCLUDE` and `LIB` environmental variables. These variables are used by CMake to detect the compilers, linkers and STL header library to be used.

<pre class="brush: powershell">
# Environment Setup
pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\"
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow
</pre>

With the environment setup, just using CMake to generate the makefile and calling those makefiles is all that's left. During makefile generation, CMake will identify the compilers as `MSVC 19.15.*` or similar.

<pre class="brush: powershell">
# Build
cmake . -Bbuild-msvc -G "NMake Makefiles"
cd build-msvc
nmake
</pre>

## clang-cl

The most well-known alternative to MSVC on Windows is clang-cl. While this compiles code using clang, it outputs objects that is compatible and links using Microsoft's linker.

#### Usage

The difference for environmental setup here is that the C (CC) and C++ (CXX) compilers need to be overwritten from the detected defaults, being `$env:CC="clang-cl"` and `$env:CXX="clang-cl"`.

<pre class="brush: powershell">
# Environment Setup
pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\"
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow

$env:CC="clang-cl"
$env:CXX="clang-cl"
</pre>

The build process is the same as for MSVC. The main difference will be during makefile generation, where CMake will identify the compilers as `Clang 7.0.0` or similar.

<pre class="brush: powershell">
cmake . -Bbuild-clang-cl -G "NMake Makefiles"
cd build-clang-cl
nmake
.\testExe.exe
</pre>

## clang with LLVM

Like the above, this alternative compiles using the clang compiler, but uses LLVM's LLD linker, and LLVM's AR archiver.

This does selection does present some problems, but is mostly usable:
- Build targets follow the Unix pattern. This means libraries are build as `.a` archives, and executables are build without the `.exe` extension, necessitating a rename to run on Windows. But they *do* run.
- LLVM on Windows itself doesn't seem capable of generating shared objects, so any shared targets will be statically linked anyways.
- CMake doesn't try to detect lld on Windows, so some overrides need to be performed on generation.

#### Usage

Only difference for the environment from clang-cl usage is that the compilers follow the unix style CC and CXX names, and the addition of LDFLAGS to force the usage of the lld linker instead of cl.

<pre class="brush: powershell">
# Environment Setup
pushd "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\"
cmd /c "vcvars64.bat&set" |
foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd
Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow

$env:CC="clang"
$env:CXX="clang++"
$env:LDFLAGS="-fuse-ld=lld"
</pre>

During makefile generation, several items need to be manually specified:
- The C and CXX compiler ID's, as CMake by default assumes those on Windows mean 'clang-cl' and attempts those defaults.
- CMAKE_SYSTEM_NAME needs to be overridden, as otherwise it will attempt to create compiler options using the '/' prefix instead of '-'. ex. `/-whole-archive` instead of `--whole-archive`
- The static library archive program needs to be specified when building library targets.

After, any executable targets need to be renamed with the '.exe' extension for Windows to recognize them as executables.

<pre class="brush: powershell">
# Build
cmake . -Bbuild-clang -G "NMake Makefiles" -DCMAKE_C_COMPILER_ID="Clang" -DCMAKE_CXX_COMPILER_ID="Clang" -DCMAKE_SYSTEM_NAME="Generic" -DCMAKE_AR="C:\Program Files\LLVM\bin\llvm-ar.exe"
cd build-clang && nmake
Rename-Item -Path testExe -NewName testExe.exe
</pre>

# MinGW Based

The 'Minimal GNU for Windows', these compilers utilize a full alternative to MS's toolchain, doing an effective job of replacing all the headers and libraries for operating on Windows, although extra DLL's need to be linked in to accomplish this.

#### Installation

Installation is a bit weird, but works. It downloads and un-tars the MSYS environment, which has a complete set of unix-like tools that can be run natively on Windows, and comes with the pacman package manager (like in Arch). It then runs `pacman -Syuu` a couple of times to grab all of the latest updates to all the originally downloaded packages.

After that, it installs the make tool, and then the GCC and Clang compilers.

*This script assumes that the 7zip command-line utility is available*
<pre class="brush: powershell">
wget https://cfhcable.dl.sourceforge.net/project/msys2/Base/x86_64/msys2-base-x86_64-20180531.tar.xz -OutFile C:\msys2.tar.gz -UseBasicParsing

7z x C:\msys2.tar.gz
7z x C:\msys2.tar
Remove-Item -path C:\msys2.tar.gz
Remove-Item -path C:\msys2.tar

# Update it
Set-Alias bash "C:\msys64\usr\bin\bash.exe"
$ErrorActionPreference = 'Continue'
while (!$done) {
    bash -lc 'pacman --noconfirm -Syuu | tee /update.log'
    $done = (Get-Content C:\msys64\update.log) -match 'there is nothing to do' | Measure-Object | ForEach-Object { $_.Count -eq 2 }
    $done = $done -or ($i -ge 5)
}
Remove-Item prefix\msys64\update.log -ea 0

# General
bash -lc 'pacman --noconfirm -S mingw64/mingw-w64-x86_64-make'

# GCC
bash -lc 'pacman --noconfirm -S mingw64/mingw-w64-x86_64-gcc mingw64/mingw-w64-x86_64-lcov'

# Clang
bash -lc 'pacman --noconfirm -S mingw64/mingw-w64-x86_64-clang mingw64/mingw-w64-x86_64-clang-analyzer mingw64/mingw-w64-x86_64-clang-tools-extra mingw64/mingw-w64-x86_64-llvm'

# Clean the cache, saving a bit of space
bash -lc 'pacman --noconfirm -Scc'
</pre>

## GCC

The original [GNU Compiler Collection](https://gcc.gnu.org/), but running on and for Windows.

#### Usage

Setting up the environment is as simple as adding the toolchain to the PATH, andsetting the environment variable to tell CMake to target the specific compiler.

<pre class="brush: powershell">
# Adding the MinGW64 toolchain to PATH
$env:PATH = "${env:PATH};C:\msys64\usr\bin;C:\msys64\mingw64\bin;"

# Setting the compiler to use
$env:CC="gcc"
$env:CXX="g++"
</pre>

Building is also just as simple, by just generating the makefile and then calling mingw32-make, MinGW's version of make on Windows.

<pre class="brush: powershell">
cmake . -Bbuild-gcc -G "MinGW Makefiles"
cd build-gcc
mingw32-make
</pre>

## Clang

The same clang/LLVM project as earlier, but just using the MinGW set of headers and libraries for interfacing with Windows.

#### Usage

Setting up the environment is as simple as adding the toolchain to the PATH, andsetting the environment variable to tell CMake to target the specific compiler.

<pre class="brush: powershell">
# Adding the MinGW64 toolchain to PATH
$env:PATH = "${env:PATH};C:\msys64\usr\bin;C:\msys64\mingw64\bin;"

# Setting the compiler to use
$env:CC="clang"
$env:CXX="clang++"
</pre>

Building is also just as simple, by just generating the makefile and then calling mingw32-make.

<pre class="brush: powershell">
cmake . -Bbuild-clang -G "MinGW Makefiles"
cd build-clang
mingw32-make
.\testExe.exe
</pre>