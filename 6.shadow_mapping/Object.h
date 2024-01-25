#pragma once

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

typedef struct
{
	int m_nNumVertex;
	unsigned char m_color[3];
	int m_vertex[NUM_MAX_VERTEX_PER_FACE];

} Face_t;

class Object
{

public:

	Object() : m_nNumVertex(0), m_nNumFace(0)
	{
		m_world = identityMatrix();
	}


	Matrix4 m_world;
	int m_nNumVertex, m_nNumFace;

	//bool isFirst = true;

	float m_vertex[3000][3];
	Face_t m_face[6000];

	//float m_tramsformedVertex[NUM_MAX_VERTEX][3];
	float m_tramsformedVertex[3000][3];
	float m_uv[3000][2] = {};
	float m_rVector[6000][3] = { 0 };
	float m_worldVertex[3000][3];
	float m_worldVertexNormal[6000][3] = { 0 };
	float m_depthVector[6000][3] = { 0 };


	float m_faceNormal[6000][3] = { 0 };
	float m_transformedFaceNormal[6000][3] = { 0 };

	float m_vertexNormal[6000][3] = { 0 };
	float m_transformedVertexNormal[6000][3] = { 0 };

	
	void readFile(char* pFileName);
	void getNewLine(char* buff, int count, FILE* pFile);

	void makefaceNormals();
	void makeVertexNormals();
	void makeVertexUVs();
	void applyMatrix(Matrix4 m, Matrix4 n);
	void makeRVector(Vector4 eye);
	void makeWorldVertex();
	void makeWorldVertexNormal();

	void makeVeritces();
	void makeWorldLightingDepth(Vector4 directionLight);
	void makeWorldCameraDepth(Vector4 eye);
};

