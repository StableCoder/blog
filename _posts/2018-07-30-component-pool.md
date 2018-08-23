---
layout: post
title: Using one value to share many
brief: Re-using a regular unsigned integer to convey multiple values.
date: 2018-07-28 00:00:00 -0000
tags : ecs
summary: One of the most fundamental items in any system, oftware or otherwise, is to be able to identify something uniquely.
---
With the last post, we have an alright ability to identify an incredible number of individual entities, with some embedded information within their IDs. Now what is needed is the ability to store the data related to the objects, with the ability to easily access them.

One idea would be to store the objects in-order, so that they could be accessed by their IDs instead of another handle type. Unfortunately, while this would seem like a workable idea, performing some quick benchmarks just shoots the idea down right away.

<binary-search benchmark here>

The binary search through pre-sorted arrays, or a sequential search just takes far too much time to be used for regular fast access. If access through this method is on a low number and generall infrequent, it could be made acceptable. However in a real-time system, it's just unacceptable.

<benchmark for map, unordered map>

So at this point, what it seems to be needed is some sort of hybrid between the direct access of a vector/pointer and a sorted search of a map-type for attempting a blind search.

However, our requirements go a little further than that. We intend for this to be used in a multithreaded environment. This means that the data pools must have some sort of synchronization policy. A traditional STL container (vector, map, unordered_map) there is no guaruntee that the data will stay in the same place in memory after an insert or erasure (iterators are invalidated often). To prevent long lockdowns of data access during processing, it would be best to prevent reallocations/moves during active processing.

But we still need a way to add/remove objects from these pools. So, what can be done is to borrow some traits from the land of Garbage Collection of the more dynamic languages. Rather than instantly performing the requested operation, instead the operations can be queued up to be done in a 'maintenance' type phase, where pools are exclusively locked and can perform all insertions, removals and any other required heavy work all at once, with no heavy synchronization required.

Doing this does have some other benefits, as it means developers can operate with some assumptions:
- When not in 'maintenance', accessors and iterators are guarunteed not to be invalidated, nor will memory addresses change.
- Maintenance occurs in a specific order. Such as removal -> verification -> insertion. THis means that an entities component data can be removed/inserted between maintenance cycles, and because removals occur before insertions, a full replacement can be ensured to occur correctly without requiring synchronization work to avoid possible duplicates.
- If multiple attempts to insert are attempted, the phase will filter out in a particular known-before way (ie pick the last unique one, or the first).
- If all component data sets perform maintenance at the same time, then it also means that the components for an entity will be locked-in until the next phase.
- Between maintenance, lists of exactly what changed can be used to quietly pick out or change other outside systems in batches instead of piecemeal, allowing for easier/possibly faster system maintenance.

## Accessing

To keep genenral access times low, it is best to use an offset handle. This is the fastest without actually giving a direct pointer itself, whilst still allowing the underlying memory to shift and be moved as required.