Plugins
=======

Introduction
------------

This is my prototype of a plugin architecture for Trillek.  This is mostly an exercise to get myself reacquainted with C++, so don't expect too much.  That said, I welcome constructive criticism.

Any instructions I give are for Ubuntu, but it shouldn't be too hard to adapt.  I haven't tested this on anything other than Linux, so I don't expect it to work.

Requirements
------------

 * SDL2

  ```$ hg clone http://hg.libsdl.org/SDL;
mkdir build;
cd build;
../configure --prefix=/usr;
make;
sudo make install```

 * lua5.1

  ```$ sudo apt-get install liblua5.1-0-dev```

Architecture
------------

Pretty simple design:

Plugins are described by a simple file format, take a look at test.mod.  Each module consists of a "library" and "arguments."
The same library can be used by multiple plugins, which will be run separately.  This allows plugins to be written in a variety of
languages.

Future Plans
-------

* **Bidirectional API**
  * Currently plugins are sent arguments prior to being started, and that is the last communication that happens.  I'd like to enable bidirectional communication between the plugins and the host program.  I'm thinking some kind of structure that is sent to the plugins on load that contains the relevant functions.

* **Events/Messaging**
 * I'd like to have some kind of event/messaging system where the plugins can subscribe/publish to topics to allow for inter-plugin communication.

* **Other Languages**
 * Adding support for Python and JavaScript are two of my next goals.
