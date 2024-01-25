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

#include"Matrix4.h"
#include"Vector4.h"
#include "Renderer.h"
#include <cmath>

#include "glut.h"
#include <stdio.h>
#include <string.h>

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
	//카메라(눈)의 위치
	Vector4 eye(5.0f, 0.0f, 5.0f);

	//카메라의 초점(참조 위치)
	Vector4 at(0.0f, 0.0f, 0.0f);

	// 카메라의 위쪽 방향 지정(x=1이면 x축으로 누워있고, y=1이면 y축을 중심으로 세워져있음
	Vector4 upVector(0.0f, 1.0f, 0.0f);

	
	//Matrix4 mForward = identityMatrix();
	//g_renderer.m_view = calculate_lookAt_matrix(Vector4(camX, 0.0f, camZ), Vector4(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f));

	//Matrix4 view = identityMatrix();
	//float radius = 10.0f;

	//float camX = sin(30.0f) * radius;
	//float camZ = cos(30.0f) * radius;
	//view = lookAt(Vector4(camX, 0.0f, camZ), Vector4(0.0f, 0.0f, 0.0f), Vector4(0.0f, 1.0f, 0.0f));

		Vector4 a;
	a.x = 0;
	a.y = 0;
	a.z = 1;
	a.w = 1;

	Vector4 forward = eye - at;
	forward = normalize(forward);

	// compute the left vector
	Vector4 left = crossProduct(upVector, forward); // cross product
	left = normalize(left);

	// recompute the orthonormal up vector
	Vector4 up = crossProduct(forward, left);    // cross product
	up = normalize(up);
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
		g_renderer.m_world = scaleMatrix(0.5f, 0.5f, 0.5f) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case 'Q':
		g_renderer.m_world = scaleMatrix(2.0f, 2.0f, 2.0f) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case '4':
		g_renderer.m_world = translateMatrix(-2.0f, 0, 0) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case '6':
		g_renderer.m_world = translateMatrix(2.0f, 0, 0) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case '8':
		g_renderer.m_world = translateMatrix(0, -2.0f, 0) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case '2':
		g_renderer.m_world = translateMatrix(0, 2.0f, 0) * g_renderer.m_world;
		glutPostRedisplay();

		break;
		
	case 'i':
		g_renderer.m_world = xRotateMatrix(30.0f) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case 'o':
		g_renderer.m_world = yRotateMatrix(30.0f) * g_renderer.m_world;
		glutPostRedisplay();

		break;

	case 'p':
		g_renderer.m_world = zRotateMatrix(30.0f) * g_renderer.m_world; 
		glutPostRedisplay();

		break;

	case 'l':
		g_renderer.m_view = lookAt(eye, at, upVector);
		glutPostRedisplay();
		break;

		//위도 경도

	case 'w':

		//forward = eye - at;
		//forward = normalize(forward);

		//eye = xRotateMatrix(1.0f) * zRotateMatrix(1.0f) * forward * a;

		//float theta1 = 10.0f;
		//float theta2 = 10.0f;
		//float forward1 = 0.0f;;
		//float scaleLength = forward1;
		//void xzRotate(theta1, theta2, forward1)
		//{

		//}

		//eye.x = 10.0f;		eye.y = 0.0f;			eye.z = 10.0f;
		//at.x = 0.0f;		at.y = 0.0f;			at.z = 0.0f;
		//upVector.x = 0.0f;	upVector.y = 1.0f;		upVector.z = 0.0f;

		//forward = eye - at;
		//forward = normalize(forward);

		//
		////scale(l)
		//mForward.arr[0][0] = forward.x;
		//mForward.arr[1][1] = forward.y;
		//mForward.arr[2][2] = forward.z;

		//g_renderer.m_view = lookAt(eye, at, upVector) * g_renderer.m_view;

		//g_renderer.m_view = xRotateMatrix(30.0f) * zRotateMatrix(30.0f) * mForward * lookAt(eye, at, upVector) * g_renderer.m_view ;

		//g_renderer.clearCheckImage();

		/*g_renderer.m_view.arr[0][0] -= 0.001f;	g_renderer.m_view.arr[0][1] -= 0.000f;	g_renderer.m_view.arr[0][2] -= 0.035f;	g_renderer.m_view.arr[0][3] -= 0.000f;
		g_renderer.m_view.arr[1][0] -= 0.000f;	g_renderer.m_view.arr[1][1] -= 0.000f;	g_renderer.m_view.arr[1][2] -= 0.000f;	g_renderer.m_view.arr[1][3] -= 0.000f;
		g_renderer.m_view.arr[2][0] += 0.035f;	g_renderer.m_view.arr[2][1] -= 0.000f;	g_renderer.m_view.arr[2][2] -= 0.001f;	g_renderer.m_view.arr[2][3] -= 0.000f;
		g_renderer.m_view.arr[3][0] -= 0.000f;	g_renderer.m_view.arr[3][1] -= 0.000f;	g_renderer.m_view.arr[3][2] -= 0.000f;	g_renderer.m_view.arr[3][3] -= 0.000f;*/

		//g_renderer.m_view.arr[0][0] -= 0.001f;
		//g_renderer.m_view.arr[2][0] += 0.035f;
		//g_renderer.m_view.arr[0][2] -= 0.035f;
		//g_renderer.m_view.arr[2][2] -= 0.001f;

		//y+

		//forward = eye - at;
		//forward = normalize(forward);


		

		//eye = xRotateMatrix(0.5f) * zRotateMatrix(0.5f) * forward * a;

		seta -= 10.0f;

		eye = xRotateMatrix(seta) * zRotateMatrix(0.5f) * scaleMatrix(length, length, length) * a;



		g_renderer.m_view = lookAt(eye, at, upVector);




		
		

		//eye = RotateMatrix(left, 5.0f) * a; //* forward;

		//eye = xlookatRotate(0.1f) * forward * a;


		glutPostRedisplay();
		break;

	case 's': //y-

		//eye.x = 20.0f;	eye.y = 0.0f;	eye.z = 30.0f;
		//at.x = 0.0f;	at.y = 0.0f;	at.z = 0.0f;
		//upVector.x = 0.0f;	upVector.y = 1.0f;	upVector.z = 0.0f;

		//세타값 설정

		//0.001 -인데 +로 임시
		//g_renderer.m_view.arr[0][0] += 0.001f;
		//g_renderer.m_view.arr[2][0] -= 0.035f;
		//g_renderer.m_view.arr[0][2] += 0.035f;
		//g_renderer.m_view.arr[2][2] += 0.001f;

		seta += 10.0f;

		eye = xRotateMatrix(seta) * zRotateMatrix(0.5f) * scaleMatrix(length, length, length) * a;




		g_renderer.m_view = lookAt(eye, at, upVector);


		//g_renderer.m_view = lookAt(eye, at, upVector) * g_renderer.m_view;
		glutPostRedisplay();
		break;

	case 'a':

		//x+

		//g_renderer.m_view.arr[1][1] -= 0.001f;
		//g_renderer.m_view.arr[1][2] -= 0.035f;
		//g_renderer.m_view.arr[2][1] += 0.035f;
		//g_renderer.m_view.arr[2][2] -= 0.001f;


		/*g_renderer.m_view.arr[0][0] -= 0.001f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.035f;	g_renderer.m_view.arr[0][0] -= 0.000f;
		g_renderer.m_view.arr[1][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;
		g_renderer.m_view.arr[2][0] += 0.035f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.001f;	g_renderer.m_view.arr[0][0] -= 0.000f;
		g_renderer.m_view.arr[3][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;	g_renderer.m_view.arr[0][0] -= 0.000f;*/


		seta += 10.0f;

		eye = yRotateMatrix(seta) * xRotateMatrix(0.5f) * scaleMatrix(length, length, length) * a;


		g_renderer.m_view = lookAt(eye, at, upVector);
		




		//g_renderer.m_view = yRotateMatrix(30.0f) * zRotateMatrix(30.0f) * lookAt(eye, at, upVector) * g_renderer.m_view;
		glutPostRedisplay();
		break;

	case 'd':

		//x-

		seta -= 10.0f;

		eye = yRotateMatrix(seta) * xRotateMatrix(0.5f) * scaleMatrix(length, length, length) * a;




		g_renderer.m_view = lookAt(eye, at, upVector);


		//g_renderer.m_view.arr[1][1] += 0.001f;
		//g_renderer.m_view.arr[1][2] += 0.035f;
		//g_renderer.m_view.arr[2][1] -= 0.035f;
		//g_renderer.m_view.arr[2][2] += 0.001f;

		//g_renderer.m_view = lookAt(eye, at, upVector) * g_renderer.m_view;
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
	g_renderer.readFile("input.msh");

	//transformed Vertex 값을 초기화
	g_renderer.m_world = translateMatrix(0, 0, 0) * g_renderer.m_world; 
	
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
