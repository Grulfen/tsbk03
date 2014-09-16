all: lab1-2 lab1-2a

lab1-2: lab1-2.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c
	gcc -Wall -std=c99 -o lab1-2 -lm -lglut -lGL -lX11 -DGL_GLEXT_PROTOTYPES lab1-2.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c -I../common

lab1-2a: lab1-2a.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c
	gcc -Wall -std=c99 -o lab1-2a -lm -lglut -lGL -lX11 -DGL_GLEXT_PROTOTYPES lab1-2a.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c -I../common

clean:
	rm lab1-2 lab1-2a
