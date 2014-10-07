# set this variable to the director in which you saved the common files
commondir = ../common/

FLAGS = -Wall -Wextra -Wno-unused-parameter -lGL -lglut -DGL_GLEXT_PROTOTYPES -std=c99

all : lab3

lab3 : lab3.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)loadobj.c $(commondir)LoadTGA.c $(commondir)zpr.c
	gcc -Wall -o lab3 -I$(commondir) $(FLAGS) lab3.c $(commondir)GL_utilities.c $(commondir)loadobj.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)zpr.c

clean :
	rm lab3

