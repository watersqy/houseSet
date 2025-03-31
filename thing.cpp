#include<stdlib.h>
#include"GL/glaux.h"
#include<GL/glut.h>
#pragma comment(lib,"glaux")
#include<math.h>
#include<map>
#include<string>
#include<iostream>
using namespace std;

void Bed(int x, int y, int z, int look, int angle, GLuint bed[])
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);//����2D������
	glBindTexture(GL_TEXTURE_2D, bed[look * 2 + 1]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(0.7, 0.7, 0.7);
	glPushMatrix();
	glTranslatef(0, 60, -211);
	auxSolidBox(300, 120, 30);
	glPopMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0, -195);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0, -195);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 120, -195);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 120, -195);//����

	glEnd();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, bed[look * 2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 60, 195);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 60, 195);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 60, -195);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 60, -195);//����
	glEnd();
	glBindTexture(GL_TEXTURE_2D, bed[look * 2 + 1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0, -195);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(-150, 0, 195);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(-150, 60, 195);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 60, -195);//����

	glTexCoord2f(0.0, 0.0); glVertex3f(-150, 0, 195);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0, 195);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 60, 195);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(-150, 60, 195);//����

	glTexCoord2f(0.0, 0.0); glVertex3f(150, 0, 195);//����
	glTexCoord2f(1.0, 0.0); glVertex3f(150, 0, -195);//����
	glTexCoord2f(1.0, 1.0); glVertex3f(150, 60, -195);//����
	glTexCoord2f(0.0, 1.0); glVertex3f(150, 60, 195);//����
	glEnd();
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}

void Box(int x, int y, int z, int look, int angle, GLuint box[])
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);//����2D������
	if (look == 0)
	{
		glBindTexture(GL_TEXTURE_2D, box[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-300, 0, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(300, 0, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(300, 500, 60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-300, 500, 60);//����
		glEnd();

		glBindTexture(GL_TEXTURE_2D, box[1]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(300, 0, -60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-300, 0, -60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-300, 500, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(300, 500, -60);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-300, 0, -60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-300, 0, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-300, 500, 60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-300, 500, -60);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(300, 0, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(300, 0, -60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(300, 500, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(300, 500, 60);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-300, 500, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(300, 500, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(300, 500, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-300, 500, -60);//����
		glEnd();
	}
	else if (look == 1)
	{
		glBindTexture(GL_TEXTURE_2D, box[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-200, 0, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(200, 0, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(200, 200, 60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-200, 200, 60);//����
		glEnd();

		glBindTexture(GL_TEXTURE_2D, box[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(200, 0, -60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-200, 0, -60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-200, 200, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(200, 200, -60);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-200, 0, -60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-200, 0, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-200, 200, 60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-200, 200, -60);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(200, 0, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(200, 0, -60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(200, 200, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(200, 200, 60);//����
		glEnd();

		glBindTexture(GL_TEXTURE_2D, box[4]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-200, 200, 60);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(200, 200, 60);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(200, 200, -60);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-200, 200, -60);//����
		glEnd();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, box[5]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-100, 0, 80);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(100, 0, 80);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(100, 300, 80);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-100, 300, 80);//����
		glEnd();

		glBindTexture(GL_TEXTURE_2D, box[6]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(100, 0, -80);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-100, 0, -80);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-100, 300, -80);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(100, 300, -80);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-100, 0, -80);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-100, 0, 80);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-100, 300, 80);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-100, 300, -80);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(100, 0, 80);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(100, 0, -80);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(100, 300, -80);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(100, 300, 80);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-100, 300, 80);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(100, 300, 80);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(100, 300, -80);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-100, 300, -80);//����
		glEnd();
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}

void Pot(int x, int y, int z, int look, int angle, GLuint pot[])
{
	GLUquadric* qObj;
	qObj = gluNewQuadric();
	gluQuadricTexture(qObj, 1);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 1, 0);

	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, pot[0]);
	gluCylinder(qObj, 15, 18, 25, 16, 16);
	glTranslatef(0, 0, 25);
	glBindTexture(GL_TEXTURE_2D, pot[1]);
	gluSphere(qObj, 17, 16, 16);
	glTranslatef(10, 10, 15);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(0.5, 0.6, 0.5);
	glBindTexture(GL_TEXTURE_2D, pot[1]);
	gluSphere(qObj, 8, 16, 16);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}

void Blanket(int x, int y, int z, int look, int angle, GLuint blanket[])
{
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, blanket[look]);
	if (look == 0 || look == 1)
	{
		glTranslatef(x, y, z);
		glRotatef(angle, 0, 1, 0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-200, 0, 350);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(200, 0, 350);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(200, 0, -350);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-200, 0, -350);//����
		glEnd();
	}
	else if (look == 2)
	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_BACK);
		glTranslatef(x, y, z);
		glRotatef(90, 1, 0, 0);
		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		gluDisk(qObj, 0, 300, 16, 16);
		glEnable(GL_BACK);
		glEnable(GL_CULL_FACE);
	}
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}

void Table(int x, int y, int z, int look, int angle, GLuint table[])
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);

	if (look == 0)
	{
		glBindTexture(GL_TEXTURE_2D, table[0]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-150, 157, 100);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(150, 157, 100);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(150, 157, -100);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-150, 157, -100);//����
		glEnd();

		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		glPushMatrix();
		glRotated(-90, 1, 0, 0);
		glTranslatef(-140, 90, 0);
		gluCylinder(qObj, 10, 10, 150, 16, 16);
		glTranslatef(280, 0, 0);
		gluCylinder(qObj, 10, 10, 150, 16, 16);
		glTranslatef(0, -180, 0);
		gluCylinder(qObj, 10, 10, 150, 16, 16);
		glTranslatef(-280, 0, 0);
		gluCylinder(qObj, 10, 10, 150, 16, 16);
		glTranslatef(140, 90, 153);
		auxSolidBox(300, 200, 6);
		glPopMatrix();
	}
	else if (look == 1)
	{
		glBindTexture(GL_TEXTURE_2D, table[1]);
		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		glPushMatrix();
		glRotated(-90, 1, 0, 0);
		glTranslatef(0, 0, 150);
		gluDisk(qObj, 0, 150, 16, 16);
		glTranslatef(0, 0, -150);
		gluDisk(qObj, 0, 50, 16, 16);
		glBindTexture(GL_TEXTURE_2D, table[0]);
		gluCylinder(qObj, 10, 10, 150, 16, 16);
		glPopMatrix();
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, table[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-150, 107, 150);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(150, 107, 150);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(150, 107, -150);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-150, 107, -150);//����
		glEnd();

		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		glPushMatrix();
		glRotated(-90, 1, 0, 0);
		glTranslatef(-140, 140, 0);
		gluCylinder(qObj, 10, 10, 100, 16, 16);
		glTranslatef(280, 0, 0);
		gluCylinder(qObj, 10, 10, 100, 16, 16);
		glTranslatef(0, -280, 0);
		gluCylinder(qObj, 10, 10, 100, 16, 16);
		glTranslatef(-280, 0, 0);
		gluCylinder(qObj, 10, 10, 100, 16, 16);
		glTranslatef(140, 140, 103);
		auxSolidBox(300, 300, 6);
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}

void Chair(int x, int y, int z, int look, int angle, GLuint chair[])
{
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 1, 0);
	glEnable(GL_TEXTURE_2D);

	if (look == 0)
	{
		glBindTexture(GL_TEXTURE_2D, chair[1]);
		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		glPushMatrix();
		glRotated(-90, 1, 0, 0);
		gluCylinder(qObj, 70, 70, 40, 16, 16);
		glTranslatef(0, 0, 40);
		glBindTexture(GL_TEXTURE_2D, chair[0]);
		gluDisk(qObj, 0, 70, 16, 16);
		glPopMatrix();
	}
	else if (look == 1)
	{
		glBindTexture(GL_TEXTURE_2D, chair[2]);
		glBegin(GL_QUADS);//��
		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 80, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 80, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 80, -45);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 80, -45);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 30, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 30, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 80, 85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 80, 85);//����
		//��
		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 0, -45);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 0, -45);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 200, -45);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 200, -45);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 200, -45);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 200, -45);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 200, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 200, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(70, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-70, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-70, 200, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(70, 200, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(70, 0, -45);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 200, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(70, 200, -45);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-70, 0, -45);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-70, 200, -45);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 200, -85);//����
		glEnd();

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D, chair[2]);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
		//��
		glTexCoord2f(0.0, 0.0); glVertex3f(70, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 0, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 150, 85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(70, 150, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(110, 0, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(110, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(110, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(110, 150, 85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(70, 0, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(110, 0, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(110, 150, 85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(70, 150, 85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(110, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(70, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(70, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(110, 150, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(70, 150, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(110, 150, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(110, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(70, 150, -85);//����
		//��
		glTexCoord2f(0.0, 0.0); glVertex3f(-110, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-110, 0, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-110, 150, 85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-110, 150, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 0, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-70, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-70, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 150, 85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-110, 0, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-70, 0, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-70, 150, 85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-110, 150, 85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-70, 0, -85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-110, 0, -85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-110, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-70, 150, -85);//����

		glTexCoord2f(0.0, 0.0); glVertex3f(-110, 150, 85);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(-70, 150, 85);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(-70, 150, -85);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-110, 150, -85);//����
		glEnd();
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
	else if (look == 2)
	{
		glBindTexture(GL_TEXTURE_2D, chair[3]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-35, 82, 25);//����
		glTexCoord2f(1.0, 0.0); glVertex3f(35, 82, 25);//����
		glTexCoord2f(1.0, 1.0); glVertex3f(35, 82, -25);//����
		glTexCoord2f(0.0, 1.0); glVertex3f(-35, 82, -25);//����
		glEnd();

		GLUquadric* qObj;
		qObj = gluNewQuadric();
		gluQuadricTexture(qObj, 1);
		glPushMatrix();
		glRotated(-90, 1, 0, 0);
		glTranslatef(-25, 15, 0);
		gluCylinder(qObj, 8, 8, 75, 16, 16);
		glTranslatef(50, 0, 0);
		gluCylinder(qObj, 8, 8, 75, 16, 16);
		glTranslatef(0, -30, 0);
		gluCylinder(qObj, 8, 8, 75, 16, 16);
		glTranslatef(-50, 0, 0);
		gluCylinder(qObj, 8, 8, 75, 16, 16);
		glTranslatef(25, 15, 78);
		auxSolidBox(70, 50, 6);
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush();
}