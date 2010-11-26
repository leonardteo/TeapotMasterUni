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
TransformNode* ballPositionNode;
TransformNode* ballRotationNode;
int ballVelocity = 0;		//The velocity of the ball
Vector3 upVector(0.0f, 1.0f, 0.0f);		//Up direction
Vector3 forwardVector;
Vector3 velocityVec;
const float ballCircumference = 31.415926535897932384626433832795f;

//Useful Math constants
const float degreesToRadians = 0.0174532925f;
const float radiansToDegrees = 57.2957795f;
const float pi = 3.14159265f;

#define TIMERMSECS 16.666666666666666666666666666667
#define PI 3.14159265f
#define RADIANSTODEGREES 57.2957795f
#define DEGREESTORADIANS 0.0174532925f

//Measuring time in milliseconds
int startTime;
int prevTime;

//Camera movements -- note that we are registering booleans to circumvent GLUT's slow keyboard response
bool upPressed = false;
bool downPressed = false;
bool rightPressed = false;
bool leftPressed = false;

//Ball movements -- again, register the keystrokes to avoid GLUT weirdness
bool wPressed = false; //forward
bool sPressed = false; //back
bool aPressed = false; //left
bool dPressed = false; //right



/** 
DebugText
Write anything I want to cout
**/
void debug()
{
	//Rotation Matrix
	cout << "Rotation matrix: " << endl;
	ballRotationNode->ballRotationMatrix.print();
}

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
 //Note that the value input here is not used
 **/
static void animate(int val)
{
	
	// Set up the next timer tick (do this first)	
	glutTimerFunc(TIMERMSECS, animate, 0);
	
	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;
	
	
	//Camera movements
	if (upPressed)
	{
		ballCam->elevation -= 4.0f;
	}
	if (downPressed)
	{
		ballCam->elevation += 4.0f;
	}
	if (leftPressed)
	{
		ballCam->azimuth -= 4.0f;
	}
	if (rightPressed)
	{
		ballCam->azimuth += 4.0f;
	}
	
	//Error checks to make sure we're in range
	if (ballCam->elevation > 90.0f)
	{
		ballCam->elevation = 90.0f;
	}
	if (ballCam->elevation < 0.0f)
	{
		ballCam->elevation = 0.0f;
	}
	if (ballCam->azimuth > 360.0f)
	{
		ballCam->azimuth -= 360.0f;
	}
	if (ballCam->azimuth < 0.0f)
	{
		ballCam->azimuth += 360.0f;
	}
	
	/****
	Handle ball animation input
	*****/
	
	//Calculate forward vector
	Vector3 ballpos = ballPositionNode->getPosition();	//Get position of ball
	Vector3 campos = ballCam->getPosition();
	Vector3 viewNormal = ballpos - campos;

	//Calculate Movement vectors
	Vector3 rightVector = viewNormal.crossProduct(upVector);
	rightVector.normalize();
	Vector3 forwardVector = upVector.crossProduct(rightVector);
	forwardVector.normalize();
	Vector3 backVector = rightVector.crossProduct(upVector);
	backVector.normalize();
	Vector3 leftVector = upVector.crossProduct(viewNormal);
	leftVector.normalize();
	Vector3 newPosition;

	//Calculate new position of the ball based on the velocity
	newPosition = ballpos + velocityVec;

	if (wPressed)	//w key is pressed. Move forward
	{
		newPosition = newPosition + (forwardVector/20);
	}
	if (sPressed)
	{
		newPosition = newPosition + (backVector/20);
	}
	if (dPressed)
	{
		newPosition = newPosition + (rightVector/20);
	}
	if (aPressed)
	{
		newPosition = newPosition + (leftVector/20);
	}

	//Calculate the velocity vector
	velocityVec = newPosition - ballpos;

	//Limit the speed. If the velocity is greater than the speed limit, set it back, and recalculate the new position
	if (velocityVec.length() > 1.0f)
	{
		velocityVec.normalize();
		newPosition = ballpos + velocityVec;
	}
	
	//Set ball Position
	ballPositionNode->setTranslation(newPosition.x, newPosition.y, newPosition.z);
		
	/**
	Ball Rotation
	**/
	float distance = velocityVec.length();
	float ballAngularDisplacement = (distance / ballCircumference) * 360.0f;
	
	//Calculate axis of rotation
	Vector3 rotationAxis = upVector.crossProduct(velocityVec);
	rotationAxis.normalize();
	ballRotationNode->rotateBall(rotationAxis, ballAngularDisplacement);


	//Reduce velocity vector
	Vector3 velocityVecNormalized = velocityVec;
	velocityVecNormalized.normalize();
	velocityVec = velocityVecNormalized * (velocityVec.length() - 0.025);
	if (velocityVec.length() < 0.001) velocityVec.zero();


	//Post redisplay
	glutPostRedisplay();
	
	//Reset the timer
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
	grid.display(4);
	



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
static void keyboard(unsigned char key, int mx, int my)
{
	switch (key)
	{
			
		case 'w':	//move ball forward
			wPressed = true;
			break;
		case 's':	//move ball back
			sPressed = true;
			break;
		case 'a':	//move ball left
			aPressed = true;
			break;
		case 'd':	//move ball right
			dPressed = true;
			break;

		case 'p':
			debug();
			break;
			
			
			//Exit
		case 27:
			exit(0);
	}
	
}

static void keyboardUp(unsigned char key, int mx, int my)
{
	switch (key)
	{
		case 'w':
			wPressed = false;
			break;
		case 'a':
			aPressed = false;
			break;
		case 's':
			sPressed = false;
			break;
		case 'd':
			dPressed = false;
			break;
	}
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
			leftPressed = true;
			break;
		case GLUT_KEY_RIGHT:
			rightPressed = true;
			break;
		case GLUT_KEY_UP:
			upPressed = true;
			break;
		case GLUT_KEY_DOWN:
			downPressed = true;
			break;
	}

}


/**
 Function keys up
 **/
static void functionKeysUp(int key, int mx, int my)
{
	switch (key)
	{
		//Camera movement
		case GLUT_KEY_LEFT:
			leftPressed = false;
			break;
		case GLUT_KEY_RIGHT:
			rightPressed = false;
			break;
		case GLUT_KEY_UP:
			upPressed = false;
			break;
		case GLUT_KEY_DOWN:
			downPressed = false;
			break;
	}

	
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
	OBJModel* ballOBJ = new OBJModel("models/ball.obj");
	PolyMeshNode* ballMeshNode = new PolyMeshNode("ball");
	ballMeshNode->attachModel(ballOBJ);
	Texture* ballTexture = new Texture("textures/star.bmp");
	ballMeshNode->attachTexture(ballTexture);

	ballRotationNode = new TransformNode("ballRotation", BALLROTATE);
	ballRotationNode->addChild(ballMeshNode);

	ballPositionNode = new TransformNode("ballPosition", TRANSLATE);
	ballPositionNode->setTranslation(0.0f, 5.0f, 0.0f);
	ballPositionNode->addChild(ballRotationNode);
	
	
	//Create a default light
	LightNode* lightNode = new LightNode("light", GL_LIGHT0, DIRECTIONAL);
	lightNode->rotate->x = 45.0f;
	lightNode->translate->y = 2.0f;
	
	
	//Scene Graph
	sceneGraph = new SceneGraph();
	sceneGraph->rootNode->addChild(lightNode);
	sceneGraph->rootNode->addChild(ballPositionNode);
	

	ballCam = new CameraNode("ballcam", POLAR);
	ballCam->distance = 40.0f;
	ballCam->elevation = 40.0f;
	ballPositionNode->addChild(ballCam);
	

	
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
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(functionKeys);
	glutSpecialUpFunc(functionKeysUp);
	glutMouseFunc(mouse_clicks);
	glutMotionFunc(mouse_move);
	//glutIdleFunc(animate);
	
	// Start the timer
    glutTimerFunc(TIMERMSECS, animate, 0);
	
	// Initialize the time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;
	

	
	glutMainLoop();
	
	exit(0);
	
}