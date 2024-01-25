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
	Edge() :yMax(0),x(0),xperY(0), z(0), u(0), uPerY(0), vPerY(0), zPerY(0) , nx(0), ny(0), nz(0), nxPerY(0), nyPerY(0), nzPerY(0), lx(0), ly(0), lz(0) {}
public:
	float yMax;
	float x;
	float xperY;
	float z;

	float u, v, uPerY, vPerY, zPerY;

	float nx, ny, nz, nxPerY, nyPerY, nzPerY;

	float lx, ly, lz;

};

//class  Vertex
//{
//public:
//	Vertex(): xStart(0),yStart(0),yEnd(0),xEnd(0), z(0) {}
//public:
//	float xStart;
//	int yStart;
//	float xEnd;
//	float yEnd;
//	float z;
//};


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
				m_zBuffer[i][j] = 1;
			}
		}

	}

	Vector4 pointLightPosition;
	Vector4 pointLightColor;


	int m_nNumVertex, m_nNumFace;
	//float m_vertex[NUM_MAX_VERTEX][3];
	float m_vertex[3000][3];

	//float m_tramsformedVertex[NUM_MAX_VERTEX][3];
	float m_tramsformedVertex[3000][3];
	float m_uv[3000][2] = {};
	//float m_normal[3000][2] = {};
	float m_zBuffer[checkImageHeight][checkImageWidth] = { 2, };

	float m_faceNormal[6000][3] = { 0 };
	float m_vertexNormal[6000][3] = { 0 };

	float m_lightVertex[6000][3] = { 0 };
	float m_lightStrength[6000] = { 0 };

	float m_texture[480][640][3];

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

	bool m_isCullEnabled = false;
	//bool filltable = false;

	bool m_texureMappingEnabled = false;

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

	void makeVertexUVs();

	void clearZBuffer();

	void makefaceNormals();

	void brightnessOfPixels();

	void makeVertexNormals();

};


