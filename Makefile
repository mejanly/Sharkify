CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS= -I $(ROOT_DIR) -I$(FREETYPE_DIR) -L $(ROOT_DIR) -g -std=c++0x
SOURCES= Main.o tiny_obj_loader.o Shape.o GLSL.o RenderingHelper.o Program.o FontEngine.o FontAtlas.o
LIBFLAGS= -lm -lGL -lGLEW -lGLU -lglut -lfreetype -lglfw3 -lXmu -lX11 -lXxf86vm -lXrandr -lpthread -lXi libGLEW.a libglfw3.a

clean:
	rm -f $(SOURCES) *~ TAGS

shark: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) -o shark $(LIBFLAGS)

Main.o: Main.cpp Includes.h
	$(CC) $(CFLAGS) -c Main.cpp

ParticleTexture.o: ParticleTexture.cpp ParticleTexture.h
	$(CC) $(CFLAGS) -c ParticleTexture.cpp

FontAtlas.o: FontAtlas.cpp FontAtlas.h
	$(CC) $(CFLAGS) -c FontAtlas.cpp

FontEngine.o: FontEngine.cpp FontEngine.h
	$(CC) $(CFLAGS) -c FontEngine.cpp

tiny_obj_loader.o: tiny_obj_loader.cc tiny_obj_loader.h
	$(CC) $(CFLAGS) -c tiny_obj_loader.cc

Shape.o: Shape.cpp Shape.h Vars.h
	$(CC) $(CFLAGS) -c Shape.cpp

GLSL.o: GLSL.cpp GLSL.h GLIncludes.h
	$(CC) $(CFLAGS) -c GLSL.cpp

RenderingHelper.o: RenderingHelper.cpp RenderingHelper.h
	$(CC) $(CFLAGS) -c RenderingHelper.cpp
