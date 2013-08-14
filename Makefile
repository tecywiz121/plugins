CCXX=g++
RM=rm -f

CXXFLAGS=-g -Wall -Wextra -Werror -Wstrict-aliasing -std=c++11 -pedantic -fPIC

SRC=src/loader.cpp src/PluginLibrary.cpp src/PluginManager.cpp src/PluginDescription.cpp src/Plugin.cpp
OBJ=$(SRC:.cpp=.o)

LUA_SRC=mod/lua.c
LUA_OBJ=$(LUA_SRC:.c=.o)

LIBS=-lSDL2

INCLUDES= -Iinclude/

all: main lua

main: $(OBJ)
	$(CCXX) $(CXXFLAGS) $(INCLUDES) -o main $(OBJ) $(LIBS)

lua: $(LUA_OBJ)
	$(CCXX) $(CXXFLAGS) $(INCLUDES) -shared -o lua.so $(LUA_OBJ) $(LIBS)

.c.o:
	$(CCXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

.cpp.o:
	$(CCXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) src/*.o *.so main 
