#pragma once
#include <iostream>
#include<vector>
#include"Vector4.h"
#define rows 4
#define cols 4

using namespace std;


class Matrix4 
{
public:

    float arr[rows][cols];

public:

    Matrix4() 
    {

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                    arr[i][j] = 1;
                else
                    arr[i][j] = 0;
            }
    }

    Vector4 operator*(const Vector4 v);
    friend Matrix4 operator*(Matrix4 m, float x);
    friend Matrix4 operator*(Matrix4 m, Matrix4 n);

    friend Matrix4 scaleMatrix(float x, float y, float z);
    friend Matrix4 cameraScaleMatrix(float x);
    friend Matrix4 translateMatrix(float x, float y, float z);
    friend Matrix4 xRotateMatrix(float x);
    friend Matrix4 yRotateMatrix(float y);
    friend Matrix4 zRotateMatrix(float z);
    friend Matrix4 lookAt(Vector4 eye, Vector4 at, Vector4 upVector);
    friend Matrix4 projectMatrix(float fovy, float aspect, float n, float f);
    friend Matrix4 inverseMatrix(Matrix4 m);
    friend Matrix4 transposeMatrix(Matrix4 m);

};

Matrix4 identityMatrix();




