/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

 /*  image.c
  *  This program demonstrates drawing pixels and shows the effect
  *  of glDrawPixels(), glCopyPixels(), and glPixelZoom().
  *  Interaction: moving the mouse while pressing the mouse button
  *  will copy the image in the lower-left corner of the window
  *  to the mouse position, using the current pixel zoom factors.
  *  There is no attempt to prevent you from drawing over the original
  *  image.  If you press the 'r' key, the original image and zoom
  *  factors are reset.  If you press the 'z' or 'Z' keys, you change
  *  the zoom factors.
  */

//#include <stdlib.h>
#include <iostream>
#include<vector>
#include <time.h>

#include"Matrix4.h"
#include"Vector4.h"
#include "Renderer.h"
#include <cmath>

#include "glut.h"
#include <stdio.h>
#include <string.h>

#include "Object.h"

#ifdef _DEBUG
#include <assert.h>

using namespace std;

#define ASSERT(p) assert(p)
#define VERIFY(p) assert(p)
#else
#define ASSERT(p)
#define VERIFY(p) p
#endif


static GLdouble zoomFactor = 1.0;
static GLint height;

Renderer g_renderer;
Object g_floor;
Object g_cow;

Matrix4 m_matrix;


void makeCheckImage(void)
{
	g_renderer.render();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	makeCheckImage();
	glRasterPos2i(0, 480);
	glPixelZoom(1.f, -1.f);
	glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB,
		GL_UNSIGNED_BYTE, g_renderer.checkImage);
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	height = (GLint)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void motion(int x, int y)
{
	static GLint screeny;

	screeny = height - (GLint)y;
	glRasterPos2i(x, screeny);
	glPixelZoom(zoomFactor, zoomFactor);
	glCopyPixels(0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
	glPixelZoom(1.0, 1.0);
	glFlush();
}
float seta = 10.0f;




void keyboard(unsigned char key, int x, int y)
{
	Vector4 a;
	a.x = 0;	a.y = 0;	a.z = 1;	a.w = 1;

	Vector4 v;

	float length = 5.0f;
	

	switch (key) {
	case 'r':
	case 'R':
		zoomFactor = 1.0;
		glutPostRedisplay();
		printf("zoomFactor reset to 1.0\n");

		break;
	case 'z':
		zoomFactor += 0.5;
		if (zoomFactor >= 3.0)
			zoomFactor = 3.0;
		printf("zoomFactor is now %4.1f\n", zoomFactor);
		break;
	case 'Z':
		zoomFactor -= 0.5;
		if (zoomFactor <= 0.5)
			zoomFactor = 0.5;
		printf("zoomFactor is now %4.1f\n", zoomFactor);
		break;

	case 'x':
		break;

	case 'q':
		g_cow.m_world = scaleMatrix(0.5f, 0.5f, 0.5f) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case 'Q':
		g_cow.m_world = scaleMatrix(2.0f, 2.0f, 2.0f) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case '4':
		g_cow.m_world = translateMatrix(-0.2f, 0, 0) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case '6':
		g_cow.m_world = translateMatrix(0.2f, 0, 0) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case '8':
		g_cow.m_world = translateMatrix(0, -0.2f, 0) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case '2':
		g_cow.m_world = translateMatrix(0, 0.2f, 0) * g_cow.m_world;
		glutPostRedisplay();

		break;
		
	case 'i':
		g_cow.m_world = xRotateMatrix(30.0f) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case 'o':
		g_cow.m_world = yRotateMatrix(30.0f) * g_cow.m_world;
		glutPostRedisplay();

		break;

	case 'p':
		g_cow.m_world = zRotateMatrix(30.0f) * g_cow.m_world; 
		glutPostRedisplay();

		break;

	case 'l':

		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		//g_renderer.m_proj = projectMatrix(90.0f, 1.3333f, 1.0f, 150.0f);
		glutPostRedisplay();
		break;

	
	case 'b':
		g_renderer.eye.x += 1.0f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'B':
		g_renderer.eye.x -= 1.0f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'n':
		g_renderer.eye.y += 1.0f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'N':
		g_renderer.eye.y -= 1.0f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'm':
		g_renderer.eye.z += 0.5f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'M':
		g_renderer.eye.z -= 0.5f;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;


	case 'w':

		seta -= 30.0f;
		a.x = 0;	a.y = 1;	a.z = 0;	a.w = 1;

		g_renderer.eye = xRotateMatrix(seta) * zRotateMatrix(0.5f) * cameraScaleMatrix(length) * a;

		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 's': 

		seta += 30.0f;
		a.x = 0;	a.y = 1;	a.z = 0;	a.w = 1;

		g_renderer.eye = xRotateMatrix(seta) * zRotateMatrix(0.5f) * cameraScaleMatrix(length) * a;

		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'a':

		seta += 30.0f;
		a.x = 0;	a.y = 0;	a.z = 1;	a.w = 1;

		g_renderer.eye = yRotateMatrix(seta) * xRotateMatrix(0.5f) * cameraScaleMatrix(length) * a;

		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case 'd':

		seta -= 30.0f;
		a.x = 0;	a.y = 0;	a.z = 1;	a.w = 1;

		g_renderer.eye = yRotateMatrix(seta) * xRotateMatrix(0.5f) * cameraScaleMatrix(length) * a;
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		glutPostRedisplay();
		break;

	case '1':
		g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
		g_renderer.m_proj = projectMatrix(90.0f, 1.3333f, 1.0f, 150.0f);

		glutPostRedisplay();
		break;

	case '9':
		g_renderer.m_isCullEnabled = !g_renderer.m_isCullEnabled;

		glutPostRedisplay();
		break;

	case '3':
		glutPostRedisplay();
		break;

	case 27:
		exit(0);
		break;
	default:
		break;
	}
}


int main(int argc, char** argv)
{
	//g_cow.readFile("cow.msh");

	g_cow.readOBJFile("diamond.txt");

	//g_floor.readFile("cow111.msh");

	//transformed Vertex 값을 초기화
	g_cow.m_world = translateMatrix(0, 0, 0) * g_cow.m_world;

	g_renderer.m_view = lookAt(g_renderer.eye, g_renderer.at, g_renderer.upVector);
	g_renderer.m_proj = projectMatrix(90.0f, 1.3333f, 0.4f, 150.0f);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutMainLoop();
	return 0;
}
