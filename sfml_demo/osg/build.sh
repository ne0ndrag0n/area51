g++ model.cpp main.cpp -std=c++14 -g -Iinclude -lpthread -lassimp -lGLEW -lGL -lsfml-graphics -lsfml-window -lsfml-system -lsfgui -ljsoncpp `pkg-config --libs openscenegraph`
