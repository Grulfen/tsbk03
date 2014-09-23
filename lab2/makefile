# set this variable to the director in which you saved the common files
commondir = ../common/

FLAGS = -Wall -Wextra -Wno-unused-parameter -lGL -lglut -DGL_GLEXT_PROTOTYPES -std=c99

all : lab2-1 lab2-2

lab2-1 : skinning.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c 
	gcc $(FLAGS) -o skinning -I$(commondir) skinning.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c

lab2-2 : skinning2.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c
	gcc $(FLAGS) -o skinning2 -I$(commondir) skinning2.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c

clean :
	rm skinning skinning2
