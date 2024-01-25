#pragma once

#include <stdio.h>
#include"Matrix4.h"
#include"Vector4.h"

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
	Edge() :yMax(0),x(0),inverseOfSlope(0), z(0), u(0), uPerY(0), vperY(0), zperY(0) {}
public:
	float yMax;
	float x;
	float inverseOfSlope;
	float z;

	float u, v, uPerY, vperY, zperY;

};

class  Vertex
{
public:
	Vertex(): xStart(0),yStart(0),yEnd(0),xEnd(0), z(0) {}
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
	Renderer() : m_nNumVertex(0), m_nNumFace(0)
	{
		m_world = identityMatrix();
		m_view = identityMatrix();
		m_proj = identityMatrix();

		for (int i = 0; i < checkImageHeight; i++)
		{
			for (int j = 0; j < checkImageWidth; j++)
			{
				zBuffer[i][j] = 1;
			}
		}

	}


	int m_nNumVertex, m_nNumFace;
	//float m_vertex[NUM_MAX_VERTEX][3];
	float m_vertex[3000][3];

	//float m_tramsformedVertex[NUM_MAX_VERTEX][3];
	float m_tramsformedVertex[3000][3];
	float m_uv[3000][2] = {};
	float zBuffer[checkImageHeight][checkImageWidth] = { 2, };



	float texture[480][640][3];

	//Face_t m_face[NUM_MAX_FACE];
	Face_t m_face[6000];

	Edge m_ET[checkImageHeight][1000] = {};
	//Edge m_AET[200]= {};
	Edge m_AET[1000] = {};

	int m_numEdgeInAET = 0;
	int m_indexCount[checkImageHeight] = { 0 };

	Matrix4 m_world;
	Matrix4 m_view;
	Matrix4 m_proj;

	bool isCullEnabled = false;
	//bool filltable = false;

	bool texureMappingEnabled = false;

public:

	GLubyte checkImage[checkImageHeight][checkImageWidth][3];
	
	void readFile(char* pFileName);
	
	void buildEdgetable(int nFace);
	
	void render(); 

	void fill(GLubyte color[3]);

	void clearEdgetable();

	void applyMatrix(Matrix4 m, Matrix4 n);

	void getNewLine(char* buff, int count, FILE* pFile);

	void clearCheckImage();

	//float checkWhereTheTriangleIsLooking(Temp tempET[3]);

	bool isBackFace(int nFace);

	void readTextureFile(char* pFileName);

	void makeUVVertex();

	void clearZBuffer();

};


