CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS= -I $(ROOT_DIR) -L $(ROOT_DIR) -g
SOURCES= Main.o GLSL.o tiny_obj_loader.o RenderingHelper.o SharkMesh.o SharkBone.o SharkSkeleton.o
LIBFLAGS= -lm -lGL -lGLEW -lGLU -lglut -lglfw3 -lXmu -lX11 -lXxf86vm -lXrandr -lpthread -lXi libGLEW.a libglfw3.a
 
shark: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) -o shark $(LIBFLAGS)

Main.o: Main.cpp includes.h
	$(CC) $(CFLAGS) -c Main.cpp

GLSL.o: GLSL.cpp GLSL.h GLIncludes.h
	$(CC) $(CFLAGS) -c GLSL.cpp

RenderingHelper.o: RenderingHelper.cpp RenderingHelper.h
	$(CC) $(CFLAGS) -c RenderingHelper.cpp

tiny_obj_loader.o: tiny_obj_loader.cc tiny_obj_loader.h
	$(CC) $(CFLAGS) -c tiny_obj_loader.cc

SharkMesh.o: SharkMesh.cpp SharkMesh.h
	$(CC) $(CFLAGS) -c SharkMesh.cpp

SharkSkeleton.o: SharkSkeleton.cpp SharkSkeleton.h
	$(CC) $(CFLAGS) -c SharkSkeleton.cpp

SharkBone.o: SharkBone.cpp SharkBone.h
	$(CC) $(CFLAGS) -c SharkBone.cpp
