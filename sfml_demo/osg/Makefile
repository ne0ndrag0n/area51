CC = g++
CFLAGS = -pthread -std=c++14 -g -rdynamic -pipe #-fsanitize=address
INCLUDES = -Iinclude
LIBS = -lpthread -lassimp -lGLEW -lGL -lsfml-graphics -lsfml-window -lsfml-system -lsfgui -ljsoncpp `pkg-config --libs openscenegraph`

SRCS = $(wildcard src/*.cpp)

OBJS = $(SRCS:.cpp=.o)

MAIN = osgtest

.PHONY: clean

all:    $(MAIN)
		@echo  osgtest built successfully.

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		find src/ -name "*.o" -type f -delete

run:    ${MAIN}
	./osgtest
