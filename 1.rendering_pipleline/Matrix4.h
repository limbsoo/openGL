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

    Vector4 operator*(const Vector4 v);

    friend Matrix4 operator*(Matrix4 m, Matrix4 n);
    friend Matrix4 scaleMatrix(float x, float y, float z);
    friend Matrix4 translateMatrix(float x, float y, float z);
    friend Matrix4 xRotateMatrix(float x);
    friend Matrix4 yRotateMatrix(float y);
    friend Matrix4 zRotateMatrix(float z);
};

Matrix4 identityMatrix();
Matrix4 currentMatrix();
//Matrix4 bringMartix(Matrix4 n);






