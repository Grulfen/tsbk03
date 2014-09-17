// Lab 1-1, multi-pass rendering with FBOs and HDR.
// Revision for 2013, simpler light, start with rendering on quad in final stage.
// Switched to low-res Stanford Bunny for more details.
// No HDR is implemented to begin with. That is your task.

// You can compile like this:
// gcc lab1-1.c ../common/*.c -lGL -o lab1-1 -I../common

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
// Mac
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#ifdef WIN32
// MS
		#include <windows.h>
		#include <stdio.h>
		#include <GL/glew.h>
		#include <GL/glut.h>
	#else
// Linux
		#include <stdio.h>
		#include <GL/gl.h>
		#include <GL/freeglut.h>
	#endif
#endif

#include "../common/VectorUtils3.h"
#include "../common/GL_utilities.h"
#include "../common/loadobj.h"
#include "../common/zpr.h"

// initial width and heights
#define W 512
#define H 512

#define NUM_LIGHTS 4

#define FILTER_PASSES 20
#define FILTER_DIAG_PASSES 3
#define DIAGONAL_PASSES 10

void OnTimer(int value);

mat4 projectionMatrix;
mat4 viewMatrix;


GLfloat square[] = {
    -1,-1, 0,
    -1, 1, 0,
     1, 1, 0,
     1,-1, 0};
GLfloat squareTexCoord[] = {
    0, 0,
    0, 1,
    1, 1,
    1, 0};
GLuint squareIndices[] = {0, 1, 2, 0, 2, 3};

Model* squareModel;

//----------------------Globals-------------------------------------------------
Point3D cam, point;
Model *model1;
FBOstruct *fbo1, *fbo2, *fbo3, *fbo4, *fbo_orig;
GLuint phongshader = 0, plaintextureshader = 0, lowpasshader = 0,
       thresholdshader = 0, bloomshader = 0, diagonalshader1 = 0, diagonalshader2 = 0, averageshader = 0;
GLfloat t = 0;

//-------------------------------------------------------------------------------------

void init(void)
{
	dumpInfo();  // shader info

	// GL inits
	glClearColor(0.1, 0.1, 0.3, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	printError("GL inits");

	// Load and compile shaders
	plaintextureshader = loadShaders("plaintextureshader.vert", "plaintextureshader.frag");  // puts texture on teapot
	phongshader = loadShaders("phong.vert", "phong.frag");  // renders with light (used for initial renderin of teapot)
	lowpasshader = loadShaders("lowpass.vert", "lowpass.frag");  // filters the values in texture and outputs
	thresholdshader = loadShaders("threshold.vert", "threshold.frag");
	bloomshader = loadShaders("bloom.vert", "bloom.frag");
	diagonalshader1 = loadShaders("diagonal.vert", "diagonal1.frag");
	diagonalshader2 = loadShaders("diagonal.vert", "diagonal2.frag");
	averageshader = loadShaders("add.vert", "add.frag");

	printError("init shader");

	fbo1 = initFBO(W, H, 0);
	fbo2 = initFBO(W, H, 0);
	fbo3 = initFBO(W, H, 0);
	fbo4 = initFBO(W, H, 0);
	fbo_orig = initFBO(W, H, 0);

	// load the model
//	model1 = LoadModelPlus("teapot.obj");
	model1 = LoadModelPlus("objects/stanford-bunny.obj");

	squareModel = LoadDataToModel(
			square, NULL, squareTexCoord, NULL,
			squareIndices, 4, 6);

	cam = SetVector(0, 5, 15);
	point = SetVector(0, 1, 0);

	glutTimerFunc(5, &OnTimer, 0);

	zprInit(&viewMatrix, cam, point);
}

void OnTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(5, &OnTimer, value);
}

//-------------------------------callback functions------------------------------------------
void display(void)
{
	mat4 vm2;
	
	// This function is called whenever it is time to render
	//  a new frame; due to the idle()-function below, this
	//  function will get called several times per second

	// render to fbo1!
	useFBO(fbo_orig, 0L, 0L);

	// Clear framebuffer & zbuffer
	glClearColor(0.1, 0.1, 0.3, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Activate shader program
	glUseProgram(phongshader);

	vm2 = viewMatrix;
	// Scale and place bunny since it is too small
	vm2 = Mult(vm2, T(0, -8.5, 0));
	vm2 = Mult(vm2, S(80,80,80));

	// Increase "time"
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);


	glUniformMatrix4fv(glGetUniformLocation(phongshader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUniformMatrix4fv(glGetUniformLocation(phongshader, "modelviewMatrix"), 1, GL_TRUE, vm2.m);
	glUniform3fv(glGetUniformLocation(phongshader, "camPos"), 1, &cam.x);
	glUniform1i(glGetUniformLocation(phongshader, "texUnit"), 0);

	// Enable Z-buffering
	glEnable(GL_DEPTH_TEST);
	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	DrawModel(model1, phongshader, "in_Position", "in_Normal", NULL);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	// THRESHOLD
	
	glUseProgram(thresholdshader);
	useFBO(fbo2, fbo_orig, 0L);
	DrawModel(squareModel, thresholdshader, "in_Position", NULL, "in_TexCoord");

	// Diagonalizing 1
	
	glUseProgram(diagonalshader1);
	useFBO(fbo3, fbo2, 0L);
	DrawModel(squareModel, diagonalshader1, "in_Position", NULL, "in_TexCoord");
	for(int i=0; i<DIAGONAL_PASSES;i++){
	  useFBO(fbo1, fbo3, 0L);
	  DrawModel(squareModel, diagonalshader1, "in_Position", NULL, "in_TexCoord");
	  useFBO(fbo3, fbo1, 0L);
	  DrawModel(squareModel, diagonalshader1, "in_Position", NULL, "in_TexCoord");
	}

	// Diagonalizing 2
	
	glUseProgram(diagonalshader2);
	useFBO(fbo4, fbo2, 0L);
	DrawModel(squareModel, diagonalshader2, "in_Position", NULL, "in_TexCoord");
	for(int i=0; i<DIAGONAL_PASSES;i++){
	  useFBO(fbo1, fbo4, 0L);
	  DrawModel(squareModel, diagonalshader2, "in_Position", NULL, "in_TexCoord");
	  useFBO(fbo4, fbo1, 0L);
	  DrawModel(squareModel, diagonalshader2, "in_Position", NULL, "in_TexCoord");
	}

	// FILTERING

	glUseProgram(lowpasshader);

	// Use CLAMP_TO_EDGE to prevent blooming to bleed over edges
	glBindTexture(GL_TEXTURE_2D, fbo1->texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, fbo2->texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Restore bound texture
	glBindTexture(GL_TEXTURE_2D, fbo_orig->texid);

	for(int i=0; i<FILTER_PASSES;i++){
	    useFBO(fbo1, fbo2, 0L);
	    DrawModel(squareModel, lowpasshader, "in_Position", NULL, "in_TexCoord");

	    useFBO(fbo2, fbo1, 0L);
	    DrawModel(squareModel, lowpasshader, "in_Position", NULL, "in_TexCoord");
	}

	// ADD DIAGONALS
	
	glUseProgram(averageshader);
	glUniform1i(glGetUniformLocation(bloomshader, "texUnit2"), 1);

	useFBO(fbo1, fbo3, fbo4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawModel(squareModel, averageshader, "in_Position", NULL, "in_TexCoord");

	// FILTER DIAGONALS
	//
	for(int i=0; i<FILTER_DIAG_PASSES;i++){
	    useFBO(fbo3, fbo1, 0L);
	    DrawModel(squareModel, lowpasshader, "in_Position", NULL, "in_TexCoord");

	    useFBO(fbo1, fbo3, 0L);
	    DrawModel(squareModel, lowpasshader, "in_Position", NULL, "in_TexCoord");
	}

	// ADDING
	
	glUseProgram(bloomshader);
	glUniform1i(glGetUniformLocation(bloomshader, "texUnit2"), 1);

	useFBO(fbo4, fbo2, fbo_orig);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawModel(squareModel, bloomshader, "in_Position", NULL, "in_TexCoord");
	useFBO(fbo3, fbo1, fbo4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawModel(squareModel, bloomshader, "in_Position", NULL, "in_TexCoord");
	
	// Activate second shader program
	useFBO(0L, fbo3, 0L);
	glUseProgram(plaintextureshader);

	DrawModel(squareModel, plaintextureshader, "in_Position", NULL, "in_TexCoord");

	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	GLfloat ratio = (GLfloat) w / (GLfloat) h;
	projectionMatrix = perspective(90, ratio, 1.0, 1000);
}


// This function is called whenever the computer is idle
// As soon as the machine is idle, ask GLUT to trigger rendering of a new
// frame
void idle()
{
  glutPostRedisplay();
}

//-----------------------------main-----------------------------------------------
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(W, H);

	glutInitContextVersion(3, 2);
	glutCreateWindow ("Render to texture with FBO");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	initKeymapManager();

	init();
	glutMainLoop();
}
