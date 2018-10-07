---
layout: post
title: Mounting mass encrypted drives in Linux
date: 2018-09-09 00:00:00 -0000
tags : linux
summary: Building a bash script for automating the boring bits of unlocking encrypted disk blocks.
---

## The Old

Previously, I had a small dedicated file server, running the cheapest embedded board possible that mounted a couple of mass storage drives available on samba, and a couple of others for backups. Only having a few drives, it was easy enough to make a small script for this custom environment:

<pre class="brush: bash">
echo "Unlocking 'home_files'"
cryptsetup open /dev/sda3 home_files
mount /dev/mapper/home_files /mnt/encrypted/home_files

echo "Unlocking 'general_files'"
cryptsetup open /dev/sdb1 general_files
mount /dev/mapper/general_files /mnt/encrypted/general_files
...
</pre>

However, time has passed, and the server has needed upgrades. One of these, of course, has been the expansion of the number of drives.

This has made the previous use of a custom script a bit too difficult, especially with the new ability for hot-swapping drives in/out and wanting to somewhat automate the ability to unlock these drives quickly. Obviously this calls for a new and improved script!

## Improvements Wanted

As visible from the above code snippet, devices were unlocked using the device identifiers. This did cause an issue when devices, when disconnected and reconnected would sometimes come back with a different device name. In a perfect world, this would be negated by using the drive's block id instead, but cryptsetup doesn't seem to support them. Thus, any new script would have to be able to use a block ID instead to circumvent this possible issue.

Like the old method however, entering the passphrase on a per-device basis is still desired, as I still am a bit too paranoid about using keyfiles, even if using external USB for them.

## The New Deal

So there's two parts to this new script. The first is a file that lists the block IDs of the encrypted drives and where they would be mounted to in the filesystem.

<pre>
cde9db92-blah-blah /mnt/encrypted/home_files
efecba8a-blah-blah /mnt/encrypted/general_files
</pre>

And a second actual script file. This script file starts off by reading in, and parsing the given file line by line. Then it reads the output of blkid line by line.

<pre class="brush: bash">
read -ep "Enter the file to read block/mount information from: " INPUT_FILE
if [ "$INPUT_FILE" != "" ]; then
    i=0
    while read line
    do
        IN_FILEARR[$i]="$line"
        i=$((i+1))
    done < $INPUT_FILE
else
    echo "No file specified."
    exit 0
fi

i=0
while read line
do
    IN_BLKARR[$i]="$line"
    i=$((i+1))
done< <(blkid)
</pre>

With these in memory, the script then goes through, searching for lines that having matching block IDs (but of course, discounting those that are already mounted).

The devices are mapped using their unique block IDs, and, then their mount points are created and the unlocked devices mapped to them.

<pre class="brush: bash">
for blkid_line in "${IN_BLKARR[@]}"
do
    for file_line in "${IN_FILEARR[@]}"
    do
        file_blkid=$( echo $file_line | cut -d " " -f 1)
        file_mnt=$( echo $file_line | cut -d " " -f 2)
        blkid_dev=$( echo $blkid_line | cut -d ":" -f 1)
        # Search for the file line that has a matching block ID
        if [[ $blkid_line == *$file_blkid* && ! $(ls /dev/mapper/$file_blkid) ]]
        then
            echo "Found matching blkid for $file_blkid, with target of $file_mnt"
            cryptsetup open $blkid_dev $file_blkid
            mkdir -p $file_mnt
            mount /dev/mapper/$file_blkid $file_mnt
        fi
    done
done
</pre>

The creative cutting up of the lines given from the file and the blkid command allows for isolation of comparison items, and the devices are mapped using their blkid by the cryptsetup tool as well, such that they are located at `/dev/mapper/<blkid>`.

The largest downside is that for unmounted items, the ls command that checks if the drive is mapped can through what appears to be an error message.