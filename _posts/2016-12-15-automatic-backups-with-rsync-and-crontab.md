---
layout: post
title: Automated backups with rsync and crontab
date: 2016-12-15 15:40:00 -0400
tags: linux
summary: The goto post after you've learned the lesson. Creating automated backups is, in fact, incredibly simple and easy. So simple, that one can set it up in less than two minutes, and is guarunteed to be the best thing EVER in case of HDD failure, or otherwise corruption.
---

So, first thing, is to install rsync, which of course requires admin or super user priviledges.

<pre class="brush: bash">
apt-get install rsync
</pre>

With that done, one needs to create a backup script somewhere that will be run, so create a file somewhere.

<pre class="brush: bash">
nano backup.sh
</pre>

Now, enter the command for the rsync backup, with the desired options.

```
0 0 * * * rsync -a /path/to/source /path/to/destination
```

Note that the first 5 characters determine the time when this script will run, whereas 'rsync' and on is the actual command. Make sure that the account that will run this script has access to both directories.

```
#Min | Hour | Day | Month | Day of Week | Run Command
 0 	   0 	    *      *         *         rsync -ar /sambadisk /backupdisk
# Set to run everyday at midnight.
```

Now, with the script setup and ready, one just needs to add it to the crontab (Command Run On Notice TABle).

<pre class="brush: bash">
crontab backup.sh
</pre>

To check that it was added successfully, just enter "crontab -l" to list all running crons.