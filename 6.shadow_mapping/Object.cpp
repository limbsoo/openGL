#include "Object.h"
#include "Renderer.h"


void Object::readFile(char* pFileName)
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
			m_vertex[i][j] = (m_vertex[i][j]);	//cow

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

void Object::getNewLine(char* buff, int count, FILE* pFile)
{
	fgets(buff, count, pFile);

	while (buff[0] == '#')
		VERIFY(fgets(buff, count, pFile));
}

void Object::makefaceNormals() //외적 
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


void Object::makeVertexNormals()
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


void Object::makeVertexUVs()
{
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v(m_vertex[i], 1);
		m_uv[i][0] = (v.x + 1) * 320;
		m_uv[i][1] = (v.y + 1) * 240;
	}
}

void Object::applyMatrix(Matrix4 mvp, Matrix4 world)
{
	Matrix4 screenMatrix = identityMatrix();
	screenMatrix.arr[0][0] = 320;
	screenMatrix.arr[0][3] = 320;
	screenMatrix.arr[1][1] = 240;
	screenMatrix.arr[1][3] = 240;

	screenMatrix = screenMatrix * mvp;

	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v(m_vertex[i], 1);

		v = screenMatrix * v;

		m_tramsformedVertex[i][0] = v.x / v.w;
		m_tramsformedVertex[i][1] = v.y / v.w;
		m_tramsformedVertex[i][2] = v.z / v.w;

		Vector4 v1(m_depthVector[i], 1);

		v1 = screenMatrix * v1;

		m_depthVector[i][0] = v1.x / v1.w;
		m_depthVector[i][1] = v1.y / v1.w;
		m_depthVector[i][2] = v1.z / v1.w;
	}

}

void Object::makeWorldVertexNormal()
{
	for (int i = 0; i < m_nNumVertex; i++)
	{

		Vector4 v2(m_vertexNormal[i], 1);

		v2 = m_world * v2;

		m_worldVertexNormal[i][0] = v2.x;
		m_worldVertexNormal[i][1] = v2.y;
		m_worldVertexNormal[i][2] = v2.z;
	}
}

void Object::makeVeritces()
{
	makeVertexUVs();
	makeWorldVertex();
	makefaceNormals();
	makeVertexNormals();
	makeWorldVertexNormal();
	
}


void Object::makeRVector(Vector4 eye)
{
	float cos;
	Vector4 s;

	for (int i = 0; i < m_nNumVertex; i++)
	{

		Vector4 normal(m_worldVertexNormal[i], 1);
		
		Vector4 postition(m_worldVertex[i], 1);

		postition = eye - postition;

		postition = normalize(postition);

		cos = normal * postition;
		s = normal * cos;

		s.x = (s.x * 2) - postition.x;
		s.y = (s.y * 2) - postition.y;
		s.z = (s.z * 2) - postition.z;

		m_rVector[i][0] = s.x;
		m_rVector[i][1] = s.y;
		m_rVector[i][2] = s.z;

	}
}

void Object::makeWorldVertex()
{
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 v(m_vertex[i], 1);

		v = m_world * v;

		m_worldVertex[i][0] = v.x / v.w;
		m_worldVertex[i][1] = v.y / v.w;
		m_worldVertex[i][2] = v.z / v.w;
	}
}

void Object::makeWorldLightingDepth(Vector4 lightPosition)
{
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 postition(m_worldVertex[i], 1);

		postition = lightPosition - postition;

		m_depthVector[i][0] = postition.x;
		m_depthVector[i][1] = postition.y;
		m_depthVector[i][2] = postition.z;

		//shadowPostition.x = ((directionLight.y * postition.x) - (directionLight.x * postition.y)) / (directionLight.y - postition.y);
		//shadowPostition.y = 0;
		//shadowPostition.z = 0;

		//m_lightVector[i][0] = shadowPostition.x;
		//m_lightVector[i][1] = shadowPostition.y;
		//m_lightVector[i][2] = shadowPostition.z;



	}
	
}

void Object::makeWorldCameraDepth(Vector4 eye)
{
	for (int i = 0; i < m_nNumVertex; i++)
	{
		Vector4 postition(m_worldVertex[i], 1);
		Vector4 shadowPostition;

		postition = eye - postition;

		m_depthVector[i][0] = postition.x;
		m_depthVector[i][1] = postition.y;
		m_depthVector[i][2] = postition.z;




	}

}
