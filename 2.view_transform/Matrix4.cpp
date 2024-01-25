#include <iostream>
#include<vector>
#include"Matrix4.h"
#include<cmath>
#include"Renderer.h"

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

Matrix4 RotateMatrix(Vector4 v, float angle)
{

    Matrix4 ret = identityMatrix();

    float c = cos(angle * PI / 180);
    float s = sin(angle * PI / 180);

    ret.arr[0][0] = c + (v.x * v.x) * (1 - c);          ret.arr[0][1] = (v.x * v.y * (1 - c)) - v.z * s;    ret.arr[0][2] = (v.x * v.z * (1 - c)) + v.z * s;
    ret.arr[1][0] = (v.y * v.x * (1 - c)) + v.z * s;    ret.arr[1][1] = c + (v.y * v.y * (1 - c));          ret.arr[1][2] = (v.y * v.z * (1 - c)) - v.x * s;
    ret.arr[2][0] = (v.z * v.x * (1 - c)) - v.y * s;    ret.arr[2][1] = (v.z * v.y * (1 - c)) - v.x * s;    ret.arr[2][2] = c + (v.z * v.z * (1 - c));

    return ret;

}


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
     up = normalize(up);


     // set rotation part, inverse rotation matrix: M^-1 = M^T for Euclidean transform

     Matrix4 R = identityMatrix();

     R.arr[0][0] = left.x;        R.arr[0][1] = left.y;      R.arr[0][2] = left.z;      R.arr[0][3] = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
     R.arr[1][0] = up.x;          R.arr[1][1] = up.y;        R.arr[1][2] = up.z;        R.arr[1][3] = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
     R.arr[2][0] = forward.x;     R.arr[2][1] = forward.y;   R.arr[2][2] = forward.z;   R.arr[2][3] = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;



     return R;
 }

 const float degree = 3.141593f / 180.0f;

 Matrix4 xlookatRotate(float angle)
 {
     Matrix4 R = identityMatrix();

     float c = cos(angle * degree);
     float s = sin(angle * degree);
     float m1 = R.arr[0][0], m2 = R.arr[0][1],
           m5 = R.arr[1][0], m6 = R.arr[1][1],
           m9 = R.arr[2][0], m10 = R.arr[2][1],
           m13 = R.arr[3][0], m14 = R.arr[3][1];

     Matrix4 ret;

     ret.arr[0][0] = m1 * c + m2 * -s;
     ret.arr[0][1] = m1 * s + m2 * c;
     ret.arr[1][0] = m5 * c + m6 * -s;
     ret.arr[1][1] = m5 * s + m6 * c;
     ret.arr[2][0] = m9 * c + m10 * -s;
     ret.arr[2][1] = m9 * s + m10 * c;
     ret.arr[3][0] = m13 * c + m14 * -s;
     ret.arr[3][1] = m13 * s + m14 * c;

     return ret;
 }


 //Matrix4 calculate_lookAt_matrix(Vector4 position, Vector4 target, Vector4 worldUp)
 //{

 //    Vector4 zaxis = normalize(position - target);
 //
 //    Vector4 xaxis = normalize(crossProduct(normalize(worldUp), zaxis));
 //
 //    Vector4 yaxis = crossProduct(zaxis, xaxis);

 //
 //    Matrix4 translation = identityMatrix();
 //    translation.arr[3][0] = -position.x; 
 //    translation.arr[3][1] = -position.y;
 //    translation.arr[3][2] = -position.z;

 //    Matrix4 rotation = identityMatrix();

 //    rotation.arr[0][0] = xaxis.x; 
 //    rotation.arr[1][0] = xaxis.y;
 //    rotation.arr[2][0] = xaxis.z;
 //    rotation.arr[0][1] = yaxis.x; 
 //    rotation.arr[1][1] = yaxis.y;
 //    rotation.arr[2][1] = yaxis.z;
 //    rotation.arr[0][2] = zaxis.x; 
 //    rotation.arr[1][2] = zaxis.y;
 //    rotation.arr[2][2] = zaxis.z;

 //    
 //    return rotation * translation; 
 //}



