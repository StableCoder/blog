---
layout: post
title: An all-in-one constructor for typescript classes
brief: Only one constructor, multiple possibilities
date: 2017-03-06 12:00 UTC
tags: software typescript javascript
summary: Because Typescript is just a superset of javascript, having different constructors from C++ is something that I do indeed somtimes miss. In this case, Java/Typescript classes can only have a single constructor, whereas many times I wish to have differing levels of constructor capability.
---

In this case, I have the desire for a constructor that can create a new object, or copy-construct. To help enforce typing, these types also use getters/setters and don't have direct acccess to the internal types. However, to enforce consistency, these classes are paired together with a base interface.

## Interface

The interface type has, as is typical, the minimum set of required items that the class needs to function, and at least the getter ones.

<pre class="prettyprint">
interface IKeyframe {
    time: number;
    region: string;
    rotation: number;
    z_index: number;
    style: string;
}
</pre>

## Class

The class has an internal data store of the data, using the interface, that all of the getters/setters/functions will interact and find the data directly. The constructor can take in an interface of the required data. This interface is, however optional, such that if not supplied, the constructor builds the class with the default values instead.

This has the benefit of being able to take in either of the below options and create a fully usable class.
- Nothing for a new object
- Raw data object(such as from an API)
- Already constructed class

<pre class="prettyprint">
export class Keyframe implements IKeyframe {
    protected data: IKeyframe;

    constructor(data?: IKeyframe) {
        if (data) {
            // Copy construct
            this.data = {
                time: Number(data.time),
                region: data.region,
                rotation: Number(data.rotation),
                z_index: Number(data.z_index),
                style: data.style,
            };
        } else {
            // New object.
            this.data = {
                time: 0,
                region: '',
                rotation: 0,
                z_index: 0,
                style: ''
            };
        }
    }

    get time(): number { return this.data.time; }
    get region(): string { return this.data.region; }
    get rotation(): number { return this.data.rotation; }
    get z_index(): number { return this.data.z_index; }
    get style(): string { return this.data.style; }
}
</pre>
