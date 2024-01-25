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
#include "glut.h"
//#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#define NUM_MAX_VERTEX_PER_FACE 10
#define NUM_MAX_VERTEX 10000
#define NUM_MAX_FACE 1000

/*	Create checkerboard image	*/
#define	checkImageWidth 640
#define	checkImageHeight 480
GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static GLdouble zoomFactor = 1.0;
static GLint height;

char buff[512], buff2[512];
int nNumVertex, nNumFace;
float vertex[NUM_MAX_VERTEX][3];
Face_t face[NUM_MAX_FACE];

class Edge
{
    int yMax;
    int x;
    float inverseOfSlope;

};
Edge ET[checkImageHeight][200];
int numEdge[checkImageHeight] = { 0 };

void makeCheckImage(void)
{
	vertex[NUM_MAX_VERTEX][3];
	face[NUM_MAX_FACE];
	nNumVertex;
	nNumFace;
	ET[0][1] = 1;

	int i, j;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {

			checkImage[i][j][0] = (GLubyte)0;
			checkImage[i][j][1] = (GLubyte)0;
			checkImage[i][j][2] = (GLubyte)0;

		}
	}
}



void init(void)
{    
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
   
   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   makeCheckImage();
   glRasterPos2i(0, 480);
   glPixelZoom (1.f, -1.f);
   glDrawPixels(checkImageWidth, checkImageHeight, GL_RGB, 
                GL_UNSIGNED_BYTE, checkImage);
   glFlush();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   height = (GLint) h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble) h);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void motion(int x, int y)
{
   static GLint screeny;
   
   screeny = height - (GLint) y;
   glRasterPos2i (x, screeny);
   glPixelZoom (zoomFactor, zoomFactor);
   glCopyPixels (0, 0, checkImageWidth, checkImageHeight, GL_COLOR);
   glPixelZoom (1.0, 1.0);
   glFlush ();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 'r':
      case 'R':
         zoomFactor = 1.0;
         glutPostRedisplay();
         printf ("zoomFactor reset to 1.0\n");

         break;
      case 'z':
         zoomFactor += 0.5;
         if (zoomFactor >= 3.0) 
            zoomFactor = 3.0;
         printf ("zoomFactor is now %4.1f\n", zoomFactor);
         break;
      case 'Z':
         zoomFactor -= 0.5;
         if (zoomFactor <= 0.5) 
            zoomFactor = 0.5;
         printf ("zoomFactor is now %4.1f\n", zoomFactor);
         break;
      case 27:
         exit(0);
         break;
      default:
         break;
   }
}

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(p) assert(p)
#define VERIFY(p) assert(p)
#else
#define ASSERT(p)
#define VERIFY(p) p
#endif


typedef struct {
	int m_nNumVertex;
	unsigned char m_color[3];
	int m_vertex[NUM_MAX_VERTEX_PER_FACE];
} Face_t;

void getNewLine(char * buff, int count, FILE* pFile)
{
	fgets(buff, count, pFile);

	while(buff[0] =='#')
		VERIFY(fgets(buff, count, pFile));
}
	
void readFile(char * pFileName)
{

	int i, j;

	FILE * pFile = fopen(pFileName, "rt");
	getNewLine(buff, 512, pFile);
	sscanf(buff, "%s %d", buff2, &nNumVertex);
	ASSERT(stricmp(buff2, "$Vertex")==0);

	getNewLine(buff, 512, pFile);
	sscanf(buff, "%s %d", buff2, &nNumFace);
	ASSERT(stricmp(buff2, "$Faces")==0);


	for(i=0; i < nNumVertex; i++)
	{
		int nNum;
		getNewLine(buff, 512, pFile);
		sscanf(buff, "%s %d %f %f %f", buff2, &nNum,  &vertex[i][0], &vertex[i][1], &vertex[i][2]);
		sscanf(buff, "%s ", buff2);

		ASSERT(stricmp(buff2, "Vertex")==0);
		ASSERT((nNum-1)==i);
	}


	for(i=0; i < nNumFace; i++)
	{
		int nNum;
		int nCurrPos;

		getNewLine(buff, 512, pFile);
		sscanf(buff, "%s %d %d %d %d %d", buff2, &nNum,  &face[i].m_color[0], &face[i].m_color[1], &face[i].m_color[2], &face[i].m_nNumVertex);
		ASSERT(stricmp(buff2, "Face")==0);
		ASSERT((nNum-1)==i);
		nCurrPos = 0;
		for(j=0; j < 5; j++)
		{
			nCurrPos += strcspn(buff+nCurrPos, " \t");
			nCurrPos += strspn(buff+nCurrPos, " \t");
		}
		for(j=0; j < face[i].m_nNumVertex; j++)
		{
            nCurrPos += strcspn(buff+nCurrPos, " \t");
			nCurrPos += strspn(buff+nCurrPos,  " \t");
			sscanf(buff+nCurrPos, "%d", &face[i].m_vertex[j]);
		}
	}
	

}



int main(int argc, char** argv)
{
	readFile("input.msh");
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

