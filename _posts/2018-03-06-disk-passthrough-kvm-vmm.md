---
layout: post
title: Disk Passthrough for KVM and VMM
brief: Just a little note for those who could use it.
date: 2018-03-06 12:00 UTC
tags: kvm linux unix software
summary: Through just the Virtual Machine Manager UI, there's not a real way to perform plain disk passthrough to a virtual machine. However, to perform a disk passthrough isn't all that hard with just a tweak to a file.
---

Quite simply, if you're comfortable with the Virutal Machine manager UI, create a VM like normal, and when it comes to giving it a storage volume, just give it a tiny dummy volume that will be discarded ASAP.

Then, find the directory where the virtual machine configurations XML files are found. A typical location would be `/etc/libvirt/qemu/` but could differ.

In that, find the section where the stub of a storage volume exists, it'll look something like this:
```
<disk type='file' device='disk'>
  <driver name='qemu' type='qcow2'/>
  <source file='/home/user/VM/HDD-Stub.qcow2'/>
  <target dev='hda' bus='ide'/>
  <address type='drive' controller='0' bus='0' target='0' unit='0'/>
</disk>
```

and swap the type from a file to a block, and change the driver to a raw type, and finally the source from a file to a device, which is the particular disk drive to passthrough.

```
<disk type='block' device='disk'>
  <driver name='qemu' type='raw'/>
  <source dev='/dev/sdb'/>
  <target dev='vdb' bus='virtio'/>
</disk>
```

The target/bus depends on what type of interface or drivers one will have available. Most notably during a Windows installation a virtio bus won't be recognized right away, requiring extra drivers to be given at installation time from somewhere reputable such as Red Hat or the [Fedora Project](https://docs.fedoraproject.org/quick-docs/en-US/creating-windows-virtual-machines-using-virtio-drivers.html).