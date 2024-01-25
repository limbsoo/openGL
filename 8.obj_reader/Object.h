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


#define NUM_MAX_VERTEX_PER_FACE 20
#define NUM_MAX_VERTEX 30000
#define NUM_MAX_FACE 30000

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
		haveUV = false;
		haveNormal = false;
	}

	bool haveUV = false;
	bool haveNormal = false;
	bool isDashExist = false;
	bool tryOnce = true;

	Matrix4 m_world;
	int m_nNumVertex, m_nNumFace;

	int m_nNumTexture; 
	int m_nNumNormal;

	//bool isFirst = true;

	float m_vertex[NUM_MAX_VERTEX][3];
	Face_t m_face[NUM_MAX_FACE];

	//float m_tramsformedVertex[NUM_MAX_VERTEX][3];
	float m_tramsformedVertex[NUM_MAX_VERTEX][3];
	float m_uv[NUM_MAX_VERTEX][2] = {};
	float m_rVector[NUM_MAX_VERTEX][3] = { 0 };
	float m_worldVertex[NUM_MAX_VERTEX][3];
	float m_worldVertexNormal[NUM_MAX_VERTEX][3] = { 0 };
	float m_depthVector[NUM_MAX_VERTEX][3] = { 0 };


	float m_faceNormal[NUM_MAX_FACE][3] = { 0 };

	//float fake_faceNormal[NUM_MAX_VERTEX][3] = { 0 };

	float m_transformedFaceNormal[NUM_MAX_FACE][3] = { 0 };

	float m_vertexNormal[NUM_MAX_VERTEX][3] = { 0 };
	float m_transformedVertexNormal[NUM_MAX_VERTEX][3] = { 0 };



	//int i, nrObj = -1, nr = 0, number, number1, number2, number3;


	//char buff[512], buff2[512];



	//int vertexNum = 0;
	//int textureNum = 0;
	//int normalNum = 0;
	//int faceNum = 0;

	//float m_vertex[1000][3];
	//float m_texture[1000][3];
	//float m_vertexNormal[1000][3];
	//float m_face_number[1000][3];
	//float m_face_texture[1000][3];
	//float m_face_normal[1000][3];
	//float m_vertex[1000][3];

	//int cnt = 0;












	
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

	void applyInverseOfMatrix(Matrix4 mvp, Matrix4 world);

	void readOBJFile(char* pFileName);
};

