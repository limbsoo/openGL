#pragma once

#include <stdio.h>
#include"Matrix4.h"

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(p) assert(p)
#define VERIFY(p) assert(p)
#else
#define ASSERT(p)
#define VERIFY(p) p
#endif
#include "glut.h"

#define NUM_MAX_VERTEX_PER_FACE 10
#define NUM_MAX_VERTEX 10000
#define NUM_MAX_FACE 10000
#define	checkImageWidth 640
#define	checkImageHeight 480

typedef struct 
{
	int m_nNumVertex;
	unsigned char m_color[3];
	int m_vertex[NUM_MAX_VERTEX_PER_FACE];

} Face_t;


class  Edge
{
public:
	Edge() :yMax(0),x(0),inverseOfSlope(0) {

	}
public:
	float yMax;
	float x;
	float inverseOfSlope;
	float z;
};

class  Vertex
{
public:
	Vertex(): xStart(0),yStart(0),yEnd(0),xEnd(0) {

	}
public:
	float xStart;
	int yStart;
	float xEnd;
	float yEnd;
	float z;
};

class Renderer 
{
public:
	Renderer() : m_nNumVertex(0), m_nNumFace(0){
		m_matrix=identityMatrix();
//m_tramsformedVertex[NUM_MAX_VERTEX][3] = m_vertex[NUM_MAX_VERTEX][3];
		//m_tramsformedVertex = transformVertex();
	}
	int m_nNumVertex, m_nNumFace;
	float m_vertex[NUM_MAX_VERTEX][3];

	float m_tramsformedVertex[NUM_MAX_VERTEX][3];

	Face_t m_face[NUM_MAX_FACE];
	Edge m_ET[checkImageHeight][1000] = {};
	//Edge m_AET[200]= {};
	Edge m_AET[1000] = {};

	int m_numEdgeInAET = 0;
	int m_indexCount[checkImageHeight] = { 0 };
	Matrix4 m_matrix;


public:

	GLubyte checkImage[checkImageHeight][checkImageWidth][3];
	
	void readFile(char* pFileName);
	
	void buildEdgetable(int nFace);
	
	void scale(float x, float y, float z);

	void move(float x, float y, float z);

	void translate(float x, float y, float z);

	void rotateX(float x);

	void rotateY(float y);

	void rotateZ(float z);

	void render(); 

	void fill(GLubyte color[3]);

	void clearEdgetable();

	//void transformVertex();

	//void preserveVertex(Vector4 v);

	//void applyVertex(Vector4 v);

	//void currentMatrix();

	void applyMatrix(Matrix4 m, Matrix4 n);

	void getNewLine(char* buff, int count, FILE* pFile);

	void Renderer::clearCheckImage();

};


