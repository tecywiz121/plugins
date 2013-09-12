Plugins
=======

Introduction
------------

This is my prototype of a plugin architecture for Trillek.  This is mostly an exercise to get myself reacquainted with C++, so don't expect too much.  That said, I welcome constructive criticism.

Any instructions I give are for Ubuntu, but it shouldn't be too hard to adapt.  I haven't tested this on anything other than Linux, so I don't expect it to work.

Requirements
------------

 * lua5.2

  ```$ sudo apt-get install liblua5.2-dev```

Building
--------

$ mkdir build
$ cd build
$ cmake ../
$ make

Running
-------

$ ./sharedobj test_lua.mod

*NB: If cmake cannot find lua it won't copy test_lua.mod*

Architecture
------------

Pretty simple design:

Plugins are described by a simple file format, take a look at test.mod.  Each module consists of a "library" and "arguments."
The same library can be used by multiple plugins, which will be run separately.  This allows plugins to be written in a variety of
languages.

Future Plans
-------

* **Events/Messaging**
 * I'd like to have some kind of event/messaging system where the plugins can subscribe/publish to topics to allow for inter-plugin communication.

* **Other Languages**
 * Adding support for Python and JavaScript are two of my next goals.
