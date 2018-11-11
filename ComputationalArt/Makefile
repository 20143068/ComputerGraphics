HEADERS = vec.hpp mat.hpp operator.hpp transform.hpp hilbert.hpp
SOURCES = main.cpp
CC = g++
CFLAGS = -std=c++11 -lGL -lglut -lGLEW
EXECUTABLE = Transform
RM = rm -rf

all: $(SOURCES) $(HEADERS)
	$(CC) -o $(EXECUTABLE) $(SOURCES) $(CFLAGS)

clean:
	$(RM) *.o $(EXECUTABLE)
