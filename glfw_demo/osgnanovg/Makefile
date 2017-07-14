CC = g++
DFLAGS = -DGLEW_STATIC -DNANOVG_GLEW -DNANOVG_GL3_IMPLEMENTATION
CFLAGS = -pthread -std=c++14 -g -rdynamic -pipe $(DFLAGS) #-fsanitize=address
INCLUDES = -Iinclude
LIBS = -lpthread -lassimp -lGLEW -lGL -lsfml-graphics -lsfml-window -lsfml-system -lsfgui -ljsoncpp `pkg-config --libs openscenegraph` `pkg-config --libs glfw3`

SRCS = $(wildcard src/*.cpp)
SRCS += $(wildcard src/lib/*.cpp)

OBJS = $(SRCS:.cpp=.o)

MAIN = osgnanovg

.PHONY: clean

all:    $(MAIN)
		@echo  osgnanovg built successfully.

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		find src/ -name "*.o" -type f -delete

run:    ${MAIN}
	./osgnanovg
