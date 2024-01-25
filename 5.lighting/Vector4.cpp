#include <iostream>
#include<vector>
#include"Vector4.h"
#include"Matrix4.h"

const double PI = 3.1415926;

Vector4 input(float x, float y, float z)
{
	Vector4 ret;

	ret.x = x;
	ret.y = y;
	ret.z = z;

	return ret;

}


Vector4 operator+(Vector4 v1, Vector4 v2) // 벡터 덧셈 
{ 
	Vector4 ret;

	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y; 
	ret.z = v1.z + v2.z; 
	ret.w = 1;

	return ret;
}

Vector4 operator-(Vector4 v1, Vector4 v2) // 벡터 뺄셈 
{
	Vector4 ret;

	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	ret.w = 1;
	

	return ret;
}

Vector4 operator/(Vector4 v1, float f) //벡터 나눗셈 
{
	Vector4 ret;

	ret.x = v1.x / f;
	ret.y = v1.y / f;
	ret.z = v1.z / f;

	return ret;
}


float operator*(Vector4 v1, Vector4 v2) //내적 구하기(스칼라 곱) 
{ 
	Vector4 ret;

	ret.x = v1.x * v2.x;
	ret.y = v1.y * v2.y;
	ret.z = v1.z * v2.z;

	return ret.x + ret.y +ret.z; 
}

float innerCos(Vector4 v1, Vector4 v2) //각도 구하기 
{ 
	float cos = (v1 * v2) / (magnitude(v1) * magnitude(v2));
	return cos; 
} 

Vector4 normalize(Vector4 v) // 정규화(단위벡터로 만들기) 
{ 
	Vector4 normal; 
	float magn = magnitude(v);

	if (magn == 0)
	{
		normal.x = 0;
		normal.y = 0;
		normal.z = 0;
	}
	else
	{
		normal.x = v.x / magn;
		normal.y = v.y / magn;
		normal.z = v.z / magn;
	}

	return normal; 
} 

float magnitude(Vector4 v) // 벡터 크기 
{ 
	auto magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)); 
	return magnitude; 
} 

static double Seta(float cos) //세타값 구하기 각도(디그리) 
{ 
	return acos(cos) * (180 / PI);
} 

static Vector4 proj(Vector4 v1, Vector4 v2) // 방향(normal) * 크기(magnitude = Proj(V1) 
{ 
	Vector4 ret; 
	ret.x = ((v1 * v2) / (v2 * v2)) * v2.x;
	ret.y = ((v1 * v2) / (v2 * v2)) * v2.y;
	ret.z = ((v1 * v2) / (v2 * v2)) * v2.z;

	return ret; 
} 
static Vector4 orthogonalProjection(Vector4 v1, Vector4 proj) // 직교화 
{
	Vector4 ortho; 
	ortho = v1 - proj;
	return ortho; 
} 

Vector4 crossProduct(Vector4 v1, Vector4 v2) //외적 
{
	Vector4 cross;
	cross.x = (v1.y * v2.z) - (v1.z * v2.y);
	cross.y = (v1.z * v2.x) - (v1.x * v2.z);
	cross.z = (v1.x * v2.y) - (v1.y * v2.x);
	return cross;
}


//Vector4 normalVertex(Vector4 v1, Vector4 v2) //외적 
//{
//	Vector4 v(m_vertex[i], 1);
//
//
//	Vector4 cross;
//	cross.x = (v1.y * v2.z) - (v1.z * v2.y);
//	cross.y = (v1.z * v2.x) - (v1.x * v2.z);
//	cross.z = (v1.x * v2.y) - (v1.y * v2.x);
//	return cross;
//}