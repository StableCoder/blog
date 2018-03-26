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