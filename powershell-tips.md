---
layout: page
title: Powershell Tidbits
summary: This page is just filled with powershell oddities that I found useful, and I'm sure others will too.
top_post: true
---

## Temporarily Allow Running of Scripts

By default, Windows doesn't allow running any ol' Powershell script. Thus, here's a quick way to allow running of scripts ffor the current open session

<pre class="brush: powershell">
Set-ExecutionPolicy Bypass -Scope Process -Force
</pre>

For a more permanent solution, consult the full documentation [here.](https://docs.microsoft.com/en-us/powershell/module/microsoft.powershell.security/set-executionpolicy?view=powershell-6)

## Environment Variables

Rather than the `$envVar` of *nix or `%envVar%` of the older Command Prompt, the following is the pattern for Powershell:
<pre class="brush: powershell">
$env:envVar
</pre>

## Multiline to file

Sometimes one just has to write to a file. Here's the best known way:
<pre class="brush: powershell">
{Line one
LineTwo} > "file.txt"
</pre>

## Updating variables for the machine/process/user

<pre class="brush: powershell">
# Machine
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","Machine") + ";C:\other\bin", [System.EnvironmentVariableTarget]::Machine )
# Process
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","Process") + ";C:\other\bin", [System.EnvironmentVariableTarget]::Process )
# User
[Environment]::SetEnvironmentVariable( "PATH", [System.Environment]::GetEnvironmentVariable("PATH","User") + ";C:\other\bin", [System.EnvironmentVariableTarget]::User )
</pre>

## Chaining commands

Much like in any real language, in Powershell, different commands can be specified in order on a single line
<pre class="brush: powershell">
command1; command2
</pre>

## dos2unix/unix2dos

Every programmer's nightmare, line ending differences. These quick one-liners can solve that issue!

<pre class="brush: powershell">
# dos2unix
$x = get-content -raw -path &lt;FILE_NAME>; $x -replace "`r`n","`n" | set-content -path &lt;FILE_NAME>

# unix2dos
$x = get-content -raw -path &lt;FILE_NAME>; $x -replace "[^`r]`n","`r`n" | set-content -path &lt;FILE_NAME>
</pre>

## Getting the windows release eg. 1709, 1803

<pre class="brush: powershell">
(Get-ItemProperty "HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion").ReleaseId
</pre>

## Refresh the PATH in the current Powershell session

<pre class="brush: powershell">
$env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
</pre>

## Turning Hyper-V On/Off

After the initial installation of Hyper-V, using these can allow turning it off and on with a restart of the computer, without (un)installing it from the features menu.

<pre class="brush: powershell">
# Enable
bcdedit /set hypervisorlaunchtype auto

# Disable
bcdedit /set hypervisorlaunchtype off
</pre>