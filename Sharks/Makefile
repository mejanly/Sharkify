CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS= -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= Main.o Matrix.o Vector.o Quad.o glQuaternion.o Texture.o tiny_obj_loader.o Shark.o SharkMesh.o SharkVertex.o SharkBone.o SharkSkeleton.o
LIBFLAGS= -lm -lGL -lGLEW -lGLU -lglut -lglfw3 -lXmu -lX11 -lXxf86vm -lXrandr -lpthread -lXi libGLEW.a libglfw3.a
 
shark: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) -o shark $(LIBFLAGS)

Main.o: Main.cpp includes.h
	$(CC) $(CFLAGS) -c Main.cpp

Quad.o: Quad.cpp Quad.h
	$(CC) $(CFLAGS) -c Quad.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(CFLAGS) -c Matrix.cpp

Vector.o: Vector.cpp Vector.h
	$(CC) $(CFLAGS) -c Vector.cpp

tiny_obj_loader.o: tiny_obj_loader.cc tiny_obj_loader.h
	$(CC) $(CFLAGS) -c tiny_obj_loader.cc

glQuaternion.o: glQuaternion.cpp glQuaternion.h
	$(CC) $(CFLAGS) -c glQuaternion.cpp

Texture.o: Texture.cpp Texture.h
	$(CC) $(CFLAGS) -c Texture.cpp

Shark.o: Shark.cpp Shark.h
	$(CC) $(CFLAGS) -c Shark.cpp

SharkMesh.o: SharkMesh.cpp SharkMesh.h
	$(CC) $(CFLAGS) -c SharkMesh.cpp

SharkVertex.o: SharkVertex.cpp SharkVertex.h
	$(CC) $(CFLAGS) -c SharkVertex.cpp

SharkSkeleton.o: SharkSkeleton.cpp SharkSkeleton.h
	$(CC) $(CFLAGS) -c SharkSkeleton.cpp

SharkBone.o: SharkBone.cpp SharkBone.h
	$(CC) $(CFLAGS) -c SharkBone.cpp
