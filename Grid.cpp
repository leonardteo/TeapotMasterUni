/*
 *  Grid.cpp
 *  TeapotMaster
 *
 *  Created by Leonard Teo on 10-11-04.
 *  Copyright 2010 Leonard Teo. All rights reserved.
 *
 */

#include "Grid.h"

/**
 Draw a 24x24 grid
 **/
void Grid::display(float scale)
{
	//Turn off lighting for grid
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	//Start drawing lines
	glBegin(GL_LINES);
	glColor3f(0.5, 0.5, 0.5);
	for (int x=-12; x<=12; x++){
		if (x != 0){
			glVertex3f((GLfloat)(x*scale), (GLfloat)0.0, (GLfloat)(-12*scale));
			glVertex3f((GLfloat)(x*scale), (GLfloat)0.0, (GLfloat)(12*scale));
		}
	}
	for (int z=-12; z<=12; z++){
		if (z != 0){
			glVertex3f((GLfloat)(-12*scale), (GLfloat)0.0, (GLfloat)(z*scale));
			glVertex3f((GLfloat)(12*scale), (GLfloat)0.0, (GLfloat)(z*scale));
		}
	}
	glEnd();
	
	//Make sure the lines at the origin are nice and different
	/*
	 glBegin(GL_LINES);
	 glColor3f(0.75, 0.75, 0.75);
	 glVertex3f(0, 0, (GLfloat)(-12*scale));
	 glVertex3f(0, 0, (GLfloat)(12*scale));
	 glVertex3f((GLfloat)(-12*scale), 0, 0);
	 glVertex3f((GLfloat)(12*scale), 0, 0);
	 glEnd();
	 */
	
	//Draw axis
	glBegin(GL_LINES);
	//x axis should be red and pointing to positive x
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(12*scale, 0.0, 0.0);
	glEnd();
	
	//Draw cone
	glPushMatrix();
	glTranslatef(12*scale, 0.0, 0.0);
	glRotatef(90, 0.0, 1.0, 0.0);
	glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();
	
	glBegin(GL_LINES);
	//y axis
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 12*scale, 0.0);
	glEnd();
	
	//Draw cone
	glPushMatrix();
	glTranslatef(0.0, 12*scale, 0.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();
	
	glBegin(GL_LINES);
	//z axis
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 12*scale);
	glEnd();	
	
	//Draw cone
	glPushMatrix();
	glTranslatef(0.0, 0.0, 12*scale);
	glutSolidCone(0.2, 0.75, 8, 3);
	glPopMatrix();
		
	//Turn lighting back on
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
}