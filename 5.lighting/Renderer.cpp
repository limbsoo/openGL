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

void Renderer::readFile(char* pFileName)
{
	char buff[512], buff2[512];
	int i, j;

	FILE* pFile = fopen(pFileName, "rt");
	getNewLine(buff, 512, pFile);
	sscanf(buff, "%s %d", buff2, &m_nNumVertex);
	ASSERT(stricmp(buff2, "$Vertex") == 0);

	getNewLine(buff, 512, pFile);
	sscanf(buff, "%s %d", buff2, &m_nNumFace);
	ASSERT(stricmp(buff2, "$Faces") == 0);


	for (i = 0; i < m_nNumVertex; i++)
	{
		int nNum;
		getNewLine(buff, 512, pFile);
		sscanf(buff, "%s %d %f %f %f", buff2, &nNum, &m_vertex[i][0], &m_vertex[i][1], &m_vertex[i][2]);
		sscanf(buff, "%s ", buff2);

		for (int j = 0; j < 3; j++)								//cow
		m_vertex[i][j] = (m_vertex[i][j]) ;	//cow

		ASSERT(stricmp(buff2, "Vertex") == 0);
		ASSERT((nNum - 1) == i);
	}


	for (i = 0; i < m_nNumFace; i++)
	{
		int nNum;
		int nCurrPos;

		getNewLine(buff, 512, pFile);
		//sscanf(buff, "%s %d %d %d %d %d", buff2, &nNum, &m_face[i].m_color[0], &m_face[i].m_color[1], &m_face[i].m_color[2], &m_face[i].m_nNumVertex);  //triangle
		
		sscanf(buff, "%s %d", buff2, &nNum);		//cow
		m_face[i].m_nNumVertex = 3;					//cow

		m_face[i].m_color[0] = (GLubyte)0;			//cow
		m_face[i].m_color[1] = (GLubyte)0;			//cow
		m_face[i].m_color[2] = (GLubyte)255;		//cow

		ASSERT(stricmp(buff2, "Face") == 0);
		ASSERT((nNum - 1) == i);
		nCurrPos = 0;
		//for (j = 0; j < 5; j++)				//triangle
		for (j = 0; j < 1; j++)						//cow
		{
			nCurrPos += strcspn(buff + nCurrPos, " \t");
			nCurrPos += strspn(buff + nCurrPos, " \t");
		}
		for (j = 0; j < m_face[i].m_nNumVertex; j++)
		{
			nCurrPos += strcspn(buff + nCurrPos, " \t");
			nCurrPos += strspn(buff + nCurrPos, " \t");
			sscanf(buff + nCurrPos, "%d", &m_face[i].m_vertex[j]);
			
			
		}
	}


}

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

void Renderer ::makefaceNormals() //외적 
{
	for (int i = 0; i < m_nNumFace; i++) // 각 face의 normal
	{
		Vector4 v1, v2;

		v1.x = m_vertex[m_face[i].m_vertex[0] - 1][0] - m_vertex[m_face[i].m_vertex[1] - 1][0];
		v1.y = m_vertex[m_face[i].m_vertex[0] - 1][1] - m_vertex[m_face[i].m_vertex[1] - 1][1];

		v1.z = m_vertex[m_face[i].m_vertex[0] - 1][2] - m_vertex[m_face[i].m_vertex[1] - 1][2];

		v2.x = m_vertex[m_face[i].m_vertex[1] - 1][0] - m_vertex[m_face[i].m_vertex[2] - 1][0];
		v2.y = m_vertex[m_face[i].m_vertex[1] - 1][1] - m_vertex[m_face[i].m_vertex[2] - 1][1];

		v2.z = m_vertex[m_face[i].m_vertex[1] - 1][2] - m_vertex[m_face[i].m_vertex[2] - 1][2];

		Vector4 ret;

		ret = crossProduct(v1, v2);
		ret = normalize(ret);

		m_faceNormal[i][0] = ret.x;
		m_faceNormal[i][1] = ret.y;
		m_faceNormal[i][2] = ret.z;
	}

}

void Renderer::makeVertexNormals()
{
	for (int i = 0; i < m_nNumFace; i++) // //점의 노말은 점이 속한 삼각형 노말의 평균
	{
		for (int j = 0; j < 3; j++)
		{  
			m_vertexNormal[m_face[i].m_vertex[j] - 1][0] += m_faceNormal[i][0];
			m_vertexNormal[m_face[i].m_vertex[j] - 1][1] += m_faceNormal[i][1];
			m_vertexNormal[m_face[i].m_vertex[j] - 1][2] += m_faceNormal[i][2];
		}
	}

	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v;

		v.x = m_vertexNormal[i][0];
		v.y = m_vertexNormal[i][1];
		v.z = m_vertexNormal[i][2];
		
		v = normalize(v);

		m_vertexNormal[i][0] = v.x;
		m_vertexNormal[i][1] = v.y;
		m_vertexNormal[i][2] = v.z;
	}

}


void Renderer::makeVertexUVs()
{
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v(m_vertex[i], 1);
		m_uv[i][0] = (v.x + 1) * 320;
		m_uv[i][1] = (v.y + 1) * 240;
	}
}

void Renderer::clearCheckImage()
{
	for (int i = 0; i < checkImageHeight; i++)
	{
		for (int j = 0; j < checkImageWidth; j++)
		{
			checkImage[i][j][0] = (GLubyte)0;
			checkImage[i][j][1] = (GLubyte)255;
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


void Renderer::applyMatrix(Matrix4 m, Matrix4 n)
{
	Matrix4 m1 = identityMatrix();
	m1.arr[0][0] = 320;
	m1.arr[0][3] = 320;
	m1.arr[1][1] = 240;
	m1.arr[1][3] = 240;
	
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v(m_vertex[i], 1);

		v = m * v;
		v = m1 * v;

		m_tramsformedVertex[i][0] = v.x / v.w;
		m_tramsformedVertex[i][1] = v.y / v.w;
		m_tramsformedVertex[i][2] = v.z / v.w;
	}
}

bool makeNormals = true;

void Renderer::render()
{
	if (!m_texureMappingEnabled)
	{
		clock_t start = clock();

		clearCheckImage();
		clearZBuffer();

		clock_t end = clock();
		printf("clearCheckImage Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
		start = clock();

		Matrix4 mvp = m_proj * m_view * m_world; //m_world 부터 역순으로 vector에 곱함


		end = clock();
		printf("Matrix4 mvp Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
		start = clock();

		applyMatrix(mvp, m_world);
		clearEdgetable();

		end = clock();
		printf("applyMatrix Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
		start = clock();


		for (int i = 0; i < m_nNumFace; i++)
		{
			clearEdgetable();

			if (m_isCullEnabled)
			{
				if (!isBackFace(i))
				{
					buildEdgetable(i);
					fill(m_face[i].m_color);
				}
			}
			else
			{
				buildEdgetable(i);
				fill(m_face[i].m_color);
			}
		}
		end = clock();
		printf("fill Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
	}
}

bool Renderer::isBackFace(int nFace)
{
	float normalZ = 0;
	Vector4 v1, v2, v3;
	//return false;

	v1.x = m_tramsformedVertex[m_face[nFace].m_vertex[0] - 1][0];
	v1.y = m_tramsformedVertex[m_face[nFace].m_vertex[0] - 1][1];
	v2.x = m_tramsformedVertex[m_face[nFace].m_vertex[1] - 1][0];
	v2.y = m_tramsformedVertex[m_face[nFace].m_vertex[1] - 1][1];
	v3.x = m_tramsformedVertex[m_face[nFace].m_vertex[2] - 1][0];
	v3.y = m_tramsformedVertex[m_face[nFace].m_vertex[2] - 1][1];

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
		//for (int j = 0; j < 1000; j++)
		//{
		//	m_ET[i][j].xperY = 0;
		//	m_ET[i][j].x = 0;
		//	m_ET[i][j].yMax = 0;
		//	

		//	//m_ET[i][j].nx = 0;
		//	//m_ET[i][j].ny = 0;
		//	//m_ET[i][j].nz = 0;

		//	//m_ET[i][j].u = 0;
		//	//m_ET[i][j].v = 0;
		//	//m_ET[i][j].uPerY = 0;
		//	//m_ET[i][j].vPerY = 0;

		//}
	}

	m_numEdgeInAET = 0;
}

Vector4 directionLight(5.f, 5.f, 5.f);

Vector4 ndirectionLight = normalize(directionLight);

void Renderer::buildEdgetable(int nFace)
{
	float vertices[2][3] = { 0 };

	float uvVertices[2][2] = { 0 };
	float nVertices[2][3] = { 0 };
	float lightVertices[2][3] = { 0 };

	int ymin = 0;
	float savedY = 0;
	float yMax = 0;

	//pointLightPosition.x = 10.0f;
	//pointLightPosition.y = 10.0f;
	//pointLightPosition.z = 10.0f;

	
	for (int i = 0; i < m_face[nFace].m_nNumVertex; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[0][j] = m_tramsformedVertex[m_face[nFace].m_vertex[i] - 1][j];
			vertices[1][j] = m_tramsformedVertex[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex] - 1][j];

			nVertices[0][j] = m_faceNormal[nFace][j];
			nVertices[1][j] = m_faceNormal[nFace][j];



			//nVertices[1][j] = m_faceNormal[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex] - 1][j];
		}

		for (int j = 0; j < 2; j++)
		{
			uvVertices[0][j] = m_uv[m_face[nFace].m_vertex[i] - 1][j];
			uvVertices[1][j] = m_uv[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex] - 1][j];
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
	
			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].uPerY;
				m_ET[ymin][m_indexCount[ymin]].v += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].vPerY;

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

			m_ET[ymin][m_indexCount[ymin]].uPerY = (uvVertices[1][0] - uvVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].vPerY = (uvVertices[1][1] - uvVertices[0][1]) / (vertices[1][1] - vertices[0][1]);

			m_ET[ymin][m_indexCount[ymin]].nxPerY = (nVertices[1][0] - nVertices[0][0]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nyPerY = (nVertices[1][1] - nVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
			m_ET[ymin][m_indexCount[ymin]].nzPerY = (nVertices[1][2] - nVertices[0][2]) / (vertices[1][1] - vertices[0][1]);


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

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].xperY;
				m_ET[ymin][m_indexCount[ymin]].z += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].zPerY;

				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].uPerY;
				m_ET[ymin][m_indexCount[ymin]].v += (ymin - savedY) * m_ET[ymin][m_indexCount[ymin]].vPerY;

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

			m_AET[j].u += m_AET[j].uPerY;
			m_AET[j].v += m_AET[j].vPerY;

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

			float lxPerX = 0;	float lyPerNY = 0;	float lzPerNZ = 0;
			float deltaLX = 0;	float deltaLY = 0;	float deltaLZ = 0;

			float sx, sy, sz; float scala = 0;

			uPerX = (m_AET[j + 1].u - m_AET[j].u) / (m_AET[j + 1].x - m_AET[j].x);
			vPerX = (m_AET[j + 1].v - m_AET[j].v) / (m_AET[j + 1].x - m_AET[j].x);
			zPerX = (m_AET[j + 1].z - m_AET[j].z) / (m_AET[j + 1].x - m_AET[j].x);

			nxPerX = (m_AET[j + 1].nx - m_AET[j].nx) / (m_AET[j + 1].x - m_AET[j].x);
			nyPerX = (m_AET[j + 1].ny - m_AET[j].ny) / (m_AET[j + 1].x - m_AET[j].x);
			nzPerX = (m_AET[j + 1].nz - m_AET[j].nz) / (m_AET[j + 1].x - m_AET[j].x);


			
			for (k = xmin; k < xmax; k++)
			{
				sx = (m_AET[j].nx + deltaNX) * (ndirectionLight.x);
				sy = (m_AET[j].ny + deltaNY) * (ndirectionLight.y);
				sz = (m_AET[j].nz + deltaNZ) * (ndirectionLight.z);

				scala = sx + sy + sz;

				if (m_AET[j].z + deltaZ < m_zBuffer[i][k])
				{
					//checkImage[i][k][0] = max(scala * 255,0.f);
					//checkImage[i][k][1] = max(scala * 255, 0.f);
					//checkImage[i][k][2] = max(scala * 255, 0.f);

					checkImage[i][k][0] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][0] * max(scala, 0.f);
					checkImage[i][k][1] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][1] * max(scala, 0.f);
					checkImage[i][k][2] = (GLubyte)m_texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaU)][2] * max(scala, 0.f);

					m_zBuffer[i][k] = m_AET[j].z + deltaZ;
				}


				deltaU += uPerX;
				deltaV += vPerX;
				deltaZ += zPerX;

				deltaNX += nxPerX;
				deltaNY += nyPerX;
				deltaNZ += nzPerX;


			}
			
		}
	}
}

void Renderer::getNewLine(char* buff, int count, FILE* pFile)
{
	fgets(buff, count, pFile);

	while (buff[0] == '#')
		VERIFY(fgets(buff, count, pFile));
}
