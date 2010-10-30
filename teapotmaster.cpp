#include <iostream>

//OpenGL libraries
#ifdef __APPLE__
	#include <SDL/SDL.h>
	#include <GLEW/GLEW.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
	#include <GLUT/GLUT.h>
#else
	#include <Windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <GL/glu.h>	
	#include <SDL.h>
#endif

using namespace std;

int width;
int height;

static void resize(int, int);

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
static void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
	glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

	
	resize(Width, Height);
}

static void resize(int newWidth, int newHeight)
{
	
	#ifdef __APPLE__
	SDL_SetVideoMode(newWidth, newHeight, 32, SDL_OPENGL | SDL_RESIZABLE);
	#endif
	
	glViewport(0, 0, newWidth, newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	gluPerspective(45.0f,(GLfloat)newWidth/(GLfloat)newHeight,0.1f,100.0f);
}

/* The main drawing function. */
static void DrawGLScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();				// Reset The View

	glTranslatef(-1.5f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
	
	// draw a triangle
	glBegin(GL_POLYGON);				// start drawing a polygon
	glVertex3f( 0.0f, 1.0f, 0.0f);		// Top
	glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
	glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left	
	glEnd();					// we're done with the polygon

	glTranslatef(3.0f,0.0f,0.0f);		        // Move Right 3 Units
	
	// draw a square (quadrilateral)
	glBegin(GL_QUADS);				// start drawing a polygon (4 sided)
	glVertex3f(-1.0f, 1.0f, 0.0f);		// Top Left
	glVertex3f( 1.0f, 1.0f, 0.0f);		// Top Right
	glVertex3f( 1.0f,-1.0f, 0.0f);		// Bottom Right
	glVertex3f(-1.0f,-1.0f, 0.0f);		// Bottom Left	
	glEnd();					// done with the polygon

	// swap buffers to display, since we're double buffered.
	SDL_GL_SwapBuffers();
}



int main(int argc, char* argv[])
{
	cout << "Initializing Engine" << endl;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "Could not initialize SDL" << endl << SDL_GetError();
		exit(2);
	}

	//Set window
	SDL_WM_SetCaption("Hello World!", "Hello World!");

	//Set openGL window
	if ( SDL_SetVideoMode(640, 480, 32, SDL_OPENGL | SDL_RESIZABLE) == NULL ) {
		cout << "Unable to create OpenGL screen: %s\n" << endl << SDL_GetError();
		SDL_Quit();
		exit(2);
	}

	//Set up event handling
	SDL_Event event;
	bool quit = false;

	//Set up OpenGL
	InitGL(640, 480);

	//Main while loop
	while (!quit)
	{
		//Event handling
		if (SDL_PollEvent(&event))
		{
			//Keys
			switch (event.type)
			{
			//Quit
			case SDL_QUIT:
				quit = true;
				break;

			//Key down
			case  SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					quit = true;
					break;
				}

			//Resize
			case SDL_VIDEORESIZE:
				resize(event.resize.w, event.resize.h);
				break;

			}

		}

		//Updates
		DrawGLScene();
	}

	SDL_Quit();
	return 0;

}

