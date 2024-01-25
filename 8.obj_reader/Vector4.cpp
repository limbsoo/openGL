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

Vector4 operator+(Vector4 v1, Vector4 v2) // º¤ÅÍ µ¡¼À 
{ 
	Vector4 ret;

	ret.x = v1.x + v2.x;
	ret.y = v1.y + v2.y; 
	ret.z = v1.z + v2.z; 
	ret.w = 1;

	return ret;
}

Vector4 operator-(Vector4 v1, Vector4 v2) // º¤ÅÍ »¬¼À 
{
	Vector4 ret;

	ret.x = v1.x - v2.x;
	ret.y = v1.y - v2.y;
	ret.z = v1.z - v2.z;
	ret.w = 1;
	

	return ret;
}

Vector4 operator/(Vector4 v1, float f) //º¤ÅÍ ³ª´°¼À 
{
	Vector4 ret;

	ret.x = v1.x / f;
	ret.y = v1.y / f;
	ret.z = v1.z / f;

	return ret;
}

Vector4 operator*(Vector4 v1, float f) 
{
	Vector4 ret;

	ret.x = v1.x * f;
	ret.y = v1.y * f;
	ret.z = v1.z * f;

	return ret;
}

float operator*(Vector4 v1, Vector4 v2) //³»Àû ±¸ÇÏ±â(½ºÄ®¶ó °ö) 
{ 
	Vector4 ret;

	ret.x = v1.x * v2.x;
	ret.y = v1.y * v2.y;
	ret.z = v1.z * v2.z;

	return ret.x + ret.y +ret.z; 
}

float innerCos(Vector4 v1, Vector4 v2) //°¢µµ ±¸ÇÏ±â 
{ 
	float cos = (v1 * v2) / (magnitude(v1) * magnitude(v2));
	return cos; 
} 

Vector4 normalize(Vector4 v) // Á¤±ÔÈ­(´ÜÀ§º¤ÅÍ·Î ¸¸µé±â) 
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

float magnitude(Vector4 v) // º¤ÅÍ Å©±â 
{ 
	auto magnitude = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2)); 
	return magnitude; 
} 

static double Seta(float cos) //¼¼Å¸°ª ±¸ÇÏ±â °¢µµ(µð±×¸®) 
{ 
	return acos(cos) * (180 / PI);
} 

static Vector4 proj(Vector4 v1, Vector4 v2) // ¹æÇâ(normal) * Å©±â(magnitude = Proj(V1) 
{ 
	Vector4 ret; 
	ret.x = ((v1 * v2) / (v2 * v2)) * v2.x;
	ret.y = ((v1 * v2) / (v2 * v2)) * v2.y;
	ret.z = ((v1 * v2) / (v2 * v2)) * v2.z;

	return ret; 
} 
static Vector4 orthogonalProjection(Vector4 v1, Vector4 proj) // Á÷±³È­ 
{
	Vector4 ortho; 
	ortho = v1 - proj;
	return ortho; 
} 

Vector4 crossProduct(Vector4 v1, Vector4 v2) //¿ÜÀû 
{
	Vector4 cross;
	cross.x = (v1.y * v2.z) - (v1.z * v2.y);
	cross.y = (v1.z * v2.x) - (v1.x * v2.z);
	cross.z = (v1.x * v2.y) - (v1.y * v2.x);
	return cross;
}
