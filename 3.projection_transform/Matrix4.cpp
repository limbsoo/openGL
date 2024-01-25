#include <iostream>
#include<vector>
#include"Matrix4.h"
#include<cmath>

int n = 4;
//asdasdasdas
using namespace std;

const double PI = 3.1415926;

//Matrix4 current;

Matrix4 identityMatrix()
{
    Matrix4 m;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                m.arr[i][j] = 1;
            }
            else
            {
                m.arr[i][j] = 0;
            }
        }
    }

    return m;
}


Vector4  Matrix4::operator*(Vector4 v)
{
    Vector4 ret;

    ret.x = v.x * arr[0][0] + v.y * arr[0][1] + v.z * arr[0][2] + v.w * arr[0][3];
    ret.y = v.x * arr[1][0] + v.y * arr[1][1] + v.z * arr[1][2] + v.w * arr[1][3];
    ret.z = v.x * arr[2][0] + v.y * arr[2][1] + v.z * arr[2][2] + v.w * arr[2][3];
    ret.w = v.x * arr[3][0] + v.y * arr[3][1] + v.z * arr[3][2] + v.w * arr[3][3];
        
    return ret;

}

Matrix4 operator*(Matrix4 m, Matrix4 n)
{
    Matrix4 ret;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.arr[i][j] = 0;
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                ret.arr[i][j] += (m.arr[i][k] * n.arr[k][j]);
            }
        }
    }

    return ret;
}

 Matrix4 scaleMatrix(float x, float y, float z)
 {
     Matrix4 m;

     m = identityMatrix();

     m.arr[0][0] = x;
     m.arr[1][1] = y;
     m.arr[2][2] = z;

     return m;

 }

 Matrix4 translateMatrix(float x, float y, float z)
 {
     Matrix4 m;

     m = identityMatrix();

     m.arr[0][3] = x;
     m.arr[1][3] = y;
     m.arr[2][3] = z;

     return m;

 }

 Matrix4 xRotateMatrix(float angle)
 {
     Matrix4 m;

     m = identityMatrix();

     float c = cos(angle * PI / 180);
     float s = sin(angle * PI / 180);

     m.arr[1][1] = c;
     m.arr[1][2] = -s;
     m.arr[2][1] = s;
     m.arr[2][2] = c;

     return m;

 }

 Matrix4 yRotateMatrix(float angle)
 {
     Matrix4 m;

     m = identityMatrix();

     float c = cos(angle * PI / 180);
     float s = sin(angle * PI / 180);

     m.arr[0][0] = c;
     m.arr[0][2] = s;
     m.arr[2][0] = -s;
     m.arr[2][2] = c;

     return m;

 }

 Matrix4 zRotateMatrix(float angle)
 {
     Matrix4 m;

     m = identityMatrix();

     float c = cos(angle * PI / 180);
     float s = sin(angle * PI / 180);

     m.arr[0][0] = c;
     m.arr[0][1] = -s;
     m.arr[1][0] = s;
     m.arr[1][1] = c;

     return m;

 }


