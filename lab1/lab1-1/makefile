all: lab1-1

lab1-1: lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c
	gcc -Wall -std=c99 -o lab1-1 -lm -lglut -lGL -lX11 -DGL_GLEXT_PROTOTYPES lab1-1.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c -I../common

clean:
	rm lab1-1
