all :  lab0

lab0: lab0.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c
	gcc -Wall -o lab0 -lm -lglut -lGL -lX11 -DGL_GLEXT_PROTOTYPES lab0.c ../common/GL_utilities.c ../common/VectorUtils3.c ../common/LoadTGA.c ../common/loadobj.c ../common/zpr.c -I../common

clean :
	rm lab0
