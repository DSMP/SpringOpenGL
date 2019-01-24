// Template1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "include\GL\GL.H"
#include "include\GL\GLU.H"
#include "include\GL\glut.h"
#include "Template1.h"


GLfloat M_PI = 3.14f;
GLint mass = 10;
GLfloat k_coeff = 1000;
GLint previous_x = 0;
GLint previous_y = 0;
GLfloat period = (float)(sqrt(mass / k_coeff));
bool growing = true;
int maxDistance = 1;
double b = 0.6;
GLdouble radius = 2.0;
double z_position = 0;
GLuint woodIntTexture, steelIntTexture, springIntTexture;
GLUquadric* quadric;

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
										 //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void ObjectWithTexture(float tx, float ty, float tz, GLuint texture, GLUquadric *quadric, float radius, int rows, int columns)
{
	glTranslatef(tx, ty, tz);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, radius, rows, columns);
	glTranslatef(-tx, -ty, -tz);
}

const int countPoints = (8 * M_PI / 0.2)*(2 * M_PI / 0.5); // its only constatnt not constantExp for arrays declared them size on compile time/stage
struct Point
{
	double x, y, z;
	double u, t;
};
struct Point Points[1500];
void hexagon(int a) {
	glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
	  // Top face (y = 1.0f)
	  // Define vertices in counter-clockwise (CCW) order with normal pointing out
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, woodIntTexture);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);     // Green
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom face (y = -1.0f)
	glTexCoord2f(0.0, 0.0); //texture first
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, -1.0f);

	// Front face  (z = 1.0f)
	glTexCoord2f(0.0, 0.0); //texture first
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face (z = -1.0f)
	glTexCoord2f(0.0, 0.0); //texture first
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face (x = -1.0f)
	glTexCoord2f(0.0, 0.0); //texture first
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face (x = 1.0f)
	glTexCoord2f(0.0, 0.0); //texture first
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();  // End of drawing color-cube
}

void TopHandle()
{
	glPushMatrix();
	glColor3f(0.8, 0.75, 0.4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, springIntTexture);
	gluQuadricTexture(quadric, 1);
	gluCylinder(quadric, 1, 1, 6, 20, 20);
	glPopMatrix();
}
void Spring(int a) {
	glPushMatrix();
	//draw top Handle
	TopHandle();
	//draw spring
	glTranslatef(-3, 0, 5);
	glPointSize(2.0f);
	glColor3f(0.3, 0.3, 0.3);
	int i = 0;
	glBegin(GL_POINTS);
	for (double t = 0; t < 8 * M_PI; t += 0.2)
	{
		for (double u = 0; u < 2 * M_PI; u += 0.5, ++i)
		{
			double x = cos(t)*(3 + cos(u));
			double y = sin(t)*(3 + cos(u));
			double z = b * t + sin(u);
			z_position = z;
			glVertex3f(x, y, z);
			Points[i].t = t; Points[i].u = u;
			Points[i].x = x; Points[i].y = y; Points[i].z = z;
		}
	}
	glEnd();


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, springIntTexture);

	//QL_Quads
	glEnable(GL_TEXTURE_2D);
	for (int j = 0; j < 125; j++)
	{
		glBegin(GL_QUAD_STRIP);
		glColor3f(0.8, 0.75, 0.4);
		for (int i = 0; i < 13; i++)
		{
			glVertex3f(Points[i + 13 * j].x, Points[i + 13 * j].y, Points[i + 13 * j].z);
			glVertex3f(Points[13 + i + 13 * j].x, Points[13 + i + 13 * j].y, Points[13 + i + 13 * j].z);
			if (i == 12)
			{
				glColor3f(0.2, 0.5, 1);
				glVertex3f(Points[13 * j].x, Points[13 * j].y, Points[13 * j].z);
				glVertex3f(Points[13 * j + 13].x, Points[13 * j + 13].y, Points[13 * j + 13].z);
			}
		}
		glEnd();
	}
	glPopMatrix();
}
void BottomHandle(int a)
{
	glPushMatrix();
	glColor3f(0.8, 0.75, 0.4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, springIntTexture);
	gluQuadricTexture(quadric, 1);
	glTranslatef(0, 0, z_position + 4.3);
	gluCylinder(quadric, 1, 1, 6, 20, 20);
	glPopMatrix();
}

void Ball(int a) {
	glPushMatrix();
	glTranslatef(0, 0, z_position + 5 + 5);
	glColor3f(0.4, 0.4, 0.4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, steelIntTexture);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, radius, 20, 20);
	glPopMatrix();
}
void MyDisplay(void) {
	// The new scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	hexagon(1);
	Spring(0.6);
	Ball(1);
	BottomHandle(1);

	// The end of scene
	glFlush();//start processing buffered OpenGL routines
}

void MyInit(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);//select clearing (background) color				

	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//=1

	gluPerspective(70.0, 1.777777777777778, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//=1
	gluLookAt(0, 0, -30, 10, 0, 0, -1, 0, 0);

	unsigned int with, height;
	with = 100;
	height = 100;
	Image* image;
	image = loadBMP("wood24.bmp");
	woodIntTexture = loadTexture(image);
	delete image;
	image = loadBMP("rust24.bmp");
	springIntTexture = loadTexture(image);
	delete image;
	image = loadBMP("steel24.bmp");
	steelIntTexture = loadTexture(image);
	delete image;

	quadric = gluNewQuadric();
}
void OnMotion(int x, int y)
{
	printf(" OnMotion prev: %d %d now: %d %d ", previous_x, previous_y, x, y);
	if (previous_x < x)
	{
		printf("previous_x < x");
		gluLookAt(/*eye*/-0.001, 0, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_x > x)
	{
		printf("previous_x > x");
		gluLookAt(/*eye*/0.001, 0, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_y < y)
	{
		printf("previous_y < y");
		gluLookAt(/*eye*/0, 0.001, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	if (previous_y > y)
	{
		printf("previous_y > y");
		gluLookAt(/*eye*/-0, -0.001, 0,/*look at*/0, 0, -0.05,/*up*/0, 1, 0);
	}
	previous_x = x;
	previous_y = y;
}
void render(int a)
{
	hexagon(a);
	Spring(a);
	Ball(a);
	BottomHandle(a);
	glutPostRedisplay();
	Growing();
	glutTimerFunc(25, render, a);
}
void Growing()
{
	if (growing)
	{
		b += 0.01;
	}
	else
	{
		b -= 0.01;
	}
	if (b <= 0.4)
	{
		growing = true;
	}
	if (b >= maxDistance)
	{
		growing = false;
	}
}

int main(int argc, char** argv) { //<- for normal API
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);//single buffer and RGBA
	glutInitWindowSize(1100, 600);//initial window size
	glutInitWindowPosition(50, 50);
	glutCreateWindow("My window");//create widnow, hello title bar
	MyInit();
	glutDisplayFunc(MyDisplay);//
	glutMotionFunc(OnMotion);
	glutTimerFunc(100, render, 0.6);
	glutMainLoop();//enter main loop and process events
	return 0;
}