/**
Teapot Master
Leonard Teo 
**/

//STD libraries
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <time.h>
#include <vector>

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
#include "NPC.h"

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
const float ballRadius = 2.5f;
const float ballCircumference = 15.707963267948966192313216916398f;
const float speedLimit = 0.7f;

//Useful Math constants
const float degreesToRadians = 0.0174532925f;
const float radiansToDegrees = 57.2957795f;
const float pi = 3.14159265f;

#define TIMERMSECS 16
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

//Teapots
NPC** teapots;	//Array of teapots
const int numTeapots = 5;
int teapotsLeft = numTeapots;
Vector3 teapot1Position;		//To encapsulate into a class...

//Shaders
bool useShaders = true;
Shader* basicShader;
void setShaders();

Shader* floorShader;

//Goal for the teapots
PolyMeshNode* goal;


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


static void drawPlayerStatus()
{
	glDisable(GL_TEXTURE_2D);
	
	int numFinished = numTeapots - teapotsLeft;

	char status[255];

	if (teapotsLeft > 0)
	{
		if (teapotsLeft == 1)
		{
			sprintf(status, "%i/%i teapots rescued. %i teapot remaining.", numFinished, numTeapots, teapotsLeft);
		} else {
			sprintf(status, "%i/%i teapots rescued. %i teapots remaining.", numFinished, numTeapots, teapotsLeft);
		}
	} else {
		sprintf(status, "Congratulations! You rescued all the teapots! :)");
	}
		
	
	glColor3f(1.0f, 1.0f, 1.0f);
	int posx, posy;
	posx = 5;
	posy = height - 20;
	glWindowPos2i(posx, posy);
	
	drawText(status);
	glEnable(GL_TEXTURE_2D);
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
AI Pass
**/
static void update_AI()
{
	
}

/**
Collision Detection
**/
static void update_collision_detection()
{




}

/**
Player Movement pass
**/
static void update_player_movement()
{
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
	if (ballCam->elevation > 85.0f)
	{
		ballCam->elevation = 85.0f;
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
	Ball Collision Detection Against Walls
	**/
	//For each plane, check collision against the ball
	for (int face=0; face < collisionNode->numFaces; face++)
	{

		//Check if ball collides with plane
		float planeDistance = collisionNode->collisionTriangles[face]->sphereCollision(newPosition, ballRadius);

		//cout << "Position collision check: " << planeDistance << endl;

		//Collision Detection
		if (planeDistance == 0.0f)
		{
			//Check if ball is inside the triangle
			bool insideTriangle = collisionNode->collisionTriangles[face]->insideTriangle(newPosition);
			bool edgeCollision = collisionNode->collisionTriangles[face]->sphereEdgeCollision(newPosition, ballRadius);
			if (insideTriangle || edgeCollision)
			{
				/* Debugging
				cout << endl ;
				int currTime = glutGet(GLUT_ELAPSED_TIME);
				cout << "Game time: " << currTime << endl;
				cout << "Collision Detected on face: " << face  << endl;
				cout << "Ball position: " << ballpos << endl;
				cout << "New ball position: " << newPosition << endl;
				cout << "Velocity Vec: " << velocityVec << endl;
				cout << "Face edges: " 
					<< "\n V1: " << collisionNode->collisionTriangles[face]->v1 
					<< "\n V2: " << collisionNode->collisionTriangles[face]->v2
					<< "\n V3: " << collisionNode->collisionTriangles[face]->v3
					<< endl;
				if (insideTriangle) 
				{
					cout << "Inside Triangle Test TRUE" << endl;
				} else {
					cout << "Inside Triangle Test FALSE" << endl;
				}
				if (edgeCollision)
				{
					cout << "Edge Collision Test TRUE" << endl;
				} else {
					cout << "Edge Collision Test FALSE" << endl;
				}
				*/

				//Bounce the ball off - calculate reflection vector  R = V-(2*V.N)N
				Vector3 V = velocityVec;
				if (V.length() >  0.0f)
				{
					V.normalize();
					Vector3 reflectionVec = V - collisionNode->collisionTriangles[face]->normal * (2 * dotProduct(V, collisionNode->collisionTriangles[face]->normal));
					velocityVec = reflectionVec * velocityVec.length() * 0.5f;	//Damp the ball bounce
				} else {
					velocityVec = collisionNode->collisionTriangles[face]->normal * 0.05f;
				}
				newPosition = ballpos + velocityVec;
				
				//break;	//We've collided. Don't bother running through the rest of the tests
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
	velocityVec = velocityVecNormalized * (velocityVec.length() - 0.01f);
	if (velocityVec.length() < 0.01) velocityVec.zero();

}

static void update_teapot_movements()
{
	//Do for teapots
	Vector3 ballpos = ballPositionNode->getPosition();
	Vector3 goalpos = goal->getPosition();

	for (int i=0; i<numTeapots; i++)
	{
	
		if (!teapots[i]->finished)
		{
			//Set up positions
			Vector3 teapotPosition = teapots[i]->getPosition();

			/**
			Check if Teapot has reached goal
			**/
			Vector3 teapot_to_goal = goalpos - teapotPosition;
			float distance_from_goal = teapot_to_goal.length();
			if (distance_from_goal < goal->colliderSphereRadius + teapots[i]->boundingSphereRadius)
			{
				//Teapot has reached the goal. Get rid of it
				teapots[i]->finished = true;
				teapots[i]->visible = false;

				teapotsLeft--;

				cout << "Teapot reached goal! " << teapotsLeft << " teapots remaining!" << endl;
			}

			/**
			Teapot AI
			**/
			Vector3 teapot_to_player = ballpos - teapotPosition;
			float distance_to_player = teapot_to_player.length();

			//Initialize alarmed state to false
			teapots[i]->alarmed = false;

			if (distance_to_player < 20.0f)
			{
				//Teapot is alarmed. Show exclamation mark
				teapots[i]->alarmed = true;
				teapots[i]->exclamation->visible = true;
				
				//Check if teapot is looking towards the player
				teapot_to_player.normalize();
				float test = dotProduct(teapot_to_player, *teapots[i]->heading);
		
				//Check if the teapot can "see" the player. If it can see the player, stop the teapot and only rotate.
				if (test > 0.0f)
				{
					//Stop the teapot. It should only rotate
					teapots[i]->velocity = 0.0f;

					//Calculate angle
					float angle = acosf(test) * RADIANSTODEGREES;

					//Test the rotation
					//Create rotation tests
					Vector3 V = *teapots[i]->heading;
					Matrix4 rotMatrixClockwise; rotMatrixClockwise.rotate(upVector, -5.0f);
					Matrix4 rotMatrixCClockwise; rotMatrixCClockwise.rotate(upVector, 5.0f);
					Vector3 V_clockwise = rotMatrixClockwise * V; 
					Vector3 V_cclockwise = rotMatrixCClockwise * V; 

					float clockwiseAngle = acos(dotProduct(V_clockwise, teapot_to_player)) * RADIANSTODEGREES;
					float cclockwiseAngle = acos(dotProduct(V_cclockwise, teapot_to_player)) * RADIANSTODEGREES;

					if (clockwiseAngle < cclockwiseAngle)
					{
						//Rotate clockwise
						teapots[i]->rotateHeading(3.0f);
					} else {
						//Rotate counter clockwise
						teapots[i]->rotateHeading(-3.0f);
					}

				} else {
					//Teapot cannot "see" the player but it is within a dangerous range, make it run faster the closer the player is.
					teapots[i]->velocity = 0.2f + ((20.0f - distance_to_player)/20.0f) * 0.5f;
				}
		
			} else {

				//Teapot is relaxed. Don't show exclamation mark and relax
				teapots[i]->exclamation->visible = false;
				teapots[i]->velocity = 0.2f;
			}
	

			/**
			Teapot Model Animation 
			**/
			//Feet
			teapots[i]->animationT += (teapots[i]->velocity + 0.3f);	//Rate of animation is linked to velocity, so the teapot goes, the faster his feet move
			if (teapots[i]->animationT > 360.0f) teapots[i]->animationT -= 360.0f;	//Limit the rate to avoid floating number limit
			float z = sin(teapots[i]->animationT) * 0.2f;
			teapots[i]->rightFoot->setTranslation(0.0f, 0.0f, z);
			teapots[i]->leftFoot->setTranslation(0.0f, 0.0f, -z);

			//Rotation
			float rotz = sin(teapots[i]->animationT) * 5.0f;
			teapots[i]->wobbleNode->rotate->z = -rotz;
			


			/**
			Teapot Moving
			**/

			//If teapot is alarmed, it should be turning faster
			float rotrate = 1.0f;
			if (teapots[i]->alarmed)
			{
				rotrate = 3.0f;
			} 

			//Calculate new position of teapot
			Vector3 teapotVelocityVec = *teapots[i]->heading * teapots[i]->velocity;
			Vector3 newTeapotPosition = teapotPosition + teapotVelocityVec;	//Set the new position

			/**
			Teapot/Player Collision Detection
			In the event that the teapot collides with the player.....
			**/

			//Set new variables based on the new teapot position
			teapot_to_player = ballpos - newTeapotPosition;
			distance_to_player = teapot_to_player.length();
			if (distance_to_player < (ballRadius + teapots[i]->boundingSphereRadius))
			{
				velocityVec = teapot_to_player;
				velocityVec.normalize();
				velocityVec = velocityVec * 0.5f;
				//newTeapotPosition = newTeapotPosition + velocityVec;

				//cout << "Player/Teapot intersection" << endl;
				//cout << "VelocityVec: " << velocityVec << endl;
			}

			/**
			Teapot/Teapot Collision Detection
			**/
			for (int j=0; j<numTeapots; j++)
			{

				if (j != i && !teapots[j]->finished) //Don't worry about checking against current teapot
				{
					//Check distance to other teapot
					Vector3 otherTeapotPosition = teapots[j]->getPosition();
					Vector3 otherTeapotToThis = newTeapotPosition - otherTeapotPosition;
					float distanceFromOtherTeapot = otherTeapotToThis.length();

					if (distanceFromOtherTeapot < teapots[i]->boundingSphereRadius + teapots[j]->boundingSphereRadius)
					{
						//Turn the teapot away
						teapots[i]->rotateHeading(rotrate);
						teapotVelocityVec.zero();
					}
				}
			}

		

			/**
			Teapot static collision detection
			**/
			//For each plane, check collision
			for (int face=0; face < collisionNode->numFaces; face++)
			{
				//Check if teapot collides with plane
				float planeDistance = collisionNode->collisionTriangles[face]->sphereCollision(newTeapotPosition, teapots[i]->boundingSphereRadius);

				//Collision Detection
				if (planeDistance == 0.0f)
				{
					//Check if teapot is inside the triangle or hits an edge
					bool faceCollision = collisionNode->collisionTriangles[face]->insideTriangle(newTeapotPosition);
					bool edgeCollision = collisionNode->collisionTriangles[face]->sphereEdgeCollision(newTeapotPosition, teapots[i]->boundingSphereRadius);
					if (faceCollision || edgeCollision)
					{
						//Stop the teapot
						teapotVelocityVec.zero();
					
						//Check what angle the teapot is colliding with the surface
						float test = dotProduct(*teapots[i]->heading, collisionNode->collisionTriangles[face]->normal);	

						//cout << "Test: " << test << endl;

						if (test == -1.0f)
						{
							//Teapot has collided at 90 degrees
							int randomNumber = (rand() % 20 + 1) - 10; //between 1-20
							if (randomNumber > 0)
							{
								teapots[i]->rotateHeading(rotrate);
							} else {
								teapots[i]->rotateHeading(-rotrate);
							}
						} else {
							//Teapot has collided at an angle. 
							//calculate reflection vector  R = V-(2*V.N)N
							Vector3 V = *teapots[i]->heading;
							Vector3 R = V - collisionNode->collisionTriangles[face]->normal * (2 * dotProduct(V, collisionNode->collisionTriangles[face]->normal));
							R.normalize();

							float reflectionAngle = acos(dotProduct(V, R)) * RADIANSTODEGREES;
					
							//Create rotation tests
							Matrix4 rotMatrixClockwise; rotMatrixClockwise.rotate(upVector, -5.0f);
							Matrix4 rotMatrixCClockwise; rotMatrixCClockwise.rotate(upVector, 5.0f);
							Vector3 V_clockwise = rotMatrixClockwise * V; 
							Vector3 V_cclockwise = rotMatrixCClockwise * V; 

							float clockwiseAngle = acos(dotProduct(V_clockwise, R)) * RADIANSTODEGREES;
							float cclockwiseAngle = acos(dotProduct(V_cclockwise, R)) * RADIANSTODEGREES;

							/*
							cout << "\nReflection Angle: " << reflectionAngle << endl;
							cout << "Clockwise rotation angle: " << clockwiseAngle << endl;
							cout << "Counter-Clockwise rotation angle: " << cclockwiseAngle << endl;
							*/

							if (clockwiseAngle < cclockwiseAngle)
							{
								//Rotate clockwise
								teapots[i]->rotateHeading(-rotrate);
							} else {
								//Rotate counter clockwise
								teapots[i]->rotateHeading(rotrate + 0.5f);	//Note the offset is to prevent us from getting stuck on 90 degree edges
							}

						}

						//break;	//We've collided. Don't bother doing further testing

					} 
			
				}
			}

			//Set final teapot position
			teapotPosition = teapotPosition + teapotVelocityVec;
			teapots[i]->setTranslation(teapotPosition.x, teapotPosition.y, teapotPosition.z);
		}

	}
}

/**
 Main update pass
 //Note that the value input here is not used
 **/
static void update(int val)
{
	
	// Set up the next timer tick (do this first)	
	glutTimerFunc(TIMERMSECS, update, 0);
	
	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;
	
	update_player_movement();
	update_teapot_movements();
	update_collision_detection();
	update_AI();
	
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
		
	drawPlayerStatus();
	
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

	PolyMeshNode* cornellbox = new PolyMeshNode("cornellbox", "models/cornellbox.obj");
	cornellbox->attachTexture("textures/cornelltexture.bmp");
	levelNode->addChild(cornellbox);

	PolyMeshNode* cornelllight = new PolyMeshNode("cornelllight", "models/cornelllight.obj");
	cornelllight->material->setDiffuseAndAmbient(1.0f, 1.0f, 1.0f, 1.0f);
	levelNode->addChild(cornelllight);

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
	
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	//Load models
	OBJModel* ballOBJ = new OBJModel("models/ball.obj");
	PolyMeshNode* ballMeshNode = new PolyMeshNode("ball");
	ballMeshNode->activeCollider = true;
	ballMeshNode->colliderSphereRadius = ballRadius;
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
	ballPositionNode->setTranslation(0.0f, ballRadius, 10.0f);		//Center of the ball is 2.5 up
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
	sceneGraph->rootNode->addChild(loadLevel());

	//Load the goal for the teapots
	goal = new PolyMeshNode("goal", "models/goal.obj");
	goal->visible = false;
	goal->setTranslation(49.247f, 0.0f, 40.978f);
	goal->colliderSphereRadius = 8.5f;
	sceneGraph->rootNode->addChild(goal);

	//Load Teapots
	OBJModel* teapotOBJ = new OBJModel("models/teapot.obj");
	OBJModel* exclamationOBJ = new OBJModel("models/exclamation.obj");
	OBJModel* teapot_leftfootOBJ = new OBJModel("models/teapot_leftfoot.obj");
	OBJModel* teapot_rightfootOBJ = new OBJModel("models/teapot_rightfoot.obj");
	Texture* teapotTexture = new Texture("textures/teapottexture.bmp");
	Material* teapotMaterial = new Material();
	teapotMaterial->setDiffuse(1.0f, 1.0f, 1.0f);
	teapotMaterial->setSpecular(0.2f, 0.2f, 0.2f);
	teapotMaterial->setShininess(100.0f);

	teapots = new NPC*[numTeapots];
	for (int i=0; i<numTeapots; i++)
	{
		//Create polymeshnodes
		char name[128];
		sprintf(name, "teapotmesh%i", i);
		PolyMeshNode* teapotMesh = new PolyMeshNode(name);
		teapotMesh->attachShader(blinnShader);
		teapotMesh->attachTexture(teapotTexture);
		teapotMesh->attachModel(teapotOBJ);
		teapotMesh->attachMaterial(teapotMaterial);
		sprintf(name, "exclamation%i", i);
		PolyMeshNode* exclamation = new PolyMeshNode(name);
		exclamation->material->setDiffuseAndAmbient(1.0f, 1.0f, 0.0f);
		exclamation->attachModel(exclamationOBJ);
		exclamation->visible = false;

		//Load feet
		sprintf(name, "teapot_leftfoot_%i", i);
		PolyMeshNode* teapotLeftFoot = new PolyMeshNode(name);
		teapotLeftFoot->attachTexture(teapotTexture);
		teapotLeftFoot->attachModel(teapot_leftfootOBJ);
		teapotLeftFoot->attachShader(blinnShader);
		teapotLeftFoot->attachMaterial(teapotMaterial);
		sprintf(name, "teapot_rightfoot_%i", i);
		PolyMeshNode* teapotRightFoot = new PolyMeshNode(name);
		teapotRightFoot->attachTexture(teapotTexture);
		teapotRightFoot->attachModel(teapot_rightfootOBJ);
		teapotRightFoot->attachShader(blinnShader);
		teapotRightFoot->attachMaterial(teapotMaterial);

		//Wobble Node
		sprintf(name, "teapot_wobble_%i", i);
		TransformNode* wobbleNode = new TransformNode(name, ROTATE);

		//Create character hierarchy
		wobbleNode->addChild(teapotMesh);
		teapotMesh->addChild(teapotLeftFoot);
		teapotMesh->addChild(teapotRightFoot);

		//Instantiate teapot
		sprintf(name, "teapot%i", i);
		teapots[i] = new NPC(name, wobbleNode);
		teapots[i]->setTranslation(i*5.0f, 2.5f, 0.0f);	//y is just up from the ground
		teapots[i]->rotateHeading(i*10.0f);
		teapots[i]->wobbleNode = wobbleNode;
		teapots[i]->wobbleNode->setTranslation(0.0f, -2.5f, 0.0f); //Offset downwards
		teapots[i]->setBoundingSphereRadius(2.5f);
		teapots[i]->exclamation = exclamation;
		exclamation->setTranslation(0.0f, -2.5f, 0.0f);
		teapots[i]->leftFoot = teapotLeftFoot;
		teapots[i]->rightFoot = teapotRightFoot;
		teapots[i]->addChild(exclamation);

		sceneGraph->rootNode->addChild(teapots[i]);
	}

	//Load collision mesh - test collision is models/testcollider.obj, live is collision.obj
	collisionNode = new PolyMeshNode("collision", "models/collision.obj");
	collisionNode->staticCollider = true;
	collisionNode->initStaticCollider();
	//sceneGraph->rootNode->addChild(collisionNode);
	
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

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	
	/* In case you want to demo in full screen.... use this and comment out glutCreateWindow
	//Determine screen resolution
	int screenwidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenheight = glutGet(GLUT_SCREEN_HEIGHT);	
	
	char gamemodestring[30];
	sprintf(gamemodestring, "%ix%i:24", screenwidth, screenheight);
	
	glutGameModeString(gamemodestring);
	glutEnterGameMode(); 
	 */
	
	glutCreateWindow("Teapot Master - Leonard Teo");
	
	

	//Initialize GLEW
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		//Problem: glewInit failed, something is seriously wrong.
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	
	//Create a random number seed
	srand ( time(NULL) );

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
    glutTimerFunc(TIMERMSECS, update, 0);
	
	// Initialize the time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;
	
	glutMainLoop();
	
	exit(0);
	
	
}