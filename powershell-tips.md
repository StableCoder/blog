---
layout: page
title: Powershell Tidbits
brief: Some estranged pieces of acquired knowledge.
---

This page is just filled with powershell oddities that I found useful, and I'm sure others will too.

## Environment Variables

Rather than the `$envVar` of *nix or `%envVar` of the older command prompt, the following is the pattern for powershell:
```
$env:envVar
```

## Multiline to file

Sometimes one just has to write to a file. Here's the best known way:
```
{Line one
LineTwo} > "file.txt"
```

## Change the path at the user or system level

In order to permanently change an environment variable in Windows, one can use the `setx` command, and to set it to be system wide, add the suffix switch of `/M`.
```
# Changes the path for the user
setx PATH "%PATH%;C:\new\path" 

# Changes the path for the entire system
setx PATH "%PATH%;C:\new\path" /M
```

## Chaining commands

Much like in any real language, in Powershell, different commands can be specified in order on a single line
```
command1; command2
```

## dos2unix/unix2dos

Every programmer's nightmare, line ending differences. These quick one-liners can solve that issue!

```ps
# dos2unix
$x = get-content -raw -path <FILE_NAME>; $x -replace "`r`n","`n" | set-content -path <FILE_NAME>

# unix2dos
$x = get-content -raw -path <FILE_NAME>; $x -replace "[^`r]`n","`r`n" | set-content -path <FILE_NAME>
```

## Getting the windows release eg. 1709, 1803

```ps
(Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion").ReleaseId
```

## Updating variables for the machine/process/user

```ps
# Machine
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","Machine") + ";C:\other\bin", [System.EnvironmentVariableTarget]::Machine )
# Process
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","Process") + ";C:\other\bin", [System.EnvironmentVariableTarget]::Process )
# User
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","User") + ";C:\other\bin", [System.EnvironmentVariableTarget]::User )
```

## Refresh the PATH in the current Powershell session

```ps
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
```

## Turning Hyper-V On/Off

After the initial installation of Hyper-V, using these can allow turning it off and on with a restart of the computer, without (un)installing it from the features menu.

```ps
# Enable
bcdedit /set hypervisorlaunchtype auto

# Disable
bcdedit /set hypervisorlaunchtype off
```