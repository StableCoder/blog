---
layout: post
title: Docker without sudo
brief: Simple, handy and effective.
date: 2018-05-11 12:00 UTC
tags: linux docker
summary: It can become tiresome to precede every docker command with sudo. Adding oneself to the docker group can avoid that.
---

#### Obligatory warning: Only do this if you're super sure about the security of your local device.

In order to use docker without sudo permissions as a user, without granting all powers to a user, the user can instead be granted full access to docker by joining the docker group on that machine.

<pre class="brush: bash">
# Add the docker group if it doesn't already exist.
$ sudo groupadd docker

# Add user to the docker group
$ sudo usermod -aG docker &lt;user>

# Log group change for user
$ newgrp docker
</pre>

At this point, docker can be successfully used without requiring sudo permissions.