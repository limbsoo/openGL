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


void Renderer::clearCheckImage()
{
	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			checkImage[i][j][0] = (GLubyte)0;
			checkImage[i][j][1] = (GLubyte)0;
			checkImage[i][j][2] = (GLubyte)0;
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

Vector4 lightPosition(2.f, -1.f, 2.f);
Vector4 ndirectionLight = normalize(lightPosition);

void Renderer::render()
{
	clearCheckImage();
	clearZBuffer();


	//Render한다.
	mvp = m_proj * m_view * g_cow.m_world;
	g_cow.applyMatrix(mvp, g_cow.m_world);
	g_cow.makeVeritces();
	//g_cow.makeRVector(eye);
	//g_cow.makeWorldLightingDepth(lightPosition);

	render(&g_cow);

}

void Renderer::render(Object* pObject)
{
	for (int i = 0; i < pObject->m_nNumFace; i++)
	{
		clearEdgetable();
		buildEdgetable(pObject, i);
		fill(pObject->m_face[i].m_color);

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

			nVertices[0][j] = pObject->m_worldVertexNormal[pObject->m_face[nFace].m_vertex[i] - 1][j];
			nVertices[1][j] = pObject->m_worldVertexNormal[pObject->m_face[nFace].m_vertex[(i + 1) % pObject->m_face[nFace].m_nNumVertex] - 1][j];


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

			m_ET[ymin][m_indexCount[ymin]].nxPerY = (nVertices[1][0] - nVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nyPerY = (nVertices[1][1] - nVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nzPerY = (nVertices[1][2] - nVertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[1][1];

			m_ET[ymin][m_indexCount[ymin]].x = vertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].z = vertices[0][2];

			m_ET[ymin][m_indexCount[ymin]].nx = nVertices[0][0];
			m_ET[ymin][m_indexCount[ymin]].ny = nVertices[0][1];
			m_ET[ymin][m_indexCount[ymin]].nz = nVertices[0][2];




	
			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].nx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nxPerY;
				m_ET[ymin][m_indexCount[ymin]].ny += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nyPerY;
				m_ET[ymin][m_indexCount[ymin]].nz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nzPerY;

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

			m_ET[ymin][m_indexCount[ymin]].nxPerY = (nVertices[1][0] - nVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nyPerY = (nVertices[1][1] - nVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nzPerY = (nVertices[1][2] - nVertices[0][2]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[0][1];

			m_ET[ymin][m_indexCount[ymin]].x = vertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].z = vertices[1][2];

			m_ET[ymin][m_indexCount[ymin]].nx = nVertices[1][0];
			m_ET[ymin][m_indexCount[ymin]].ny = nVertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].nz = nVertices[1][2];


			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].nx += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nxPerY;
				m_ET[ymin][m_indexCount[ymin]].ny += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nyPerY;
				m_ET[ymin][m_indexCount[ymin]].nz += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].nzPerY;

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

			m_AET[j].nx += m_AET[j].nxPerY;
			m_AET[j].ny += m_AET[j].nyPerY;
			m_AET[j].nz += m_AET[j].nzPerY;

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

			zPerX = (m_AET[j + 1].z - m_AET[j].z) / (m_AET[j + 1].x - m_AET[j].x);

			nxPerX = (m_AET[j + 1].nx - m_AET[j].nx) / (m_AET[j + 1].x - m_AET[j].x);
			nyPerX = (m_AET[j + 1].ny - m_AET[j].ny) / (m_AET[j + 1].x - m_AET[j].x);
			nzPerX = (m_AET[j + 1].nz - m_AET[j].nz) / (m_AET[j + 1].x - m_AET[j].x);

			for (k = xmin; k < xmax; k++)
			{
				float noramlX = (m_AET[j].nx + 1) * 127;
				float noramlY = (m_AET[j].ny + 1) * 127;
				float noramlZ = (m_AET[j].nz + 1) * 127;

				if (m_AET[j].z + deltaZ < m_zBuffer[i][k])
				{
					//checkImage[i][k][0] = (GLubyte)255;
					//checkImage[i][k][1] = (GLubyte)255;
					//checkImage[i][k][2] = (GLubyte)255;

					checkImage[i][k][0] = (GLubyte)noramlX;
					checkImage[i][k][1] = (GLubyte)noramlY;
					checkImage[i][k][2] = (GLubyte)noramlZ;

				}

				m_zBuffer[i][k] = m_AET[j].z + deltaZ;


				deltaZ += zPerX;

				deltaNX += nxPerX;
				deltaNY += nyPerX;
				deltaNZ += nzPerX;




			}
			
		}
	}








}
