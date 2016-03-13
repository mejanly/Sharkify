export IRRKLANG_INC_x64 := irrKlang-64-bit-1.5.0/include
export IRRKLANG_LIB_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/libIrrKlang.so
export IRRKLANG_MP3_U_x64 := irrKlang-64bit-1.5.0/bin/linux-gcc-64/ikpMP3.so

CC=g++
ROOT_DIR= $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CFLAGS= -I $(ROOT_DIR) -I$(FREETYPE_DIR) -I$(IRRKLANG_INC_x64) -L $(ROOT_DIR) -g -std=c++0x
SOURCES= Main.o tiny_obj_loader.o Shape.o GLSL.o RenderingHelper.o Program.o FontEngine.o FontAtlas.o Sound.o Shark.o Particle.o ParticleTexture.o MatrixStack.o ParticleSystem.o
LIBFLAGS= -lm -lGL -lGLEW -lGLU -lglut -lfreetype -lglfw3 -lXmu -lX11 -lXxf86vm -lXrandr -lpthread -lXi $(IRRKLANG_LIB_U_x64) $(IRRKLANG_MP3_U_x64) libGLEW.a libglfw3.a

clean:
	rm -f $(SOURCES) *~ TAGS

shark: $(SOURCES)
	cp libglfw3_x64.a libglfw3.a
	cp libGLEW_x64.a libGLEW.a
	$(CC) $(CFLAGS) $(SOURCES) -o shark $(LIBFLAGS)

Main.o: Main.cpp Includes.h
	$(CC) $(CFLAGS) -c Main.cpp

Shark.o: Shark.cpp Shark.h
	$(CC) $(CFLAGS) -c Shark.cpp

Sound.o: Sound.cpp Sound.h
	$(CC) $(CFLAGS) -c Sound.cpp

ParticleTexture.o: ParticleTexture.cpp ParticleTexture.h
	$(CC) $(CFLAGS) -c ParticleTexture.cpp

FontAtlas.o: FontAtlas.cpp FontAtlas.h
	$(CC) $(CFLAGS) -c FontAtlas.cpp

FontEngine.o: FontEngine.cpp FontEngine.h
	$(CC) $(CFLAGS) -c FontEngine.cpp

tiny_obj_loader.o: tiny_obj_loader.cc tiny_obj_loader.h
	$(CC) $(CFLAGS) -c tiny_obj_loader.cc

Shape.o: Shape.cpp Shape.h
	$(CC) $(CFLAGS) -c Shape.cpp

GLSL.o: GLSL.cpp GLSL.h GLIncludes.h
	$(CC) $(CFLAGS) -c GLSL.cpp

RenderingHelper.o: RenderingHelper.cpp RenderingHelper.h
	$(CC) $(CFLAGS) -c RenderingHelper.cpp

Particle.o: Particle.cpp Particle.h
	$(CC) $(CFLAGS) -c Particle.cpp

ParticleTexture.o: ParticleTexture.cpp ParticleTexture.h
	$(CC) $(CFLAGS) -c ParticleTexture.cpp ParticleTexture.h

MatrixStack.o: MatrixStack.cpp MatrixStack.h
	$(CC) $(CFLAGS) -c MatrixStack.cpp

ParticleSystem.o: ParticleSystem.cpp ParticleSystem.h
	$(CC) $(CFLAGS) -c ParticleSystem.cpp
