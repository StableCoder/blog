---
layout: post
title: Default high-quality on Linux for Bluetooth audio devices
date: 2019-02-22 00:00:00 -0400
tags : linux pulseaudio audio bluetooth
summary: Tired of it defaulting to garbage HSP quality all the time?
---

Bluetooth audio devices that have a micorphone often default to the HSP quality level when connected to a Linux system, which is incredibly annoying.

To change the audio codec to use when it auto-connects, try this:

Open the default file for PulseAudio, found at `/etc/pulse/default.pa`.

```bash
# Change this line
load-module module-bluetooth-policy

## To this
load-module module-bluetooth-policy auto_switch=2
```

This neat trick was found originally [here](https://www.reddit.com/r/linux/comments/at0zo0/til_that_since_2017_its_possible_in_pulseaudio_to/).