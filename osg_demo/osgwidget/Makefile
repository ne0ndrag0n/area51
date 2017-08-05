CC = g++
DFLAGS =
CFLAGS = -pthread -std=c++14 -g -rdynamic -pipe $(DFLAGS) #-fsanitize=address
INCLUDES = -Iinclude -Ilib -Ilib/nanovg
LIBPATHS = -Llib/nanovg
LIBS = -lpthread -lassimp -lGLEW -lGL -lsfml-graphics -lsfml-window -lsfml-system -lsfgui -ljsoncpp `pkg-config --libs openscenegraph` -lnanovg
LFLAGS =

export LD_LIBRARY_PATH := /usr/local/lib64:/usr/lib/gcc4/x64/release/
export PKG_CONFIG_PATH := /usr/local/lib64/pkgconfig

SRCS = $(wildcard src/*.cpp)

OBJS = $(SRCS:.cpp=.o)

MAIN = osgtest

.PHONY: clean

all:    $(MAIN)
		@echo  osgtest built successfully.

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) $(LIBPATHS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		find src/ -name "*.o" -type f -delete

run:    ${MAIN}
	./osgtest
