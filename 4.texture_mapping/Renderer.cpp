#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include<iostream>
#include<vector>
#include <cmath>
#include <time.h>

#include "Renderer.h"
#include"Vector4.h"
#include"Matrix4.h"

#include <string.h>
#include <stdlib.h>

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
			texture[i][j][0] = (GLubyte)input_data[i][j][0];
			texture[i][j][1] = (GLubyte)input_data[i][j][1];
			texture[i][j][2] = (GLubyte)input_data[i][j][2];
		}
	}
}

void Renderer::makeUVVertex()
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
			zBuffer[i][j] = 1;
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


void Renderer::render()
{
	if (!texureMappingEnabled)
	{
		clock_t start = clock();

		clearCheckImage();
		clearZBuffer();

		clock_t end = clock();
		printf("clearCheckImage Time: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);
		//Vector4 eye(0.0f, 0.0f, -1.5f);
		//Vector4 at(0.0f, 0.0f, 0.0f);
		//Vector4 upVector(0.0f, 1.0f, 0.0f);

		//m_view = lookAt(eye, at, upVector);
		//m_proj = projectMatrix(90.0f, 1.3333f, 1.0f, 150.0f);
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

			if (isCullEnabled)
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
		for (int j = 0; j < 1000; j++)
		{
			m_ET[i][j].inverseOfSlope = 0;
			m_ET[i][j].x = 0;
			m_ET[i][j].yMax = 0;
			m_indexCount[i] = 0;
		}
	}

	m_numEdgeInAET = 0;
}



void Renderer::buildEdgetable(int nFace)
{
	float vertices[2][3];
	int ymin;
	float yMax, x, inverseOfSlope, z, zPerY;
	
	for (int i = 0; i < m_face[nFace].m_nNumVertex; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[0][j] = m_tramsformedVertex[m_face[nFace].m_vertex[i] - 1][j];
			vertices[1][j] = m_tramsformedVertex[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex] - 1][j];
		}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		float uvVertices[2][2];
		float u, v, uPerY, vPerY;

		for (int j = 0; j < 2; j++)
		{
			uvVertices[0][j] = m_uv[m_face[nFace].m_vertex[i] - 1][j];
			uvVertices[1][j] = m_uv[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex] - 1][j];
		}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if (vertices[0][1] == vertices[1][1])
		{
			continue;
		}
		else
		{
			inverseOfSlope = (vertices[1][0] - vertices[0][0]) / (vertices[1][1] - vertices[0][1]); //xperY
		}

		zPerY = (vertices[1][2] - vertices[0][2]) / (vertices[1][1] - vertices[0][1]);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		uPerY = (uvVertices[1][0] - uvVertices[0][0]) / (vertices[1][1] - vertices[0][1]); //uperY
		vPerY = (uvVertices[1][1] - uvVertices[0][1]) / (vertices[1][1] - vertices[0][1]);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		float savedY;
		float savedV;

		if (vertices[0][1] < vertices[1][1]) // 작을때 ceiling 클 때 floor 
		{
			savedY = vertices[0][1];

			//if ((int)vertices[0][1] == vertices[0][1])
			//{
			//	vertices[0][1] += 1;
			//}
			ymin = ceil(vertices[0][1]);
			ymin = max(ymin, 0);

			if (ymin > checkImageHeight - 1)
			{
				continue;
			}

			m_ET[ymin][m_indexCount[ymin]].x = vertices[0][0];

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * inverseOfSlope;
			}

			m_ET[ymin][m_indexCount[ymin]].zperY = zPerY;

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].inverseOfSlope = inverseOfSlope;
			m_ET[ymin][m_indexCount[ymin]].z = vertices[1][2];
			


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			savedV = uvVertices[0][1];
			if (savedY != 0)
			{
				v = ymin * uvVertices[0][1] / savedY;
			}
			else
			{
				v = 0;
			}


			m_ET[ymin][m_indexCount[ymin]].u = uvVertices[0][0];

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * uPerY;
			}

			m_ET[ymin][m_indexCount[ymin]].v = v;
			m_ET[ymin][m_indexCount[ymin]].uPerY = uPerY;
			m_ET[ymin][m_indexCount[ymin]].vperY = vPerY;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m_indexCount[ymin]++;

		}
		else
		{
			savedY = vertices[1][1];

			//if ((int)vertices[1][1] == vertices[1][1])
			//{
			//	vertices[1][1] += 1;
			//}
			ymin = ceil(vertices[1][1]);
			ymin = max(ymin, 0);

			if (ymin > checkImageHeight - 1)
			{
				continue;
			}

			m_ET[ymin][m_indexCount[ymin]].x = vertices[1][0];

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * inverseOfSlope;
			}
			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[0][1];  
			m_ET[ymin][m_indexCount[ymin]].inverseOfSlope = inverseOfSlope;
			m_ET[ymin][m_indexCount[ymin]].z = vertices[0][2];



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			savedV = uvVertices[1][1];
			if (savedY != 0)
			{
				v = ymin * uvVertices[1][1] / savedY;
			}
			else
			{
				v = 0;
			}

			m_ET[ymin][m_indexCount[ymin]].u = uvVertices[1][0];

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].u += (ymin - savedY) * uPerY;
			}

			m_ET[ymin][m_indexCount[ymin]].v = v;
			m_ET[ymin][m_indexCount[ymin]].uPerY = uPerY;
			m_ET[ymin][m_indexCount[ymin]].vperY = vPerY;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m_indexCount[ymin]++;
		}
	}

}


void Renderer::fill(GLubyte color[3])
{
	// AET
	for (int i = 0; i < checkImageHeight; i++)
	{

		//update intersection
		for (int j = 0; j < m_numEdgeInAET; j++)
		{
			m_AET[j].x += m_AET[j].inverseOfSlope;

			m_AET[j].z += m_AET[j].zperY;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			m_AET[j].u += m_AET[j].uPerY;
			m_AET[j].v += m_AET[j].vperY;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

			//if (m_AET[j].x == (int)m_AET[j].x)
			//{
			//	xmin += 1;
			//}

			xmin = max(xmin, 0);
			xmax = min(xmax, checkImageWidth - 1);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			float uPerX = (m_AET[j + 1].u - m_AET[j].u) / (xmax - xmin);
			float deltaX = 0;

			float vPerX = (m_AET[j + 1].v - m_AET[j].v) / (xmax - xmin);
			float deltaV = 0;

			float zPerX = (m_AET[j + 1].z - m_AET[j].z) / (xmax - xmin);
			float deltaZ = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




			for (k = xmin; k < xmax; k++)
			{
				//checkImage[i][k][0] = (GLubyte)color[0];
				//checkImage[i][k][1] = (GLubyte)color[1];
				//checkImage[i][k][2] = (GLubyte)color[2];

				/*checkImage[i][k][0] = (GLubyte)texture[(int)(m_AET[j].v + vperXcnt)][(int)(m_AET[j].u + uperXcnt)][0];
				checkImage[i][k][1] = (GLubyte)texture[(int)(m_AET[j].v + vperXcnt)][(int)(m_AET[j].u + uperXcnt)][1];
				checkImage[i][k][2] = (GLubyte)texture[(int)(m_AET[j].v + vperXcnt)][(int)(m_AET[j].u + uperXcnt)][2];*/

				if (m_AET[j].z + deltaZ < zBuffer[i][k])
				{
					checkImage[i][k][0] = (GLubyte)texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaX)][0];
					checkImage[i][k][1] = (GLubyte)texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaX)][1];
					checkImage[i][k][2] = (GLubyte)texture[(int)(m_AET[j].v + deltaV)][(int)(m_AET[j].u + deltaX)][2];

					zBuffer[i][k] = m_AET[j].z + deltaZ;
					deltaX += uPerX;
					deltaV += vPerX;
					deltaZ += zPerX;
				}

				//else
				//	zBuffer[i][j] = m_AET[j].z + zperXcnt;
				//	zperXcnt += zperX;
				//else
				//{
				//	checkImage[i][k][0] = (GLubyte)255;
				//	checkImage[i][k][1] = (GLubyte)0;
				//	checkImage[i][k][2] = (GLubyte)0;
				//}
	

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
