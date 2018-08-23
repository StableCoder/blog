---
layout: post
title: Some Docker containers won't start
brief: Notably CentOS6 on Linux 4.15+
date: 2018-05-12 12:00 UTC
tags: linux docker centos
summary: On newer kernels, some Docker containers may not start correctly, often giving nonsensical errors.
---

This can be fixed, as found [here](https://bbs.archlinux.org/viewtopic.php?id=234282).

The idea is to add a kernel parameter that will allow these containers to operate correctly again, by adding `vsyscall=emulate`.

Most often, this can be added to the GRUB options.
<pre class="brush: bash">
sudo vi /etc/default/grub

GRUB_CMDLINE_LINUX="vsyscall=emulate"
</pre>

Then remaking the gurb configuration.
<pre class="brush: bash">
sudo grub-mkconfig -o /boot/grub/grub.cfg
sudo reboot
</pre>

For other ways to set a kernel parameter, consult [here](https://wiki.archlinux.org/index.php/Kernel_parameters).