#include<iostream>
#include<vector>
#include <cmath>

#include "Renderer.h"
//#include"Vector4.h"
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

		for (int j = 0; j < 3; j++)
			m_vertex[i][j] = (m_vertex[i][j] + 1) / 2.0f * 640.f;

		ASSERT(stricmp(buff2, "Vertex") == 0);
		ASSERT((nNum - 1) == i);
	}


	for (i = 0; i < m_nNumFace; i++)
	{
		int nNum;
		int nCurrPos;

		getNewLine(buff, 512, pFile);
		//sscanf(buff, "%s %d %d %d %d %d", buff2, &nNum, &m_face[i].m_color[0], &m_face[i].m_color[1], &m_face[i].m_color[2], &m_face[i].m_nNumVertex);
		sscanf(buff, "%s %d", buff2, &nNum);
		m_face[i].m_nNumVertex = 3;

		m_face[i].m_color[0] = (GLubyte)0;
		m_face[i].m_color[1] = (GLubyte)0;
		m_face[i].m_color[2] = (GLubyte)255;

		ASSERT(stricmp(buff2, "Face") == 0);
		ASSERT((nNum - 1) == i);
		nCurrPos = 0;
		for (j = 0; j < 1; j++)
		{
			nCurrPos += strcspn(buff + nCurrPos, " \t");
			nCurrPos += strspn(buff + nCurrPos, " \t");
		}
		for (j = 0; j < m_face[i].m_nNumVertex; j++)
		{
			nCurrPos += strcspn(buff + nCurrPos, " \t");
			nCurrPos += strspn(buff + nCurrPos, " \t");
			//sscanf(buff + nCurrPos, "%d", &m_face[i].m_vertex[j]);
			sscanf(buff + nCurrPos, "%d", &m_face[i].m_vertex[j]);
			//m_face[i].m_vertex[j] = buff[19];
			
		}
	}


}

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


void Renderer::render()
{


	for (int i = 0; i < m_nNumFace; i++)
	{
		clearEdgetable();
		buildEdgetable(i);
		fill(m_face[i].m_color);
	
		//translate(i);
	}

}


//bool istrue = true;
//int nFaceCount = 0;

void Renderer::buildEdgetable(int nFace)
{
	float vertices[2][3];
	int ymin;
	float flooringNum;
	float yMax, x, inverseOfSlope,z;

	//if (istrue)
	//{
	//	for (int i = 0; i < NUM_MAX_VERTEX; i++)
	//	{
	//		for (int j = 0; j < 3; j++)
	//		{
	//			m_tramsformedVertex[i][j] = m_vertex[i][j];
	//		}
	//	}
	//	istrue = false;
	//}

	
	for (int i = 0; i < m_face[nFace].m_nNumVertex; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			vertices[0][j] = m_tramsformedVertex[m_face[nFace].m_vertex[i]-1][j];
			vertices[1][j] = m_tramsformedVertex[m_face[nFace].m_vertex[(i + 1) % m_face[nFace].m_nNumVertex]-1][j];
		}

		if (vertices[0][1] == vertices[1][1])
		{
			continue;
		}
		else
			inverseOfSlope = (vertices[1][0] - vertices[0][0]) / (vertices[1][1] - vertices[0][1]);

		float savedY;

		if (vertices[0][1] < vertices[1][1]) // 작을때 ceiling 클 때 floor 
		{
			savedY = vertices[0][1];
			if ((int)vertices[0][1] == vertices[0][1])
			{
				vertices[0][1] += 1;
			}
			ymin = ceil(vertices[0][1]);

			ymin = max(ymin, 0);
			ymin = min(ymin, checkImageHeight - 1);
			//ymin = min(ymin, checkImageHeight);



			m_ET[ymin][m_indexCount[ymin]].x = vertices[0][0];

			if (ymin - savedY != 0)
			{
				m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * inverseOfSlope;
			}

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[1][1];
			m_ET[ymin][m_indexCount[ymin]].inverseOfSlope = inverseOfSlope;
			m_ET[ymin][m_indexCount[ymin]].z = vertices[1][2];
			m_indexCount[ymin]++;

		}
		else
		{
			savedY = vertices[1][1];
			if ((int)vertices[1][1] == vertices[1][1])
			{
				vertices[1][1] += 1;
			}
			ymin = ceil(vertices[1][1]);

			ymin = max(ymin, 0);
			ymin = min(ymin, checkImageHeight - 1);
			//ymin = min(ymin, checkImageHeight);

			m_ET[ymin][m_indexCount[ymin]].x = vertices[1][0];

			m_ET[ymin][m_indexCount[ymin]].x += (ymin - savedY) * inverseOfSlope;

			m_ET[ymin][m_indexCount[ymin]].yMax = vertices[0][1];
			m_ET[ymin][m_indexCount[ymin]].inverseOfSlope = inverseOfSlope;
			m_ET[ymin][m_indexCount[ymin]].z = vertices[0][2];
			m_indexCount[ymin]++;
		}
	}

}

void Renderer::fill(GLubyte color[3])
{
	//istrue = true;
	// AET
	for (int i = 0; i < checkImageHeight; i++)
	{
		//update intersection
		for (int j = 0; j < m_numEdgeInAET; j++)
		{
			m_AET[j].x += m_AET[j].inverseOfSlope;
			//ASSERT(0 <= j && j < 200);
		}

		//Add new edge

		for (int j = 0; j < m_indexCount[i]; j++)
		{
			m_AET[m_numEdgeInAET + j] = m_ET[i][j];
			//ASSERT(0 <= m_numEdgeInAET + j && j + m_numEdgeInAET < 200);
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
					//ASSERT(0 <= k && k < 200);
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
			int xmin = ceil(m_AET[j].x);
			int xmax = floor(m_AET[j + 1].x);

			if (m_AET[j].x == (int)m_AET[j].x)
			{
				xmin += 1;
			}

			//if(k> checkImageWidth)

			xmin = max(xmin, 0);
			xmax = min(xmax, checkImageWidth - 1);
			//xmax = min(xmax, checkImageWidth);

			for (k = xmin; k < xmax; k++)
			{
				checkImage[i][k][0] = (GLubyte)color[0];
				checkImage[i][k][1] = (GLubyte)color[1];
				checkImage[i][k][2] = (GLubyte)color[2];
			}
		}
	}
}




void Renderer::applyMatrix(Matrix4 m, Matrix4 n)
{
	for (int i = 0; i < m_nNumVertex; i++)
	{

		int a = 3;
		a = 4;
		//Vector4 v(m_vertex[i], 1);
		//v = m * v;

		Vector4 v(m_vertex[i], 1);
		Vector4 w;

		w = (m * n) * v;

		//n = m * n;
		//w = n * v;

		//currentMatrix() = m * n;;

		m_matrix = m * n;

		//v = m * v;
		//vector 값은초기 값으로
		// matrix는 곱한값으로

		m_tramsformedVertex[i][0] = w.x;
		m_tramsformedVertex[i][1] = w.y;
		m_tramsformedVertex[i][2] = w.z;

		//m_vertex[i][1] = v.y;
		//m_vertex[i][2] = v.z;

	}
}

void Renderer::scale(float x, float y, float z) // Matrix build
{
	//perserve v
	applyMatrix(scaleMatrix(x, y, z), m_matrix);
}

void Renderer::translate(float x, float y, float z) // Matrix build
{
	applyMatrix(translateMatrix(x,y,z), m_matrix);
}

void Renderer::rotateX(float angle) // Matrix build
{
	applyMatrix(xRotateMatrix(angle), m_matrix);
}

void Renderer::rotateY(float angle)
{
	applyMatrix(yRotateMatrix(angle), m_matrix);
}


void Renderer::rotateZ(float angle)
{
	applyMatrix(zRotateMatrix(angle), m_matrix);
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

void Renderer::getNewLine(char* buff, int count, FILE* pFile)
{
	fgets(buff, count, pFile);

	while (buff[0] == '#')
		VERIFY(fgets(buff, count, pFile));
}
