#pragma once

#include <vector>
#include <Hauk.h>

using namespace std;

struct vec3d
{
	float x, y, z;
};

struct triangle
{
	vec3d p[3];
};

struct mesh
{
	vector<triangle> tris;
};

struct mat4x4
{
	float m[4][4] = { 0 };
};

void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m);
void DrawCube3D(ANIMATED_WINDOW_INFO *sAnimateWindowInfo);
