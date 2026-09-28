CXX = g++
CC = gcc

CXXFLAGS = -Iinclude -I/opt/homebrew/include
CFLAGS = -Iinclude
LDFLAGS = -L/opt/homebrew/lib -lglfw -framework OpenGL

TARGET = triangulo

all: $(TARGET)

$(TARGET): main.o glad.o
	$(CXX) main.o glad.o -o $(TARGET) $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

glad.o: src/glad.c
	$(CC) $(CFLAGS) -c src/glad.c -o glad.o

clean:
	rm -f main.o glad.o $(TARGET)
