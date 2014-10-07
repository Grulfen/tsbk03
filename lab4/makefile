# set this variable to the director in which you saved the common files
commondir = ../common/

FLAGS = -Wall -Wextra -Wno-unused-parameter -lGL -lglut -DGL_GLEXT_PROTOTYPES -std=c99

all: $(commondir)LoadTGA.c SpriteLight.c SpriteLightDemo.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c
	gcc -o SpriteLightDemo $(FLAGS) -I$(commondir) $(commondir)LoadTGA.c $(commondir)VectorUtils3.c $(commondir)GL_utilities.c SpriteLight.c SpriteLightDemo.c -DGL_GLEXT_PROTOTYPES

clean:
	rm -f SpriteLightDemo

