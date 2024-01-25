#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include<iostream>
#include<vector>
#include <cmath>
#include <time.h>

#include"Renderer.h"
#include"Vector4.h"
#include"Matrix4.h"

#include <string.h>
#include <stdlib.h>

const double PI = 3.1415926;

extern Object g_cow;
extern Object g_floor;

void Renderer::readTextureFile(char* pFileName)
{
	FILE* input_file;

	char input_data[checkImageHeight][checkImageWidth][3];

	input_file = fopen(pFileName, "rb");

	fread(input_data, sizeof(char), checkImageWidth * checkImageHeight * 3, input_file);

	fclose(input_file);

	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			m_texture[i][j][0] = (GLubyte)input_data[i][j][0];
			m_texture[i][j][1] = (GLubyte)input_data[i][j][1];
			m_texture[i][j][2] = (GLubyte)input_data[i][j][2];
		}
	}
}

//void Renderer::readSphereTextureFile(char* pFileName)
//{
//	FILE* input_file1;
//
//	char input_data1[checkImageHeight][checkImageWidth][3];
//
//	input_file1 = fopen(pFileName, "rb");
//
//	fread(input_data1, sizeof(char), checkImageWidth * checkImageHeight * 3, input_file1);
//
//	fclose(input_file1);
//
//	for (int i = 0; i < checkImageHeight; i++)
//	{
//		for (int j = 0; j < checkImageWidth; j++)
//		{
//			m_sphereTexture[i][j][0] = (GLubyte)input_data1[i][j][0];
//			m_sphereTexture[i][j][1] = (GLubyte)input_data1[i][j][1];
//			m_sphereTexture[i][j][2] = (GLubyte)input_data1[i][j][2];
//		}
//	}
//}



void Renderer::clearCheckImage()
{
	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			checkImage[i][j][0] = (GLubyte)255;
			checkImage[i][j][1] = (GLubyte)192;
			checkImage[i][j][2] = (GLubyte)203;
		}
	}
}

void Renderer::clearZBuffer()
{
	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			m_zBuffer[i][j] = 1;
		}
	}
}

Vector4 lightPosition(2.f, -2.f, -2.f);
Vector4 ndirectionLight = normalize(lightPosition);

void Renderer::render()
{
	clearCheckImage();
	clearZBuffer();

	//카메라의 eye 값을 광원의 lighting Position과 일치시킨다.
	eye.x = 2.f;	eye.y = -2.f;	eye.z = -2.f;

	// view matrix에 적용
	m_view = lookAt(eye, at, upVector);

	//Render한다.
	mvp = m_proj * m_view * g_cow.m_world;

	g_cow.makeWorldLightingDepth(lightPosition);
	g_cow.makeVeritces();
	g_cow.makeRVector(eye);
	g_cow.applyMatrix(mvp, g_cow.m_world);

	isFirstRender = true;
	render(&g_cow);
	isFirstRender = false;

	mvp = m_proj * m_view * g_floor.m_world;

	g_floor.makeVeritces();
	g_floor.makeRVector(eye);
	g_floor.makeWorldLightingDepth(lightPosition);
	g_floor.applyMatrix(mvp, g_floor.m_world);

	render(&g_floor);

	isDepthRender = true;

	//	ZBuffer배열을 ShadowMap배열에 복사한다.

	makeShadowMap();


	clearZBuffer();



	//카메라를 원래의 eye 위치에 위치시킨다.
	eye.x = 1.f;
	eye.y = -1.f;
	eye.z = 1.f;
	m_view = lookAt(eye, at, upVector);

	mvp = m_proj * m_view * g_cow.m_world;
	g_cow.applyMatrix(mvp, g_cow.m_world);
	g_cow.makeVeritces();
	g_cow.makeRVector(eye);
	g_cow.makeWorldCameraDepth(eye);

	//	Render한다.

	//{
	//	fill()내부에서 각 픽셀(i, j)마다 벡터(i, j, z)(z는 픽셀의 z값)를 만들고 MscreenMprojectionMlightviewMview - 1Mprojection - 1Mscreen - 1을 곱하여 shadowMap에서의 좌표(i',j', z')을 구한다. 
	//		if (shadowMap[i'][j'] < z') 
	//			shadow color를 i, j에 저장
	//		else
	//			lighted color를 i, j에 저장
	//}

	isFirstRender = true;
	render(&g_cow);
	isFirstRender = false;

	mvp = m_proj * m_view * g_floor.m_world;
	g_floor.applyMatrix(mvp, g_floor.m_world);
	g_floor.makeVeritces();
	g_floor.makeRVector(eye);
	g_floor.makeWorldCameraDepth(eye);

	iscameraRender = true;
	render(&g_floor);
	iscameraRender = false;
	//makeShadowMap();

	isDepthRender = false;
	makeShadowMap();


}

void Renderer::render(Object *pObject)
{
	for (int i = 0; i < pObject->m_nNumFace; i++)
	{
		clearEdgetable();

		if (m_isCullEnabled)
		{
			if (!isBackFace(pObject, i))
			{
				buildEdgetable(pObject, i);
				fill(pObject->m_face[i].m_color);
			}
		}
		else
		{
			buildEdgetable(pObject, i);
			fill(pObject->m_face[i].m_color);
		}
	}
}

bool Renderer::isBackFace(Object *pObject, int nFace)
{
	float normalZ = 0;
	Vector4 v1, v2, v3;

	v1.x = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[0] - 1][0];
	v1.y = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[0] - 1][1];
	v2.x = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[1] - 1][0];
	v2.y = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[1] - 1][1];
	v3.x = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[2] - 1][0];
	v3.y = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[2] - 1][1];

	normalZ = ((v2.x - v1.x) * (v3.y - v1.y)) - ((v2.y - v1.y) * (v3.x - v1.x));

	if (normalZ < 0)
	{
		return true;
	}

	else
	{
		return false;
	}

}

void Renderer::clearEdgetable()
{
	//ET 초기화
	for (int i = 0; i < checkImageHeight; i++)
	{
		m_indexCount[i] = 0;
	}

	m_numEdgeInAET = 0;
}


void Renderer::buildEdgetable(Object *pObject, int nFace)
{
	float vertices[2][3] = { 0 };

	float uvVertices[2][2] = { 0 };
	float nVertices[2][3] = { 0 };
	float dVertices[2][3] = { 0 };
	float rVertices[2][3];

	int ymin = 0;
	float savedY = 0;
	float yMax = 0;

	
	for (int i = 0; i < pObject->m_face[nFace].m_nNumVertex; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[0][j] = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[i] - 1][j];
			vertices[1][j] = pObject->m_tramsformedVertex[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];

			//nVertices[0][j] = pObject->m_faceNormal[nFace][j];
			//nVertices[1][j] = pObject->m_faceNormal[nFace][j];

			//nVertices[0][j] = m_transformedFaceNormal[pObject->m_face[nFace].m_vertex[i] - 1][j];
			//nVertices[1][j] = m_transformedFaceNormal[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];

			nVertices[0][j] = pObject->m_worldVertexNormal[pObject->m_face[nFace].m_vertex[i] - 1][j];
			nVertices[1][j] = pObject->m_worldVertexNormal[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];

			rVertices[0][j] = pObject->m_rVector[pObject->m_face[nFace].m_vertex[i] - 1][j];
			rVertices[1][j] = pObject->m_rVector[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];

			dVertices[0][j] = pObject->m_depthVector[pObject->m_face[nFace].m_vertex[i] - 1][j];
			dVertices[1][j] = pObject->m_depthVector[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];

		}

		for (int j = 0; j < 2; j++)
		{
			uvVertices[0][j] = pObject->m_uv[pObject->m_face[nFace].m_vertex[i] - 1][j];
			uvVertices[1][j] = pObject->m_uv[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];
		}

		if (vertices[0][1] == vertices[1][1])
		{
			continue;
		}

		if (vertices[0][1] < vertices[1][1]) // 작을때 ceiling 클 때 floor 
		{
			savedY = vertices[0][1];
			ymin = ceil(vertices[0][1]);
			ymin = max(ymin, 0);

			if (ymin > checkImageHeight - 1)
			{
				continue;
			}

			m_ET[ymin][m_indexCount[ymin]].xperY = (vertices[1][0] - vertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].zPerY = (vertices[1][2] - vertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].uPerY = (uvVertices[1][0] - uvVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].vPerY = (uvVertices[1][1] - uvVertices[0][1]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].nxPerY = (nVertices[1][0] - nVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nyPerY = (nVertices[1][1] - nVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nzPerY = (nVertices[1][2] - nVertices[0][2]) / (vertices[1][1] - vertices[0][1]);


			m_ET[ymin][m_indexCount[ymin]].rxPerY = (rVertices[1][0] - rVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].ryPerY = (rVertices[1][1] - rVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].rzPerY = (rVertices[1][2] - rVertices[0][2]) / (vertices[1][1] - vertices[0][1]);


			m_ET[ymin][m_indexCount[ymin]].rx = rVertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].ry = rVertices[0][1];
			m_ET[ymin][m_indexCount[ymin]].rz = rVertices[0][2];

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[1][1];

			m_ET[ymin][m_indexCount[ymin]].x = vertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].z = vertices[0][2];
			//m_ET[ymin][m_indexCount[ymin]].z = vertices[1][2];

			m_ET[ymin][m_indexCount[ymin]].u = uvVertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].v = uvVertices[0][1];

			m_ET[ymin][m_indexCount[ymin]].nx = nVertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].ny = nVertices[0][1];
			m_ET[ymin][m_indexCount[ymin]].nz = nVertices[0][2];
			//m_ET[ymin][m_indexCount[ymin]].nz = vertices[1][2];

			m_ET[ymin][m_indexCount[ymin]].lx = dVertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].ly = dVertices[0][1];
			m_ET[ymin][m_indexCount[ymin]].lz = dVertices[0][2];

			m_ET[ymin][m_indexCount[ymin]].lxPerY = (dVertices[1][0] - dVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].lyPerY = (dVertices[1][1] - dVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].lzPerY = (dVertices[1][2] - dVertices[0][2]) / (vertices[1][1] - vertices[0][1]);



	
			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].uPerY;
				m_ET[ymin][m_indexCount[ymin]].v += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].vPerY;

				m_ET[ymin][m_indexCount[ymin]].nx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nxPerY;
				m_ET[ymin][m_indexCount[ymin]].ny += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nyPerY;
				m_ET[ymin][m_indexCount[ymin]].nz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nzPerY;

				m_ET[ymin][m_indexCount[ymin]].rx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].rxPerY;
				m_ET[ymin][m_indexCount[ymin]].ry += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].ryPerY;
				m_ET[ymin][m_indexCount[ymin]].rz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].rzPerY;

				m_ET[ymin][m_indexCount[ymin]].lx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lxPerY;
				m_ET[ymin][m_indexCount[ymin]].ly += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lyPerY;
				m_ET[ymin][m_indexCount[ymin]].lz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lzPerY;
			}


		}

		else
		{
			savedY = vertices[1][1];
			ymin = ceil(vertices[1][1]);
			ymin = max(ymin, 0);

			if (ymin > checkImageHeight - 1)
			{
				continue;
			}

			m_ET[ymin][m_indexCount[ymin]].xperY = (vertices[1][0] - vertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].zPerY = (vertices[1][2] - vertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].uPerY = (uvVertices[1][0] - uvVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].vPerY = (uvVertices[1][1] - uvVertices[0][1]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].nxPerY = (nVertices[1][0] - nVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nyPerY = (nVertices[1][1] - nVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nzPerY = (nVertices[1][2] - nVertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].rxPerY = (rVertices[1][0] - rVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].ryPerY = (rVertices[1][1] - rVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].rzPerY = (rVertices[1][2] - rVertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].rx = rVertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].ry = rVertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].rz = rVertices[1][2];


			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[0][1];

			m_ET[ymin][m_indexCount[ymin]].x = vertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].z = vertices[1][2];
			//m_ET[ymin][m_indexCount[ymin]].z = vertices[0][2];

			m_ET[ymin][m_indexCount[ymin]].u = uvVertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].v = uvVertices[1][1];

			m_ET[ymin][m_indexCount[ymin]].nx = nVertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].ny = nVertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].nz = nVertices[1][2];
			//m_ET[ymin][m_indexCount[ymin]].nz = nVertices[0][2];

			m_ET[ymin][m_indexCount[ymin]].lx = dVertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].ly = dVertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].lz = dVertices[1][2];

			m_ET[ymin][m_indexCount[ymin]].lxPerY = (dVertices[1][0] - dVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].lyPerY = (dVertices[1][1] - dVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].lzPerY = (dVertices[1][2] - dVertices[0][2]) / (vertices[1][1] - vertices[0][1]);


			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].uPerY;
				m_ET[ymin][m_indexCount[ymin]].v += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].vPerY;

				m_ET[ymin][m_indexCount[ymin]].nx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nxPerY;
				m_ET[ymin][m_indexCount[ymin]].ny += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nyPerY;
				m_ET[ymin][m_indexCount[ymin]].nz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nzPerY;

				m_ET[ymin][m_indexCount[ymin]].rx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].rxPerY;
				m_ET[ymin][m_indexCount[ymin]].ry += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].ryPerY;
				m_ET[ymin][m_indexCount[ymin]].rz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].rzPerY;

				m_ET[ymin][m_indexCount[ymin]].lx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lxPerY;
				m_ET[ymin][m_indexCount[ymin]].ly += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lyPerY;
				m_ET[ymin][m_indexCount[ymin]].lz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].lzPerY;
			}

		}

		m_indexCount[ymin]++;

	}

}
float normal[10000][3];
int cnt;

void Renderer::fill(GLubyte color[3])
{
	// AET
	for (int i = 0; i < checkImageHeight; i++)
	{

		//update intersection
		for (int j = 0; j < m_numEdgeInAET; j++)
		{
			m_AET[j].x += m_AET[j].xperY;
			m_AET[j].z += m_AET[j].zPerY;

			m_AET[j].u += m_AET[j].uPerY;
			m_AET[j].v += m_AET[j].vPerY;

			m_AET[j].nx += m_AET[j].nxPerY;
			m_AET[j].ny += m_AET[j].nyPerY;
			m_AET[j].nz += m_AET[j].nzPerY;

			m_AET[j].rx += m_AET[j].rxPerY;
			m_AET[j].ry += m_AET[j].ryPerY;
			m_AET[j].rz += m_AET[j].rzPerY;

			m_AET[j].lx += m_AET[j].lxPerY;
			m_AET[j].ly += m_AET[j].lyPerY;
			m_AET[j].lz += m_AET[j].lzPerY;
		}

		//Add new edge
		for (int j = 0; j < m_indexCount[i]; j++)
		{
			m_AET[m_numEdgeInAET + j] = m_ET[i][j];
		}
		m_numEdgeInAET += m_indexCount[i];

		//Delete edge
		for (int j = 0; j < m_numEdgeInAET; j++)
		{
			if (m_AET[j].yMax < i)
			{
				for (int k = j; k < m_numEdgeInAET; k++)
				{
					m_AET[k] = m_AET[k + 1];
				}
				j--;
				m_numEdgeInAET--;
			}
		}

		//Sort intersections
		Edge temp;
		for (int j = 0; j < m_numEdgeInAET - 1; j++)
		{
			for (int k = j + 1; k < m_numEdgeInAET; k++)
			{
				if (m_AET[j].x > m_AET[k].x)
				{
					temp = m_AET[j];
					m_AET[j] = m_AET[k];
					m_AET[k] = temp;

				}
			}
		}


		//Render
		for (int j = 0; j < m_numEdgeInAET; j += 2)
		{
			int k;
			int xmin = floor(m_AET[j].x);
			int xmax = floor(m_AET[j + 1].x);
			xmin = max(xmin, 0);
			xmax = min(xmax, checkImageWidth - 1);

			float deltaU = 0;	float deltaV = 0;	float deltaZ = 0;
			float deltaNX = 0;	float deltaNY = 0;	float deltaNZ = 0;
			float uPerX = 0;	float vPerX = 0;	float zPerX = 0;
			float nxPerX = 0;	float nyPerX = 0;	float nzPerX = 0;
			float rxPerX = 0;	float ryPerX = 0;	float rzPerX = 0;

			float lxPerX = 0;	float lyPerX = 0;	float lzPerX = 0;

			//float lxPerX = 0;	float lyPerNY = 0;	float lzPerNZ = 0;
			float deltaLX = 0;	float deltaLY = 0;	float deltaLZ = 0;
			float deltaRX = 0;	float deltaRY = 0;	float deltaRZ = 0;

			//float deltaLX = 0;	float deltaLY = 0;	float deltaLZ = 0;

			float sx, sy, sz; float scala = 0;

			float rx, ry, rz, r;
			float vx, vy, vz;

			float scalaNL, scalaRV, normalN, scalaN;

			int s, t;

			uPerX = (m_AET[j + 1].u - m_AET[j].u) / (m_AET[j + 1].x - m_AET[j].x);
			vPerX = (m_AET[j + 1].v - m_AET[j].v) / (m_AET[j + 1].x - m_AET[j].x);
			zPerX = (m_AET[j + 1].z - m_AET[j].z) / (m_AET[j + 1].x - m_AET[j].x);

			nxPerX = (m_AET[j + 1].nx - m_AET[j].nx) / (m_AET[j + 1].x - m_AET[j].x);
			nyPerX = (m_AET[j + 1].ny - m_AET[j].ny) / (m_AET[j + 1].x - m_AET[j].x);
			nzPerX = (m_AET[j + 1].nz - m_AET[j].nz) / (m_AET[j + 1].x - m_AET[j].x);

			rxPerX = (m_AET[j + 1].rx - m_AET[j].rx) / (m_AET[j + 1].x - m_AET[j].x);
			ryPerX = (m_AET[j + 1].ry - m_AET[j].ry) / (m_AET[j + 1].x - m_AET[j].x);
			rzPerX = (m_AET[j + 1].rz - m_AET[j].rz) / (m_AET[j + 1].x - m_AET[j].x);

			lxPerX = (m_AET[j + 1].lx - m_AET[j].lx) / (m_AET[j + 1].x - m_AET[j].x);
			lyPerX = (m_AET[j + 1].ly - m_AET[j].ly) / (m_AET[j + 1].x - m_AET[j].x);
			lzPerX = (m_AET[j + 1].lz - m_AET[j].lz) / (m_AET[j + 1].x - m_AET[j].x);

			
			for (k = xmin; k < xmax; k++)
			{
				Vector4 Normal(m_AET[j].nx + deltaNX, m_AET[j].ny + deltaNY, m_AET[j].nz + deltaNZ);

				float light = Normal * ndirectionLight;
				light = max(0.f, light * 255);


				sx = (m_AET[j].nx + deltaNX) * (ndirectionLight.x);
				sy = (m_AET[j].ny + deltaNY) * (ndirectionLight.y);
				sz = (m_AET[j].nz + deltaNZ) * (ndirectionLight.z);

				scalaNL = sx + sy + sz;

				if (m_AET[j].z + deltaZ < m_zBuffer[i][k])
				{
					//checkImage[i][k][0] = max(scalaNL * 255, 0.f);
					//checkImage[i][k][1] = max(scalaNL * 255, 0.f);
					//checkImage[i][k][2] = max(scalaNL * 255, 0.f);


					int s, t;
					Vector4 r;
					r.x = m_AET[j].rx + deltaRX;
					r.y = m_AET[j].ry + deltaRY;
					r.z = m_AET[j].rz + deltaRZ;

					scalaN = sqrt(pow(r.x, 2) + pow(r.y, 2) + pow(r.z + 1, 2));

					s = (r.x / scalaN + 1) * 320;
					t = (r.y / scalaN + 1) * 240;

					if (isFirstRender)
					{
						//checkImage[i][k][0] = (GLubyte)(m_texture[t][s][0]);
						//checkImage[i][k][1] = (GLubyte)(m_texture[t][s][1]);
						//checkImage[i][k][2] = (GLubyte)(m_texture[t][s][2]);

						//checkImage[i][k][0] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][0] * max(scalaNL, 0.f);
						//checkImage[i][k][1] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][1] * max(scalaNL, 0.f);
						//checkImage[i][k][2] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][2] * max(scalaNL, 0.f);

						//checkImage[i][k][0] = (GLubyte)((r.x + 1) * 128);
						//checkImage[i][k][1] = (GLubyte)((r.y + 1) * 128);
						//checkImage[i][k][2] = (GLubyte)((r.z + 1) * 128);

						checkImage[i][k][0] = (GLubyte)m_AET[j].lx;
						checkImage[i][k][1] = (GLubyte)m_AET[j].ly;
						checkImage[i][k][2] = (GLubyte)m_AET[j].lz;


						m_zBuffer[i][k] = m_AET[j].z + deltaZ;
					}

					else
					{

						if (floorWorldCameraDepthBuffer[i][k] < m_zBuffer[i][k])
						{
							checkImage[i][k][0] = (GLubyte)m_AET[j].lx;
							checkImage[i][k][1] = (GLubyte)m_AET[j].ly;
							checkImage[i][k][2] = (GLubyte)m_AET[j].lz;
						}

						//else
						//{
						//	checkImage[i][k][0] = (GLubyte)m_AET[j].lx;
						//	checkImage[i][k][1] = (GLubyte)m_AET[j].ly;
						//	checkImage[i][k][2] = (GLubyte)m_AET[j].lz;
						//}

						
						else
						{

							checkImage[i][k][0] = (GLubyte)0;
							checkImage[i][k][1] = (GLubyte)0;
							checkImage[i][k][2] = (GLubyte)0;

						}


					}



					
				}


				deltaU += uPerX;
				deltaV += vPerX;
				deltaZ += zPerX;

				deltaNX += nxPerX;
				deltaNY += nyPerX;
				deltaNZ += nzPerX;

				deltaRX += rxPerX;
				deltaRY += ryPerX;
				deltaRZ += rzPerX;

				deltaLX += lxPerX;
				deltaLY += lyPerX;
				deltaLZ += lzPerX;


			}
			
		}
	}
}


void Renderer::makeShadowMap()
{
	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			//floorWorldLightingDepthBuffer[i][j] = m_zBuffer[i][j];
			if (isDepthRender)
			{
				floorWorldLightingDepthBuffer[i][j] = m_zBuffer[i][j];
			}

			else
			{
				floorWorldCameraDepthBuffer[i][j] = m_zBuffer[i][j];
			}
		
		}
	}






}
