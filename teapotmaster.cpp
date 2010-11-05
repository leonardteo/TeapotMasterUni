/**
Teapot Master
Leonard Teo 
**/

//STD libraries
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

//OpenGL libraries
#ifdef __APPLE__
#include <GLEW/GLEW.h>
#include <GLUT/GLUT.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

//Engine libraries
#include "OBJModel.h"
#include "MyBitmap.h"
#include "Node.h"
#include "TransformNode.h"
#include "PolyMeshNode.h"
#include "Texture.h"
#include "Vector3.h"
#include "CameraNode.h"
#include "SceneGraph.h"
#include "LightNode.h"
#include "Grid.h"

using namespace std;

//Default Screen Size
const int WIDTH = 640;
const int HEIGHT = 480;

/**
 Global Variables
 **/

//Temporary screen width and height
int width = WIDTH;
int height = HEIGHT;

//Scene Graph
SceneGraph* sceneGraph;
CameraNode* ballCam;

//Grid
Grid grid;

//Calculating fps
int frame = 0;
int currenttime = 0;
int timebase=0;
float fps=0.0f;
char s[30];

//Ball Stuff
Vector3* ballVelocity;
Vector3* upVector;

//Camera movements
float camSpringElevation = 0.0f;	//Between -90 and 90
float camSpringHeading = 0.0f;		//Between -90 and 90

//Useful Math constants
const float degreesToRadians = 0.0174532925f;
const float radiansToDegrees = 57.2957795f;
const float pi = 3.14159265f;

#define TIMERMSECS 33
#define PI 3.14159265f
#define RADIANSTODEGREES 57.2957795f
#define DEGREESTORADIANS 0.0174532925f

//Measuring time in milliseconds
int startTime;
int prevTime;


/**
 Write text to screen
 **/
void drawText(char* string)
{
	char* p;
	for (p=string; *p; p++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
	}
}

/**
 Calculate and display FPS
 **/
static void profiler()
{
	glDisable(GL_TEXTURE_2D);
	
	//Calculate FPS
	frame++;	//Increment number of frames
	currenttime = glutGet(GLUT_ELAPSED_TIME);	//Get current time
	
	//Time is current time, timebase is last time since we took a benchmark
	if ((currenttime - timebase) > 500) {	//Do this every x milliseconds
		
		float fps = 1000* (float)frame / (float)(currenttime-timebase);
		sprintf(s,"FPS: %4.2f", fps );
	 	
		timebase = currenttime;	//Set timebase to current time
		frame = 0;	//Reset number of frames
	}
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glWindowPos2i(5, 5);
	
	drawText(s);
	glEnable(GL_TEXTURE_2D);
	
}

/**
 Animation
 **/
static void animate(int value)
{
	
	// Set up the next timer tick (do this first)	
	glutTimerFunc(TIMERMSECS, animate, 0);
	
	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;
	
	
	//Decrement the camera spring coefficient
	if (camSpringHeading > 0.0f)
	{
		camSpringHeading--;
		if (camSpringHeading < 0.0f)
			camSpringHeading = 0.0f;
	} else {
		camSpringHeading++;
		if (camSpringHeading > 0.0f)
			camSpringHeading = 0.0f;
	}

	if (camSpringElevation > 0.0f)
	{
		camSpringElevation--;
		if (camSpringElevation < 0.0f)
			camSpringElevation = 0.0f;
	} else {
		camSpringElevation++;
		if (camSpringElevation > 0.0f)
			camSpringElevation = 0.0f;
	}
	
	
	
	cout << "Time: " << currTime << " Heading: " << camSpringHeading << endl;
	
	//Decrement the velocity of the ball
	
	glutPostRedisplay();
	
	prevTime = currTime;
	
}

/**
 Display function
 **/
static void display()
{
	
	//Set the projection
	ballCam->setProjection(width, height);
	
	//Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//Call scene graph camera
	ballCam->viewTransform();
	
	//Draw a grid for reference
	grid.display(1);
	
	sceneGraph->render();
		
	profiler();
	
	glutSwapBuffers();
	glFlush();
	
}


/**
 Reshape function
 **/
static void reshape(int new_width, int new_height)
{
	//Set Global Variables
	width = new_width;
	height = new_height;
	
	//Reset the viewport
	glViewport(0, 0, width, height);

	glutPostRedisplay();
}

/** 
 Keyboard Input
 **/
static void keyboard_func(unsigned char key, int mx, int my)
{
	switch (key)
	{
			
		case 'w':	//move ball forward
			break;
		case 's':	//move ball back
			break;
		case 'a':	//move ball left
			break;
		case 'd':	//move ball right
			break;
			
			
			//Exit
		case 27:
			exit(0);
	}
	
	glutPostRedisplay();
}

/**
 Function Keys
 **/
static void functionKeys(int key, int mx, int my)
{
	switch (key)
	{
		//Camera movement
		case GLUT_KEY_LEFT:
			camSpringHeading += 3.0f;
			if (camSpringHeading > 90.0f)
				camSpringHeading = 90.0f;
			
			ballCam->azimuth += 50*sin(camSpringHeading*degreesToRadians);
			break;
		case GLUT_KEY_RIGHT:
			ballCam->azimuth -= 5.0f;
			break;
		case GLUT_KEY_UP:
			ballCam->elevation += 5.0f;
			break;
		case GLUT_KEY_DOWN:
			ballCam->elevation -= 5.0f;
			break;
	}

	glutPostRedisplay();
}

/**
 Mouse Movement
 **/
static void mouse_move(int mx, int my)
{
	
	
}

/**
 Mouse Clicks
 **/
static void mouse_clicks(int button, int state, int mx, int my)
{
	switch (button)
	{
		case GLUT_LEFT_BUTTON:
			break;
			
		case GLUT_RIGHT_BUTTON:
			break;
			
		case GLUT_MIDDLE_BUTTON:
			break;
			
	}
	
	
}



/**
 Clear out the screen
 **/
static void init()
{
	//OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glLightModelf(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_DEPTH_TEST);
	
	//Anti-Aliasing & transparency
	
	glEnable (GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glLineWidth (1);
	
	
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Load models
	OBJModel* ball = new OBJModel("models/ball.obj");
	PolyMeshNode* ballNode = new PolyMeshNode("ball");
	ballNode->translate->y = 1.0f;
	ballNode->attachModel(ball);
	Texture* ballTexture = new Texture("textures/star.bmp");
	ballNode->attachTexture(ballTexture);
	 
	
	//Create a default light
	LightNode* lightNode = new LightNode("light", GL_LIGHT0, DIRECTIONAL);
	lightNode->rotate->x = 45.0f;
	lightNode->translate->y = 2.0f;
	
	
	//Scene Graph
	sceneGraph = new SceneGraph();
	sceneGraph->rootNode->addChild(ballNode);
	sceneGraph->rootNode->addChild(lightNode);
	
	ballCam = new CameraNode("ballcam", POLAR);
	ballCam->distance = 10.0f;
	ballCam->elevation = 40.0f;
	ballNode->addChild(ballCam);
	
	/*
	sgCamera = new CameraNode("camera", POLAR);
	sgCamera->distance = 30.0f;
	sgCamera->azimuth = 0.0f;
	sgCamera->elevation = 40.0f;
	 */
	
	//Initialize other variables
	ballVelocity = new Vector3();
	
	upVector = new Vector3(0.0f, 1.0f, 0.0f);
	Vector3* rightVector = new Vector3(1.0f, 0.0f, 0.0f);
	Vector3* zVector = rightVector->crossProduct(upVector);
	cout << "ZVector: " << zVector << endl;
	
	
}

/**
 Main function
 **/
int main(int argc, char** argv)
{
	
	//Get current directory
	cout << "Firing up OBJViewer" << endl;
	cout << "===================" << endl << endl;
	
	cout << "Camera Motion " << endl;
	cout << "Orbit: Left click and drag mouse" << endl;
	cout << "Panning: Middle click and drag mouse" << endl;
	cout << "Dolly: Right click and drag mouse" << endl << endl;
	
	cout << "Viewport Controls " << endl;
	cout << "w: Toggle wireframe" << endl;
	cout << "s: Toggle shaded mode" << endl;
	cout << "p: Perspective mode" << endl;
	cout << "o: Orthographic mode" << endl;
	cout << "+ or z: Zoom in" << endl;
	cout << "- or Z(shift-z): Zoom out" << endl;
	cout << "c: Reset view" << endl << endl;
	
	cout << "Escape: Quit" << endl;
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBJViewer Test - Leonard Teo");
	
	//Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_func);
	glutSpecialFunc(functionKeys);
	glutMouseFunc(mouse_clicks);
	glutMotionFunc(mouse_move);
	
	// Start the timer
    glutTimerFunc(TIMERMSECS, animate, 0);
	
	// Initialize the time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;
	

	
	glutMainLoop();
	
	exit(0);
	
}