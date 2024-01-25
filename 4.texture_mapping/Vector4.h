#pragma once
#include <iostream>
#include<vector>

using namespace std;

class Vector4
{
public:

    float x, y, z, w;

public:

    Vector4(float _x = 0, float _y = 0, float _z = 0, float _w = 1) : x(_x), y(_y), z(_z), w(_w) {}
 

    Vector4(float v[4])
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    Vector4(float v[3], float _w = 1)
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = _w;
    }
        


    friend Vector4 normalize(Vector4 v);
    friend float magnitude(Vector4 v);
    friend Vector4 crossProduct(Vector4 v1, Vector4 v2);
    friend float innerCos(Vector4 v1, Vector4 v2);

    friend Vector4 operator/(Vector4 v1, float f);
    friend Vector4 operator+(Vector4 a, Vector4 b);
    friend Vector4 operator-(Vector4 a, Vector4 b);
    friend float operator*(Vector4 a, Vector4 b);

    

};