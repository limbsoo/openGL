#pragma once

#include <stdio.h>
#include"Matrix4.h"
#include"Vector4.h"
#include "Object.h"

#ifdef _DEBUG
#include <assert.h>
#define ASSERT(p) assert(p)
#define VERIFY(p) assert(p)
#else
#define ASSERT(p)
#define VERIFY(p) p
#endif
#include "glut.h"


#define	checkImageWidth 640
#define	checkImageHeight 480





class  Edge
{
public:
	Edge() :yMax(0),x(0),xperY(0), z(0), 
			u(0), uPerY(0), vPerY(0), zPerY(0) , 
			nx(0), ny(0), nz(0), nxPerY(0), nyPerY(0), nzPerY(0), 
			lx(0), ly(0), lz(0),
			rx(0),ry(0),rz(0), rxPerY(0), ryPerY(0), rzPerY(0) {}
public:
	float yMax;
	float x;
	float xperY;
	float z;

	float u, v, uPerY, vPerY, zPerY;

	float nx, ny, nz, nxPerY, nyPerY, nzPerY;

	float rx, ry, rz;

	float rxPerY, ryPerY, rzPerY;

	float lx, ly, lz;

	float lxPerY, lyPerY, lzPerY;

};

class Renderer 
{
public:
	Renderer() 
	{
		m_view = identityMatrix();
		m_proj = identityMatrix();

		for (int i = 0; i < checkImageHeight; i++)
		{
			for (int j = 0; j < checkImageWidth; j++)
			{
				m_zBuffer[i][j] = 1;
			}
		}

		eye.x = 1.f;
		eye.y = -1.f;
		eye.z = 1.f;

		at.x = 0.f;
		at.y = 0.f;
		at.z = 0.f;

		upVector.x = 0.f;
		upVector.y = 1.f;
		upVector.z = 0.f;

		bias = 0.001;

	}

	float m_zBuffer[checkImageHeight][checkImageWidth];

	

	Matrix4 m_lightView;
	

	//float cowWorldLightingDepthBuffer[checkImageHeight][checkImageWidth] = { 2, };
	//float cowWorldCameraDepthBuffer[checkImageHeight][checkImageWidth] = { 2, };

	float shadowMap[checkImageHeight][checkImageWidth] = { 1, };
	float floorWorldCameraDepthBuffer[checkImageHeight][checkImageWidth] = { 1, };

	float bias;

	Matrix4 screenToShadowMatrix;

	Matrix4 mvp;

	bool isCow = false;
	//bool isSecondRender = true;

	bool isDepthRender = false;
	bool iscameraRender = false;

	float m_texture[480][640][3];
	float m_sphereTexture[480][640][3];

	float m_savedColor[480][640][3];
	float m_savedFloorColor[480][640][3];
	float m_savedZbuffer[480][640];

	float m_cowAET[480][640][3];
	float m_squareAET[480][640][3];

	float m_lightColor[480][640];





	Edge m_ET[checkImageHeight][1000] = {};
	Edge m_AET[1000] = {};

	bool isFirst = true;

	int m_numEdgeInAET = 0;
	int m_indexCount[checkImageHeight] = { 0 };



	
	Matrix4 m_view;
	Matrix4 m_proj;

	bool m_isCullEnabled = false;
	bool m_texureMappingEnabled = false;

	//카메라(눈)의 위치
	Vector4 eye;

	//카메라의 초점(참조 위치)
	Vector4 at;

	// 카메라의 위쪽 방향 지정(x=1이면 x축으로 누워있고, y=1이면 y축을 중심으로 세워져있음
	Vector4 upVector;

	float saveShadowVertex[480][640][3];



public:

	GLubyte checkImage[checkImageHeight][checkImageWidth][3];
	
	
	
	void buildEdgetable(Object *pObject, int nFace);
	void render(); 
	void render(Object *pObject);
	void fill(GLubyte color[3]);


	void clearEdgetable();

	void applyMatrix(Matrix4 m, Matrix4 n);

	void getNewLine(char* buff, int count, FILE* pFile);

	void clearCheckImage();

	bool isBackFace(Object *pObject, int nFace);

	void readTextureFile(char* pFileName);

	
	void clearZBuffer();

	//void sphericaltMapping();

	void readSphereTextureFile(char* pFileName);

	void makeShadowMap();

	void shadowFill(GLubyte color[3]);


};


