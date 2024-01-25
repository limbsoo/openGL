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

    friend Matrix4 operator*(Matrix4 m, float x);

    friend Matrix4 operator*(Matrix4 m, Matrix4 n);
    friend Matrix4 scaleMatrix(float x, float y, float z);
    friend Matrix4 translateMatrix(float x, float y, float z);
    friend Matrix4 xRotateMatrix(float x);
    friend Matrix4 yRotateMatrix(float y);
    friend Matrix4 zRotateMatrix(float z);

    friend Matrix4 lookAt(Vector4 eye, Vector4 at, Vector4 upVector);

    friend Matrix4 xlookatRotate(float angle);

    friend Matrix4 RotateMatrix(Vector4 v, float angle);


    //friend Matrix4 calculate_lookAt_matrix(Vector4 position, Vector4 target, Vector4 worldUp);
};

Matrix4 identityMatrix();







