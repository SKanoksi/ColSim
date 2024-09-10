
# Dependencies: glfw3, glade, glm

CC = gcc
CXX = g++

INCLUDES = -I$(HOME)/.local/include
LIBRARIES = 

CFLAGS = -Wall -O3 $(INCLUDES)
CXXFLAGS = -Wall -O3 -fconcepts $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw

TARGET = ColSim.exe
cpp_files = main.cpp comshader.cpp shaderprogram.cpp camera.cpp box.cpp particle.cpp setup.cpp simulation.cpp interface.cpp
objects = $(cpp_files:.cpp=.o) glad.o 
headers =

all: $(TARGET)

$(TARGET): $(objects) 
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY : clean
clean :
	rm $(TARGET) $(objects)


