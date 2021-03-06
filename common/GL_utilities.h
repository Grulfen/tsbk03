#ifndef _GL_UTILITIES_
#define _GL_UTILITIES_

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/freeglut.h>
#include <GL/gl.h>

void printError(const char *functionName);
GLuint loadShaders(const char *vertFileName, const char *fragFileName);
GLuint loadShadersG(const char *vertFileName, const char *fragFileName, const char *geomFileName);
void dumpInfo(void);

void initKeymapManager();
char keyIsDown(unsigned char c);

// FBO support

//------------a structure for FBO information-------------------------------------
typedef struct
{
	GLuint texid;
	GLuint fb;
	GLuint rb;
	GLuint depth;
	int width, height;
} FBOstruct;

FBOstruct *initFBO(int width, int height, int int_method);
FBOstruct *initFBO2(int width, int height, int int_method, int create_depthimage);
void useFBO(FBOstruct *out, FBOstruct *in1, FBOstruct *in2);
void updateScreenSizeForFBOHandler(int w, int h); // Temporary workaround to inform useFBO of screen size changes

#ifdef __cplusplus
}
#endif

#endif
