---
layout: post
title: Gitlab CI Advanced Use Tips
brief: Items for advanced GitlabCI use.
date: 2018-04-15 12:00 UTC
tags: gitlab gitlabci linux
summary: While for the most part the Gitlab CI Runner documentation is great, there are a few options that aren't as known that are useful for more advanced use or requirements.
---

# Privileged Docker Runner

While a regular CI docker runner can run docker images, there are times when one would wish to be able to build Docker images as well, from within the GitlabCI docker runner. As stated in the official documentation [here](https://docs.gitlab.com/runner/executors/docker.html#the-privileged-mode), one needs to set the config for the CI runner to have `privileged = true`, however this alone is not enough.

Often, especially on SELinux, one will also have to modify the docker runner config's `volumes` value to also include the docker socket that's being passed through, to allow it to succeed:

```yaml
  [runners.docker]
    ...
    privileged = true
    volumes = ["/var/run/docker.sock:/var/run/docker.sock", "/cache"]
    ...
```

# CAP_ADD

There are often times when a docker container may require enhanced access to the underlying kernel to perform what you want.

Especially in my case, attempting to run the [MemorySanitizer or LeakSanitizer]({% post_url 2018-02-01-analyzer-build-types %}) to detect memory leaks from within a Docker container would result in errors, as the sanitizer instrumentation could not access ptrace to hook into allocations and deallocations.

This can be worked around by adding the `--cap-add` to add Linux capabilities listed [here](https://docs.docker.com/engine/reference/run/#runtime-privilege-and-linux-capabilities). To add them to the Gitlab-spawned docker images, however, simply add the same-named cpabilities to the GitlabRunner config like so:

```yaml
  [runners.docker]
    ...
    cap_add = ["NET_ADMIN"]
    cap_drop = ["DAC_OVERRIDE"]
    ...
```