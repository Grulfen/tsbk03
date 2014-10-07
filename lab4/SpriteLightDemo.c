// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include "MicroGlut.h"
// uses framework Cocoa
#else
#include <GL/gl.h>
#include <GL/freeglut.h> 
#endif

#include <math.h>
#include <stdlib.h>
#include "../common/LoadTGA.h"
#include "SpriteLight.h"
#include "../common/GL_utilities.h"

GLfloat max(GLfloat a, GLfloat b)
{
    return a > b ? a : b;
}

// Lägg till egna globaler här efter behov.
float maxDist = 200;
float minDist = 70;
float maxSpeed = 2;
float cohesionCoeff = 0.01;
float separationCoeff = 0.15;
float alignmentCoeff = 0.02;

SpriteRec* dog;

float distance(FPoint a, FPoint b)
{
    return sqrt(pow(a.h-b.h,2) + pow(a.v - b.v,2));
}

float normFPoint(FPoint a)
{
    return sqrt(pow(a.h,2)+pow(a.v,2));
}

FPoint normalizeFPoint(FPoint a)
{
    float norm = normFPoint(a);
    FPoint normalized = {a.h/norm, a.v/norm};
    return normalized;
}

void SpriteBehavior() // Din kod!
{
    SpritePtr i = gSpriteRoot; 
    SpritePtr j = i->next;
    
    int count = 0;
    float dPos;


    FPoint black_hole = {0,0};
    FPoint alignmentVel = {0,0};

    while(i != NULL){
	if(i->ai_type == DOG){
	    i = i->next;
	    continue;
	}

	count = 0;
	while(j != NULL){
	    
	    dPos = distance(i->position, j->position);

	    if(dPos < maxDist){
		count++;

		// Cohesion
		if(i->ai_type == SHEEP || j->ai_type != DOG){
		    black_hole.h += j->position.h;
		    black_hole.v += j->position.v;
		}

		if(j->ai_type != DOG){
		    // Alignment
		    alignmentVel.h += j->speed.h;
		    alignmentVel.v += j->speed.v;
		}
	    }

	    // Separation
	    if(dPos < minDist && (i != j)){
		i->speed.h += 1/(dPos + 0.1)*separationCoeff*(i->position.h - j->position.h);
		i->speed.v += 1/(dPos + 0.1)*separationCoeff*(i->position.v - j->position.v);
	    }

	    if(j->ai_type == DOG && dPos < maxDist){
		i->speed.h += 1/(dPos + 0.1)*10*separationCoeff*(i->position.h - j->position.h);
		i->speed.v += 1/(dPos + 0.1)*10*separationCoeff*(i->position.v - j->position.v);
	    }

	    j = j->next;
	}

	black_hole.h /= count;
	black_hole.v /= count;

	alignmentVel.h /= count;
	alignmentVel.v /= count;

	if(i->ai_type == SHEEP){
	    i->speed.h += cohesionCoeff*(black_hole.h - i->position.h);
	    i->speed.v += cohesionCoeff*(black_hole.v - i->position.v);

	    i->speed.h += alignmentCoeff*alignmentVel.h;
	    i->speed.v += alignmentCoeff*alignmentVel.v;
	}

	i->speed.h += i->randomness*((float)random()/RAND_MAX - 0.5);
	i->speed.v += i->randomness*((float)random()/RAND_MAX - 0.5);
	
	if(normFPoint(i->speed) > maxSpeed) {
	    i->speed.h = maxSpeed*normalizeFPoint(i->speed).h;
	    i->speed.v = maxSpeed*normalizeFPoint(i->speed).v;
	}

	i = i->next;
	j = gSpriteRoot;

	black_hole.h = 0;
	black_hole.v = 0;

	alignmentVel.h = 0;
	alignmentVel.v = 0;
    }
}

// Drawing routine
void Display()
{
    SpritePtr sp;

    glClearColor(0, 0, 0.2, 1);
    glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DrawBackground();

    SpriteBehavior(); // Din kod!

    // Loop though all sprites. (Several loops in real engine.)
    sp = gSpriteRoot;
    do {
	HandleSprite(sp); // Callback in a real engine
	DrawSprite(sp);
	sp = sp->next;
    } while (sp != NULL);

    glutSwapBuffers();
}

void Reshape(int h, int v)
{
    glViewport(0, 0, h, v);
    gWidth = h;
    gHeight = v;
}

void Timer(int value)
{
    glutTimerFunc(20, Timer, 0);
    glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
	__attribute__((unused)) int x,
	__attribute__((unused)) int y)
{
    switch (key)
    {
	case '+':
	    cohesionCoeff += 0.001;
	    printf("cohesionCoeff = %f\n", cohesionCoeff);
	    break;
	case '-':
	    cohesionCoeff -= 0.001;
	    printf("cohesionCoeff = %f\n", cohesionCoeff);
	    break;
	case 'q':
	    maxDist += 10;
	    printf("maxDist = %f\n", maxDist);
	    break;
	case 'a':
	    maxDist -= 10;
	    printf("maxDist = %f\n", maxDist);
	    break;
	case 'w':
	    maxSpeed += 10;
	    printf("maxSpeed = %f\n", maxSpeed);
	    break;
	case 's':
	    maxSpeed -= 10;
	    printf("maxSpeed = %f\n", maxSpeed);
	    break;
	case 'e':
	    minDist += 5;
	    printf("minDist = %f\n", minDist);
	    break;
	case 'd':
	    minDist -= 5;
	    printf("minDist = %f\n", minDist);
	    break;
	case 'r':
	    separationCoeff += 0.01;
	    printf("separationCoeff = %f\n", separationCoeff);
	    break;
	case 'f':
	    separationCoeff -= 0.01;
	    printf("separationCoeff = %f\n", separationCoeff);
	    break;
	case 'i':
	    dog->position.v += 10;
	    break;
	case 'k':
	    dog->position.v -= 10;
	    break;
	case 'j':
	    dog->position.h -= 10;
	    break;
	case 'l':
	    dog->position.h += 10;
	    break;


	case 0x1b:
	    exit(0);
    }
}

void Init()
{
    TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

    LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

    sheepFace = GetFace("bilder/sheep.tga"); // Ett får
    blackFace = GetFace("bilder/blackie.tga"); // Ett svart får
    dogFace = GetFace("bilder/dog.tga"); // En hund
    foodFace = GetFace("bilder/mat.tga"); // Mat

    NewSprite(sheepFace, 100, 200, 1, 1, SHEEP);
    NewSprite(sheepFace, 200, 100, 1.5, -1, SHEEP);
    NewSprite(sheepFace, 250, 200, -1, 1.5, SHEEP);
    NewSprite(sheepFace, 125, 200, 0.4, 1, SHEEP);

    NewSprite(sheepFace, 450, 50, -0.4, 1, SHEEP);
    NewSprite(sheepFace, 500, 50, 0.4, -1, SHEEP);

    NewSprite(blackFace, 320, 300, -0.2, -1, BLACKSHEEP);
    dog = NewSprite(dogFace, 320, 300, 0, 0, DOG);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(800, 600);
    glutInitContextVersion(3, 2);
    glutCreateWindow("SpriteLight demo / Flocking");

    glutDisplayFunc(Display);
    glutTimerFunc(20, Timer, 0); // Should match the screen synch
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Key);

    InitSpriteLight();
    Init();

    glutMainLoop();
    return 0;
}
