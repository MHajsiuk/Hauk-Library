#include <Windows.h>
#include <Hauk.h>
#include "Cube3D.h"


void DrawCube3D(ANIMATED_WINDOW_INFO *sAnimateWindowInfo)
{
	cDrawingBuffer *DrawingBuffer = sAnimateWindowInfo->DrawingBufferObjectAddress;

	mesh meshcube;
	mat4x4 matproj, matrotz, matrotx;
	FillMemory(&matproj, sizeof(mat4x4), 0);
	FillMemory(&matrotz, sizeof(mat4x4), 0);
	FillMemory(&matrotx, sizeof(mat4x4), 0);

	meshcube.tris = {

		// South
		{ 0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 0.0f, 0.0f },

		// East
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 1.0f },

		// North
		{ 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 1.0f },
		{ 1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f },

		// West
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,		0.0f, 0.0f, 0.0f },

		// Top
		{ 0.0f, 1.0f, 0.0f,		0.0f, 1.0f, 1.0f,		1.0f, 1.0f, 1.0f },
		{ 0.0f, 1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f, 0.0f },

		// Bottom
		{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f },

	};

	DWORD BufferSizeX = 0;
	DWORD BufferSizeY = 0;

	DrawingBuffer->GetBufferInfo(&BufferSizeX, &BufferSizeY);

	float fNear = 1.0f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float fAspectRation = (float)BufferSizeY / (float)BufferSizeX;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	float fSecondsPassed = (float)sAnimateWindowInfo->TimeInfo.ElapsedTime.QuadPart / (float)1000000;

	float fTheta = 1.0f * (fSecondsPassed * 2);

	// Rotation by Z axis
	matrotz.m[0][0] = cosf(fTheta);
	matrotz.m[0][1] = sinf(fTheta);
	matrotz.m[1][0] = -sinf(fTheta);
	matrotz.m[1][1] = cosf(fTheta);
	matrotz.m[2][2] = 1;
	matrotz.m[3][3] = 1;

	// Rotation by X axis
	matrotx.m[0][0] = 1;
	matrotx.m[1][1] = cosf(fTheta * 0.5f);
	matrotx.m[1][2] = sinf(fTheta * 0.5f);
	matrotx.m[2][1] = -sinf(fTheta * 0.5f);
	matrotx.m[2][2] = cosf(fTheta * 0.5f);
	matrotx.m[3][3] = 1;

	// Projection Matrix
	matproj.m[0][0] = fAspectRation * fFovRad;
	matproj.m[1][1] = fFovRad;
	matproj.m[2][2] = fFar - (fFar - fNear);
	matproj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
	matproj.m[2][3] = 1.0f;
	matproj.m[3][3] = 0.0f;

	DrawingBuffer->ClearBuffer();

	for (auto tri : meshcube.tris)
	{
		triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;

		MultiplyMatrixVector(tri.p[0], triRotatedZ.p[0], matrotz);
		MultiplyMatrixVector(tri.p[1], triRotatedZ.p[1], matrotz);
		MultiplyMatrixVector(tri.p[2], triRotatedZ.p[2], matrotz);

		MultiplyMatrixVector(triRotatedZ.p[0], triRotatedZX.p[0], matrotx);
		MultiplyMatrixVector(triRotatedZ.p[1], triRotatedZX.p[1], matrotx);
		MultiplyMatrixVector(triRotatedZ.p[2], triRotatedZX.p[2], matrotx);

		triTranslated = triRotatedZX;
		triTranslated.p[0].z = triRotatedZX.p[0].z + 2.5f;
		triTranslated.p[1].z = triRotatedZX.p[1].z + 2.5f;
		triTranslated.p[2].z = triRotatedZX.p[2].z + 2.5f;

		MultiplyMatrixVector(triTranslated.p[0], triProjected.p[0], matproj);
		MultiplyMatrixVector(triTranslated.p[1], triProjected.p[1], matproj);
		MultiplyMatrixVector(triTranslated.p[2], triProjected.p[2], matproj);

		// scale view
		triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
		triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
		triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

		triProjected.p[0].x *= 0.5f * (float)BufferSizeX;
		triProjected.p[0].y *= 0.5f * (float)BufferSizeY;
		triProjected.p[1].x *= 0.5f * (float)BufferSizeX;
		triProjected.p[1].y *= 0.5f * (float)BufferSizeY;
		triProjected.p[2].x *= 0.5f * (float)BufferSizeX;
		triProjected.p[2].y *= 0.5f * (float)BufferSizeY;

		DrawingBuffer->DrawTriangle2D((DWORD)triProjected.p[0].x, triProjected.p[0].y,
									  (DWORD)triProjected.p[1].x, triProjected.p[1].y,
									  (DWORD)triProjected.p[2].x, triProjected.p[2].y,
									  RGB(0xFF, 0xFF, 0xFF));
	}

}

	
void MultiplyMatrixVector(vec3d &i, vec3d &o, mat4x4 &m)
{
	float w;

	o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + i.z * m.m[2][0] + m.m[3][0];
	o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + i.z * m.m[2][1] + m.m[3][1];
	o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + i.z * m.m[2][2] + m.m[3][2];
	w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];

	if (w != 0.0f)
	{
		o.x = o.x / w;
		o.y = o.y / w;
		o.z = o.z / w;
	}
}


