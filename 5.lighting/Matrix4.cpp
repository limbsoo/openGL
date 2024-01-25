#include <iostream>
#include<vector>
#include"Matrix4.h"
#include<cmath>
#include"Renderer.h"
//#include"opengl_normalVertex_cow"




int n = 4;
using namespace std;

const double PI = 3.1415926;

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

Matrix4 operator*(Matrix4 m, float x)
{
    Matrix4 ret;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.arr[i][j] = m.arr[i][j] * x;
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

 Matrix4 cameraScaleMatrix(float x)
 {
     Matrix4 m;

     m = identityMatrix();

     m.arr[0][0] = x;
     m.arr[1][1] = x;
     m.arr[2][2] = x;

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
     //return *this
 }

//Renderer g_renderer;

Renderer renderer;

 Matrix4 lookAt(Vector4 eye, Vector4 at, Vector4 upVector)
 {
     // compute the forward vector from target to eye
     Vector4 forward = eye - at;
     forward = normalize(forward);                 // make unit length

     // compute the left vector
     Vector4 left = crossProduct(upVector,forward); // cross product
     left = normalize(left);

     // recompute the orthonormal up vector
     Vector4 up = crossProduct(forward,left);    // cross product
     //up = normalize(up);


     // set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform

     Matrix4 ret = identityMatrix();

     ret.arr[0][0] = left.x;        ret.arr[0][1] = left.y;      ret.arr[0][2] = left.z;      ret.arr[0][3] = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
     ret.arr[1][0] = up.x;          ret.arr[1][1] = up.y;        ret.arr[1][2] = up.z;        ret.arr[1][3] = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
     ret.arr[2][0] = forward.x;     ret.arr[2][1] = forward.y;   ret.arr[2][2] = forward.z;   ret.arr[2][3] = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;

     //normal
        
     // mworld에 곱하기


     return ret;
 }


 Matrix4 projectMatrix(float fovy, float aspect, float n, float f)
 {
     Matrix4 ret;

     for (int i = 0; i < 4; i++)
     {
         for (int j = 0; j < 4; j++)
         {
             ret.arr[i][j] = 0;
         }
     }

     ret.arr[0][0] = (1 / tan(fovy / 2 * PI / 180)) / aspect;
     ret.arr[1][1] = 1 / tan(fovy / 2 * PI / 180);
     ret.arr[2][2] = - ((f + n) / (f - n));
     ret.arr[2][3] = - ((2 * (n * f)) / (f - n));
     ret.arr[3][2] = -1;

     //normal

     //Matrix4 inverseTransposeMatrix;

     //inverseTransposeMatrix = inverseMatrix(ret);
     //inverseTransposeMatrix = transposeMatrix(inverseTransposeMatrix);

     //for (int i = 0; i < renderer.m_nNumFace; i++) // 각 face의 normal
     //{
     //    renderer.m_faceNormal[i][0] = renderer.m_faceNormal[i][0] * inverseTransposeMatrix.arr[0][0] + renderer.m_faceNormal[i][1] * inverseTransposeMatrix.arr[0][1] + renderer.m_faceNormal[i][2] * inverseTransposeMatrix.arr[0][2];
     //    renderer.m_faceNormal[i][1] = renderer.m_faceNormal[i][0] * inverseTransposeMatrix.arr[1][0] + renderer.m_faceNormal[i][1] * inverseTransposeMatrix.arr[1][1] + renderer.m_faceNormal[i][2] * inverseTransposeMatrix.arr[1][2];
     //    renderer.m_faceNormal[i][2] = renderer.m_faceNormal[i][0] * inverseTransposeMatrix.arr[2][0] + renderer.m_faceNormal[i][1] * inverseTransposeMatrix.arr[2][1] + renderer.m_faceNormal[i][2] * inverseTransposeMatrix.arr[2][2];
     //}


     return ret;

 }

 Matrix4 inverseMatrix(Matrix4 m)
 {

     Matrix4 inv;
     float det;
     Matrix4 ret;

     inv.arr[0][0] = m.arr[1][1] * m.arr[2][2] * m.arr[3][3] - m.arr[1][1] * m.arr[2][3] * m.arr[3][2] - m.arr[2][1] * m.arr[1][2] * m.arr[3][3] + m.arr[2][1] * m.arr[1][3] * m.arr[3][2] + m.arr[3][1] * m.arr[1][2] * m.arr[2][3] - m.arr[3][1] * m.arr[1][3] * m.arr[2][2];
     inv.arr[0][1] = -m.arr[0][1] * m.arr[2][2] * m.arr[3][3] + m.arr[0][1] * m.arr[2][3] * m.arr[3][2] + m.arr[2][1] * m.arr[0][2] * m.arr[3][3] - m.arr[2][1] * m.arr[0][3] * m.arr[3][2] - m.arr[3][1] * m.arr[0][2] * m.arr[2][3] + m.arr[3][1] * m.arr[0][3] * m.arr[2][2];
     inv.arr[0][2] = m.arr[0][1] * m.arr[1][2] * m.arr[3][3] - m.arr[0][1] * m.arr[1][3] * m.arr[3][2] - m.arr[1][1] * m.arr[0][2] * m.arr[3][3] + m.arr[1][1] * m.arr[0][3] * m.arr[3][2] + m.arr[3][1] * m.arr[0][2] * m.arr[1][3] - m.arr[3][1] * m.arr[0][3] * m.arr[1][2];
     inv.arr[0][3] = -m.arr[0][1] * m.arr[1][2] * m.arr[2][3] + m.arr[0][1] * m.arr[1][3] * m.arr[2][2] + m.arr[1][1] * m.arr[0][2] * m.arr[2][3] - m.arr[1][1] * m.arr[0][3] * m.arr[2][2] - m.arr[2][1] * m.arr[0][2] * m.arr[1][3] + m.arr[2][1] * m.arr[0][3] * m.arr[1][2];

     inv.arr[1][0] = -m.arr[1][0] * m.arr[2][2] * m.arr[3][3] + m.arr[1][0] * m.arr[2][3] * m.arr[3][2] + m.arr[2][0] * m.arr[1][2] * m.arr[3][3] - m.arr[2][0] * m.arr[1][3] * m.arr[3][2] - m.arr[3][0] * m.arr[1][2] * m.arr[2][3] + m.arr[3][0] * m.arr[1][3] * m.arr[2][2];
     inv.arr[1][1] = m.arr[0][0] * m.arr[2][2] * m.arr[3][3] - m.arr[0][0] * m.arr[2][3] * m.arr[3][2] - m.arr[2][0] * m.arr[0][2] * m.arr[3][3] + m.arr[2][0] * m.arr[0][3] * m.arr[3][2] + m.arr[3][0] * m.arr[0][2] * m.arr[2][3] - m.arr[3][0] * m.arr[0][3] * m.arr[2][2];
     inv.arr[1][2] = -m.arr[0][0] * m.arr[1][2] * m.arr[3][3] + m.arr[0][0] * m.arr[1][3] * m.arr[3][2] + m.arr[1][0] * m.arr[0][2] * m.arr[3][3] - m.arr[1][0] * m.arr[0][3] * m.arr[3][2] - m.arr[3][0] * m.arr[0][2] * m.arr[1][3] + m.arr[3][0] * m.arr[0][3] * m.arr[1][2];
     inv.arr[1][3] = m.arr[0][0] * m.arr[1][2] * m.arr[2][3] - m.arr[0][0] * m.arr[1][3] * m.arr[2][2] - m.arr[1][0] * m.arr[0][2] * m.arr[2][3] + m.arr[1][0] * m.arr[0][3] * m.arr[2][2] + m.arr[2][0] * m.arr[0][2] * m.arr[1][3] - m.arr[2][0] * m.arr[0][3] * m.arr[1][2];

     inv.arr[2][0] = m.arr[1][0] * m.arr[2][1] * m.arr[3][3] - m.arr[1][0] * m.arr[2][3] * m.arr[3][1] - m.arr[2][0] * m.arr[1][1] * m.arr[3][3] + m.arr[2][0] * m.arr[1][3] * m.arr[3][1] + m.arr[3][0] * m.arr[1][1] * m.arr[2][3] - m.arr[3][0] * m.arr[1][3] * m.arr[2][1];
     inv.arr[2][1] = -m.arr[0][0] * m.arr[2][1] * m.arr[3][3] + m.arr[0][0] * m.arr[2][3] * m.arr[3][1] + m.arr[2][0] * m.arr[0][1] * m.arr[3][3] - m.arr[2][0] * m.arr[0][3] * m.arr[3][1] - m.arr[3][0] * m.arr[0][1] * m.arr[2][3] + m.arr[3][0] * m.arr[0][3] * m.arr[2][1];
     inv.arr[2][2] = m.arr[0][0] * m.arr[1][1] * m.arr[3][3] - m.arr[0][0] * m.arr[1][3] * m.arr[3][1] - m.arr[1][0] * m.arr[0][1] * m.arr[3][3] + m.arr[1][0] * m.arr[0][3] * m.arr[3][1] + m.arr[3][0] * m.arr[0][1] * m.arr[1][3] - m.arr[3][0] * m.arr[0][3] * m.arr[1][1];
     inv.arr[2][3] = -m.arr[0][0] * m.arr[1][1] * m.arr[2][3] + m.arr[0][0] * m.arr[1][3] * m.arr[2][1] + m.arr[1][0] * m.arr[0][1] * m.arr[2][3] - m.arr[1][0] * m.arr[0][3] * m.arr[2][1] - m.arr[2][0] * m.arr[0][1] * m.arr[1][3] + m.arr[2][0] * m.arr[0][3] * m.arr[1][1];

     inv.arr[3][0] = -m.arr[1][0] * m.arr[2][1] * m.arr[3][2] + m.arr[1][0] * m.arr[2][2] * m.arr[3][1] + m.arr[2][0] * m.arr[1][1] * m.arr[3][2] - m.arr[2][0] * m.arr[1][2] * m.arr[3][1] - m.arr[3][0] * m.arr[1][1] * m.arr[2][2] + m.arr[3][0] * m.arr[1][2] * m.arr[2][1];
     inv.arr[3][1] = m.arr[0][0] * m.arr[2][1] * m.arr[3][2] - m.arr[0][0] * m.arr[2][2] * m.arr[3][1] - m.arr[2][0] * m.arr[0][1] * m.arr[3][2] + m.arr[2][0] * m.arr[0][2] * m.arr[3][1] + m.arr[3][0] * m.arr[0][1] * m.arr[2][2] - m.arr[3][0] * m.arr[0][2] * m.arr[2][1];
     inv.arr[3][2] = -m.arr[0][0] * m.arr[1][1] * m.arr[3][2] + m.arr[0][0] * m.arr[1][2] * m.arr[3][1] + m.arr[1][0] * m.arr[0][1] * m.arr[3][2] - m.arr[1][0] * m.arr[0][2] * m.arr[3][1] - m.arr[3][0] * m.arr[0][1] * m.arr[1][2] + m.arr[3][0] * m.arr[0][2] * m.arr[1][1];
     inv.arr[3][3] = m.arr[0][0] * m.arr[1][1] * m.arr[2][2] - m.arr[0][0] * m.arr[1][2] * m.arr[2][1]  - m.arr[1][0] * m.arr[0][1] * m.arr[2][2] + m.arr[1][0] * m.arr[0][2] * m.arr[2][1] + m.arr[2][0] * m.arr[0][1] * m.arr[1][2] - m.arr[2][0] * m.arr[0][2] * m.arr[1][1];





     det = m.arr[0][0] * inv.arr[0][0] + m.arr[0][1] * inv.arr[1][0] + m.arr[0][2] * inv.arr[2][0] + m.arr[0][3] * inv.arr[3][0];



     if (det == 0)
     {
         //return 0;
         ret = identityMatrix();
     }

     else
     {
         det = 1.0 / det;

         for (int i = 0; i < 4; i++)
         {
             for (int j = 0; j < 4; j++)
             {
                 ret.arr[i][j] = inv.arr[i][j] * det;
             }
         }
     }






     //return 1;

    return ret;

 }


Matrix4 transposeMatrix(Matrix4 m)
{
    Matrix4 ret;
    float transpose[4][4];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ret.arr[i][j] = m.arr[j][i];
        }
    }

    //for (int i = 0; i < 4; i++)
    //{
    //    for (int j = 0; j < 4; i++)
    //    {
    //        ret.arr[i][j] = transpose[i][j];
    //    }
    //}
    return ret;
}
   
 
