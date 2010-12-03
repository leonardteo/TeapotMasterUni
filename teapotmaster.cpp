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
#include <gl/glew.h>
#include <gl/glut.h>
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
#include "Shader.h"

#include "textfile.h"

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
const float ballCircumference = 15.707963267948966192313216916398f;
const float speedLimit = 0.7f;

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

//Collision Mesh
PolyMeshNode* collisionNode;

//Shaders
bool useShaders = true;
Shader* basicShader;
void setShaders();

Shader* floorShader;


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
		ballCam->elevation += 4.0f;
	}
	if (downPressed)
	{
		ballCam->elevation -= 4.0f;
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
	if (ballCam->elevation < -10.0f)
	{
		ballCam->elevation = -10.0f;
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
	if (velocityVec.length() > speedLimit)
	{
		velocityVec.normalize();
		velocityVec = velocityVec * speedLimit;
		newPosition = ballpos + velocityVec;
	}

	/**
	Collision Detection
	**/
	//For each plane, check collision
	for (int face=0; face < collisionNode->numFaces; face++)
	{

		//Check if ball collides with plane
		float planeDistance = collisionNode->collisionPlanes[face]->classifyPoint(newPosition);
		//cout << "Position collision check: " << planeDistance << endl;

		//Rude test. if collision with plane, stop the ball
		if (planeDistance < 2.5f)
		{
			//Check if ball is inside the triangle
			if (collisionNode->collisionPlanes[face]->insideTriangle(newPosition))
			{
				newPosition = ballpos;
				velocityVec.zero();
			}
			
		}

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


	//Reduce velocity vector - drag factor
	Vector3 velocityVecNormalized = velocityVec;
	velocityVecNormalized.normalize();
	velocityVec = velocityVecNormalized * (velocityVec.length() - 0.01);
	if (velocityVec.length() < 0.01) velocityVec.zero();


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
	//grid.display(4);
	
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
			

		case 'o':
			useShaders = !useShaders;
			if (useShaders){
				cout << "Turning on shaders" << endl;
				setShaders();
			} else {
				cout << "Turning off shaders" << endl;
				sceneGraph->getNode("ball")->shader = NULL;
				sceneGraph->getNode("ground")->shader = NULL;
			}
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
Load the level
**/
Node* loadLevel()
{
	//Create a null node to load the level mesh
	TransformNode* levelNode = new TransformNode("level");

	//Load the level
	PolyMeshNode* floorNode = new PolyMeshNode("floor", "models/floor.obj");
	floorNode->attachTexture("textures/floor.bmp");
	levelNode->addChild(floorNode);

	PolyMeshNode* frontWallNode = new PolyMeshNode("frontwall", "models/wall_front.obj");
	frontWallNode->attachTexture("textures/wall_front.bmp");
	levelNode->addChild(frontWallNode);

	PolyMeshNode* rearWallNode = new PolyMeshNode("rearwall", "models/wall_rear.obj");
	rearWallNode->attachTexture("textures/wall_rear.bmp");
	levelNode->addChild(rearWallNode);

	PolyMeshNode* rightWallNode = new PolyMeshNode("rightwall", "models/wall_right.obj");
	rightWallNode->attachTexture("textures/wall_right.bmp");
	levelNode->addChild(rightWallNode);

	PolyMeshNode* leftWallNode = new PolyMeshNode("leftwall", "models/wall_left.obj");
	leftWallNode->attachTexture("textures/wall_left.bmp");
	levelNode->addChild(leftWallNode);

	PolyMeshNode* ceilingNode = new PolyMeshNode("ceiling", "models/ceiling.obj");
	ceilingNode->attachTexture("textures/ceiling.bmp");
	levelNode->addChild(ceilingNode);

	PolyMeshNode* mouldingsNode = new PolyMeshNode("mouldings", "models/mouldings.obj");
	mouldingsNode->attachTexture("textures/mouldings.bmp");
	levelNode->addChild(mouldingsNode);

	//Load bed, mattress and blanket
	PolyMeshNode* bedNode = new PolyMeshNode("bed", "models/bed.obj");
	bedNode->attachTexture("textures/bed.bmp");
	levelNode->addChild(bedNode);

	PolyMeshNode* mattressNode = new PolyMeshNode("mattress", "models/mattress.obj");
	mattressNode->attachTexture("textures/mattress.bmp");
	levelNode->addChild(mattressNode);

	PolyMeshNode* blanketNode = new PolyMeshNode("blanket", "models/blanket.obj");
	blanketNode->attachTexture("textures/blanket.bmp");
	levelNode->addChild(blanketNode);

	//Load furniture
	PolyMeshNode* bedsideNode = new PolyMeshNode("bedside", "models/bedside.obj");
	bedsideNode->attachTexture("textures/bedside.bmp");
	levelNode->addChild(bedsideNode);

	PolyMeshNode* chestNode = new PolyMeshNode("chest", "models/chest.obj");
	chestNode->attachTexture("textures/chest.bmp");
	levelNode->addChild(chestNode);

	PolyMeshNode* ottomanNode = new PolyMeshNode("ottman", "models/ottoman.obj");
	ottomanNode->attachTexture("textures/ottoman.bmp");
	levelNode->addChild(ottomanNode);

	PolyMeshNode* tableNode = new PolyMeshNode("table", "models/table.obj");
	tableNode->attachTexture("textures/table.bmp");
	levelNode->addChild(tableNode);

	PolyMeshNode* shelfNode = new PolyMeshNode("shelf", "models/shelf.obj");
	shelfNode->attachTexture("textures/shelf.bmp");
	levelNode->addChild(shelfNode);

	PolyMeshNode* wallShelfNode = new PolyMeshNode("wallshelf", "models/wallshelf.obj");
	wallShelfNode->attachTexture("textures/wallshelf.bmp");
	levelNode->addChild(wallShelfNode);

	PolyMeshNode* dresserNode = new PolyMeshNode("dresser", "models/dresser.obj");
	dresserNode->attachTexture("textures/dresser.bmp");
	levelNode->addChild(dresserNode);

	//Load doors and windows
	PolyMeshNode* doorNode = new PolyMeshNode("door", "models/door_left.obj");
	doorNode->attachTexture("textures/door_left.bmp");
	levelNode->addChild(doorNode);

	PolyMeshNode* closetDoorNode = new PolyMeshNode("closetdoor", "models/closetdoor.obj");
	closetDoorNode->attachTexture("textures/closetdoor.bmp");
	levelNode->addChild(closetDoorNode);

	PolyMeshNode* rightWindow = new PolyMeshNode("rightwindow", "models/window_right.obj");
	rightWindow->attachTexture("textures/window_right.bmp");
	levelNode->addChild(rightWindow);

	PolyMeshNode* frontWindow = new PolyMeshNode("frontwindow", "models/window_front.obj");
	frontWindow->attachTexture("textures/window_front.bmp");
	levelNode->addChild(frontWindow);

	//Load skybox
	PolyMeshNode* skybox = new PolyMeshNode("skybox", "models/skybox.obj");
	skybox->attachTexture("textures/skybox.bmp");
	levelNode->addChild(skybox);

	return levelNode;
	
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
	
	glClearColor(0.2, 0.2, 0.2, 0.0);

	//Load models
	OBJModel* ballOBJ = new OBJModel("models/ball.obj");
	PolyMeshNode* ballMeshNode = new PolyMeshNode("ball");
	ballMeshNode->activeCollider = true;
	ballMeshNode->colliderSphereRadius = 2.5f;
	ballMeshNode->attachModel(ballOBJ);
	Texture* ballTexture = new Texture("textures/star.bmp");
	ballMeshNode->attachTexture(ballTexture);

	Material* ballMaterial = new Material();
	ballMaterial->setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	//ballMaterial->setAmbient(0.25f, 0.25f, 0.25f, 1.0f);
	ballMaterial->setShininess(20.0f);
	ballMaterial->setSpecular(0.8f, 0.8f, 0.8f, 1.0f);
	
	ballMeshNode->attachMaterial(ballMaterial);

	ballRotationNode = new TransformNode("ballRotation", BALLROTATE);
	ballRotationNode->addChild(ballMeshNode);

	ballPositionNode = new TransformNode("ballPosition", TRANSLATE);
	ballPositionNode->setTranslation(0.0f, 2.5f, 0.0f);		//Center of the ball is 2.5 up
	ballPositionNode->addChild(ballRotationNode);
	
	//Offset the camera upwards slightly
	TransformNode* ballCamOffset = new TransformNode("ballCamOffset", TRANSLATE);
	ballCamOffset->setTranslation(0.0f, 7.5f, 0.0f);
	ballPositionNode->addChild(ballCamOffset);

	//Ball Camera
	ballCam = new CameraNode("ballcam", POLAR);
	ballCam->fov = 90.0f;
	ballCam->distance = 20.0f;
	ballCam->elevation = 20.0f;
	ballCamOffset->addChild(ballCam);
	
	//Create a default light
	LightNode* lightNode = new LightNode("light", GL_LIGHT0, POINTLIGHT);
	//lightNode->rotate->x = 45.0f;
	//lightNode->translate->y = 2.0f;
	lightNode->setTranslation(0.0f, 50.0f, 0.0f);

	//Create a few lights
	LightNode* lightNode2 = new LightNode("light2", GL_LIGHT1, POINTLIGHT);
	lightNode2->setTranslation(100.0f, 50.0f, 0.0f);
	lightNode2->intensity = 0.2f;
	LightNode* lightNode3 = new LightNode("light3", GL_LIGHT2, POINTLIGHT);
	lightNode3->setTranslation(-100.0f, 50.0f, 0.0f);
	lightNode3->intensity = 0.2f;
	
	//Load blinn shader
	Shader* blinnShader = new Shader();
	blinnShader->loadVertexShader("shaders/blinn.vert");
	blinnShader->loadFragmentShader("shaders/blinn.frag");
	blinnShader->compile();
	
	//Scene Graph
	sceneGraph = new SceneGraph();
	sceneGraph->rootNode->addChild(lightNode);
	sceneGraph->rootNode->addChild(ballPositionNode);
	sceneGraph->rootNode->addChild(lightNode2);
	sceneGraph->rootNode->addChild(lightNode3);


	//Load the level into the root node
	//sceneGraph->rootNode->addChild(loadLevel());

	//Load collision mesh
	collisionNode = new PolyMeshNode("collision", "models/testcollider.obj");
	collisionNode->staticCollider = true;
	collisionNode->initStaticCollider();
	sceneGraph->rootNode->addChild(collisionNode);
	
	setShaders();
}

void setShaders()
{
	//Attach ball shader
	basicShader = new Shader();
	basicShader->loadVertexShader("shaders/blinn.vert");
	basicShader->loadFragmentShader("shaders/blinn.frag");
	basicShader->compile();
	sceneGraph->getNode("ball")->attachShader(basicShader);
}

/**
 Main function
 **/
int main(int argc, char** argv)
{
	
	//Get current directory
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Teapot Master - Leonard Teo");
	
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