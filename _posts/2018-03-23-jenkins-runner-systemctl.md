---
layout: post
title: Jenkins Agent Service
brief: Getting Jenkins to run automatically on boot.
date: 2018-03-23 12:00 UTC
tags: jenkins systemctl scheduler agent linux windows
summary: While this can be reapplied more broadly, this is specifically about getting the Jenkins agents to run automatically on boot, for both Linux and Windows.
---

## Linux

I'm only going over systemctl-based Linux here, always got to look forwards.

Anyways, first is of course to build a user to run the jenkins agent. Having an agent run as root or the regular user account is just asking for serious trouble, as any job that is run *could* contain malicious intent if the source repositories aren't monitored closely. This user should have their own home directory, so that the Jenkins agent jar and working directories can be kept safely isolated there. However, don't give a password, so that no one can login in externally directly to them, as in some cases you may give them some sudo access to certain directories, such as to docker.

```sh
$ useradd -m jenkins-runner
```

Now, create an entry in `/etc/systemd/system` directory to create a systemctl service that outlines it's properties, such that it can be maniuplated like any other service. Be sure to add the user to run the service as.

```
[Unit]
Description=Jenkins Runner

[Service]
ExecStart=/usr/bin/java -jar /home/jenkins-runner/agent.jar -jnlpUrl <url>/slave-agent.jnlp -secret <key> -workDir "/home/jenkins-runner/jenkins"
Restart=always
RestartSec=30
User=jenkins-runner

[Install]
WantedBy=multi-user.target
```

After, call `systemctl daemon-reload` to find the new service file, then using the typical commands of start/stop/enable/disable will act accordingly for the service.

### Give non-sudo access to docker

How to allow use of docker a non-privileged user?

Best case is to use/create the docker group that has automatic non-sudo access to docker itself.

```sh
groupadd docker
gpasswd -a jenkins-runner docker
# Refresh user groups
newgrp docker
```