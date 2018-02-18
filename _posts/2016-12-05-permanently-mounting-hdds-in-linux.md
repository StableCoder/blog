---
layout: post
title: Permanently Mounting Drives in Linux
brief: Sometimes you have more than one drive.
date: 2016-12-05 05:39:00 -0400
tags : linux, bash
summary: Mounting a HDD in Linux is a pretty basic task, really. All you need to do is figure where you want the files to show up in the local filesystem, and figure out the device moniker Linux gave to your HDD.
---

## A place for the files to call home


First thing one needs is a place to put the files to show up. Unlike in Windows, where HDD partitions are given Drive letters and names (such as C:, N:, Z:), one can place partitions to show up anywhere in Linux. Typically, though, people place them in the '/media' folder, but I typically instead place them in the '/' root folder instead.

Wherever one wants them to show up though, you just need to make a directory, like so:

```bash
mkdir /drivemountdir
```

So, for example I want to place it at /home/user/sambadisk I'd do:

```bash
mkdir /home/user/sambadisk
```

## HDD Identification

Next, we need to find the UUID(Universal Unique ID) of the HDD we want to mount. We want the UUID as this allows us to remove and re-add the hard drive as needed and it will re-mount in the same position, without Linux accidentally mounting a different drive in it's place.

To get the UUID, enter the following command(as admin or super user):

```bash
blkid
```

This will spit out all connected drives, and other information such as this:

```bash
/dev/sdb1: UUID="277e1a67-e166-4e48-b1a4-4cee9c4be738" TYPE="ext4"
/dev/sda1: UUID="832B-D051" TYPE="vfat" PARTUUID="dd5cdcd0-9711-47c8-aa33-81fe8fee4f83"
/dev/sda2: UUID="e4041cad-7b1a-4eef-97b6-753c7f6bbde7" TYPE="ext4" PARTLABEL="Debian OS" PARTUUID="bfcee99a-e884-4e60-80fa-cf4a1156545d"
/dev/sda3: UUID="53379bcf-f2d5-4071-827c-d8ab4a0e128f" TYPE="swap" PARTUUID="1118740f-3771-42cb-8b7c-740038b4dabc"
/dev/sdc1: UUID="9d7d2f22-3609-42f7-8b1f-6dfd87d41bab" TYPE="ext4"
```

## fstab - File System Table

Now, to mount to such a location on boot, one needs to amend the ftab. fstab is located in /etc, so one needs to open the file in an editor, such as vim or nano (as root or super-user) and amend it with the following data:

```bash
UUID={YOUR-UID}    {/path/to/mount/point}               {file-system-type}    defaults,errors=remount-ro 0       1
```

So, for an example:

```bash
UUID=277e1a67-e166-4e48-b1a4-4cee9c4be738 /sambadisk ext4 defaults 0 2
UUID=9d7d2f22-3609-42f7-8b1f-6dfd87d41bab /backupdisk ext4 defaults 0 2
```

Now save it, and check to make sure there are no errors by entering:

```bash
mount -a
```

That forces a mount of all devices. If it works there, it will remount according to the fstab on every boot from then on.
