// SpriteLight - Heavily simplified sprite engine
// by Ingemar Ragnemalm 2009

// What does a mogwai say when it sees a can of soda?
// Eeek! Sprite light! Sprite light!

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
#else
	#include <GL/gl.h>
	#include <GL/freeglut.h>
#endif

#include "../common/LoadTGA.h"

enum AI_TYPE {
	SHEEP,
	BLACKSHEEP,
	DOG,
	FOOD
} AI_TYPE;

typedef struct FPoint
{
	GLfloat h, v;
} FPoint;

typedef struct SpriteRec
{
	FPoint position;
	TextureData *face;
	FPoint speed;
	GLfloat rotation;
	struct SpriteRec *next;

	float randomness;
	enum AI_TYPE ai_type;
	
	// Add custom sprite data here as needed
} SpriteRec, *SpritePtr;

// Globals: The sprite list, background texture and viewport dimensions (virtual or real pixels)
extern SpritePtr gSpriteRoot;
extern GLuint backgroundTexID;
extern long gWidth, gHeight;

// Functions
TextureData *GetFace(char *fileName);
struct SpriteRec *NewSprite(TextureData *f, GLfloat h, GLfloat v, GLfloat hs, GLfloat vs, enum AI_TYPE ai_type);
void HandleSprite(SpritePtr sp);
void DrawSprite(SpritePtr sp);
void DrawBackground();

void InitSpriteLight();
