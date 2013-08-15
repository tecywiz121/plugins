Plugins
=======

Introduction
------------

This is my prototype of a plugin architecture for Trillek.  This is mostly an exercise to get myself reacquainted with C++, so don't expect too much.  That said, I welcome constructive criticism.

Any instructions I give are for Ubuntu, but it shouldn't be too hard to adapt.  I haven't tested this on anything other than Linux, so I don't expect it to work.

Requirements
------------

 * SDL2

    $ hg clone http://hg.libsdl.org/SDL
    $ mkdir build
    $ cd build
    $ ../configure --prefix=/usr
    $ make
    $ sudo make install

 * lua5.1

    $ sudo apt-get install liblua5.1-0-dev

Architecture
------------

Pretty simple design:

Plugins are described by a simple file format, take a look at test.mod.  Each module consists of a "library" and "arguments."
The same library can be used by multiple plugins, which will be run separately.  This allows plugins to be written in a variety of
languages.
