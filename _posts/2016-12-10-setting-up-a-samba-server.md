---
layout: post
title: Setting up a Samba Server
brief: It's almost like cloud storage, except secured at home.
date: 2016-12-10 09:58:00 -0400
tags: linux
summary: The ability to have files accessible from any device connected to the network is a great way to store and consume one's data. For example, on my home network, I have a central basic Linux file server, which stores all my music, videos, pictures, etc, such that I can play my 250GB music collection on any of my other connected devices, such as my dekstop, tablet, and phone, without requiring massive storage for the files on each device, or have to worry about updating the collection on each device.
---

## Introduce the Samba System

In order to achieve this, we'll be using Samba, which is integrated into Windows by default, and can easily be installed on Linux as well.

Samba will allow us the ability to access files from the file server as if the files were stored locally on a hard drive, using the same file system as regular files.

![alt text](/assets/posts/samba-setup/sambadisk.jpg "Samba share")

## Creating Samba Share on Linux

To start, one needs to know which folder they want to share, and needs Samba installed on their machine.

<pre class="brush: bash">
# Debian Based
apt-get install samba
# Arch
pacman -S samba
</pre>

Then, one needs to create a user to login to the samba share, to ensure not anyone can access it. The -a means to add a user. One can also remove a user using the -x option.

<pre class="brush: bash">
smbpasswd -a sambauser
</pre>

Next, we need to configure Samba, using it's config files, so, as an admin or super user, enter:

<pre class="brush: bash">
nano /etc/samba/smb.conf
</pre>

And scroll down to the bottom and add the options for the new Samba share.

```
[shareName]
path = /path/to/share
public = no
valid users = sambauser
read only = no
writable = yes
browsable = yes
```

Of course, changine options as needed. As an example, here's my media share options:

```
[sambadisk]
comment =  Files Share
public = no
valid users = myself
read only = no
writable = yes
browsable = yes
path = /sambadisk
```

Once the options are set, save and close the file, then restart the Samba service to let it grab the new options.

<pre class="brush: bash">
service smbd restart
</pre>

## Accessing the Samba share on Windows

Accessing the Samba share on Windows is as easy as creting a shortcut. So, on the Desktop, right-click and go to New -> Shortcut

![alt text](/assets/posts/samba-setup/new-shortcut.jpg "Typical windows right click menu")

Then type in the network location of the new shared folder

```
\\SERVER-IP-ADDRESS\SHARENAME
```

![alt text](/assets/posts/samba-setup/shortcut-addr.jpg "Entering the address of the samba share")

Then enter the username and password for the samba account, and away one goes.

![alt text](/assets/posts/samba-setup/shortcut-complete.jpg "Completed shortcut")