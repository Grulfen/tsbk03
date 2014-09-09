// Converted to MicroGlut and VectorUtils3 2013.
// MicroGLut currently exists for Linux and Mac OS X. On other platforms (e.g. MS Windows)
// you should use FreeGLUT. You will also need GLEW. You may sometimes need to work around
// differences, e.g. additions in MicroGlut that don't exist in FreeGlut.

// gcc lab0.c ../common/*.c -lGL -o lab0 -I../common

#include <GL/gl.h>
#include <GL/freeglut.h>
#include "../common/GL_utilities.h"
#include "../common/VectorUtils3.h"
#include "../common/loadobj.h"
#include "../common/zpr.h"
#include "../common/LoadTGA.h"

//constants
const int initWidth=512,initHeight=512;

// Modify this matrix.
// See below for how it is applied to your model.
mat4 bunnyMatrix = {{ 1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 1.0, -1.0,
    0.0, 0.0, 0.0, 1.0}};

mat4 teddyMatrix = {{ 0.3, 0.0, 0.0, 2.0,
    0.0, 0.3, 0.0, 0.0,
    0.0, 0.0, 0.3, -4.0,
    0.0, 0.0, 0.0, 1.0}};

mat4 rotationMatrix;

mat4 viewMatrix, projectionMatrix;

// Globals

// Model(s)
Model *bunny, *teddy;

// Reference(s) to shader program(s)
GLuint program;

// Texture(s)
GLuint maskros, grass;

// Time
GLfloat t = 0;

void init(void)
{

    dumpInfo();

    // GL inits
    glClearColor(0.2,0.2,0.5,0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    printError("GL inits");

    projectionMatrix = perspective(90, 1.0, 0.1, 1000);

    // Load and compile shader
    program = loadShaders("lab0.vert", "lab0.frag");
    printError("init shader");

    // Upload geometry to the GPU:
    bunny = LoadModelPlus("objects/bunnyplus.obj");
    teddy = LoadModelPlus("objects/teapot.obj");
    printError("load models");

    // Load textures
    LoadTGATextureSimple("textures/maskros512.tga",&maskros);
    LoadTGATextureSimple("textures/grass.tga",&grass);
    printError("load textures");
}


void display(void)
{

    t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
    printError("pre display");

    // clear the screen
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    rotationMatrix = Ry(0.001*t);

    //activate the program, and set its variables
    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "t"), t);
    glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
    glUniformMatrix4fv(glGetUniformLocation(program, "rotMatrix"), 1, GL_TRUE, rotationMatrix.m);

    // Upload texture
    glUniform1i(glGetUniformLocation(program, "tex1"), 0);
    glActiveTexture(GL_TEXTURE0);

    //draw the model
    glBindTexture(GL_TEXTURE_2D, maskros);
    mat4 m = Mult(viewMatrix, bunnyMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
    DrawModel(bunny, program, "in_Position", "in_Normal", "in_TexCoord");

    glBindTexture(GL_TEXTURE_2D, grass);
    m = Mult(viewMatrix, teddyMatrix);
    glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_TRUE, m.m);
    DrawModel(teddy, program, "in_Position", "in_Normal", "in_TexCoord");

    printError("display");

    glutSwapBuffers();
}

void idle()
{
    // This function is called whenever the computer is idle
    // As soon as the machine is idle, ask GLUT to trigger rendering of a new frame
    glutPostRedisplay();
}

vec3 cam = {0,0,2}, point = {0,0,0};

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
    glutInitContextVersion(3, 2);
    glutCreateWindow ("Lab 0 - OpenGL 3.0 Introduction");
    zprInit(&viewMatrix, cam, point);
    glutDisplayFunc(display); 
    glutIdleFunc(idle);
    init ();
    glutMainLoop();

    return 0;
}
