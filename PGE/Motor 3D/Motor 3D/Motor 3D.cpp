
#define _USE_MATH_DEFINES
#include <math.h>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct vec3d {
	float x, y, z;
};

struct triangle {
	vec3d p[3];
};

struct mesh {
	std::vector <triangle> tris;
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

// Override base class with your custom functionality
class Demo : public olc::PixelGameEngine
{
public:
	Demo()
	{
		// Name your application
		sAppName = "Example";
	}

private:

	mesh meshCube;
	mat4x4 MatProj;

	float fTheta;

	void MultiplicaMatrizxVector(vec3d &i, vec3d &o, mat4x4 &m) {
		o.x = i.x * m.m[0][0] + i.y * m.m[1][0] + +i.z * m.m[2][0] + m.m[3][0];
		o.y = i.x * m.m[0][1] + i.y * m.m[1][1] + +i.z * m.m[2][1] + m.m[3][1];
		o.z = i.x * m.m[0][2] + i.y * m.m[1][2] + +i.z * m.m[2][2] + m.m[3][2];
		float w = i.x * m.m[0][3] + i.y * m.m[1][3] + +i.z * m.m[2][3] + m.m[3][3];

		if (w != 0.0f) {
			o.x /= w; o.y /= w; o.z /= w;
		}
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		
		meshCube.tris = {
		
			//Frente
			{-1.0f, -1.0f, -1.0f,   -1.0f, 1.0f, -1.0f,   1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, -1.0f,   1.0f, 1.0f, -1.0f,   1.0f, -1.0f, -1.0f},

			//Izquierda
			{1.0f, -1.0f, -1.0f,   1.0f, 1.0f, -1.0f,   1.0f, 1.0f, 1.0f},
			{1.0f, -1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, -1.0f, 1.0f},

			//Posterior
			{1.0f, -1.0f, 1.0f,   1.0f, 1.0f, 1.0f,   -1.0f, 1.0f, 1.0f},
			{1.0f, -1.0f, 1.0f,   -1.0f, 1.0f, 1.0f,   -1.0f, -1.0f, 1.0f},

			//Derecha
			{-1.0f, -1.0f, 1.0f,   -1.0f, 1.0f, 1.0f,   -1.0f, 1.0f, -1.0f},
			{-1.0f, -1.0f, 1.0f,   -1.0f, 1.0f, -1.0f,   -1.0f, -1.0f, -1.0f},

			//Superior
			{-1.0f, 1.0f, -1.0f,   -1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f},
			{-1.0f, 1.0f, -1.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, -1.0f},

			//Inferior
			{1.0f, -1.0f, 1.0f,   -1.0f, -1.0f, 1.0f,   -1.0f, -1.0f, -1.0f},
			{1.0f, -1.0f, 1.0f,   -1.0f, -1.0f, -1.0f,   1.0f, -1.0f, -1.0f}
		
		};


		//Proyección
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFOV = 100.0;
		float fAspectRatio = float(ScreenHeight()) / float(ScreenWidth());
		float fFOVRad = 1.0f / tan(fFOV * 0.5f / 180.0f * M_PI);

		MatProj.m[0][0] = fAspectRatio * fFOVRad;
		MatProj.m[1][1] = fFOVRad;
		MatProj.m[2][2] = fFar / (fFar - fNear);
		MatProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		MatProj.m[2][3] = 1.0f;
		MatProj.m[3][3] = 0.0f;

		return true;
	}

	void RotateInX(mesh &Obj, float Angle) {
		mat4x4 matRotX;
		matRotX.m[0][0] = 1.0f;
		matRotX.m[1][1] = cosf(Angle);
		matRotX.m[1][2] = sinf(Angle);
		matRotX.m[2][1] = -sinf(Angle);
		matRotX.m[2][2] = cosf(Angle);
		matRotX.m[3][3] = 1.0f;

		for (auto &tri : Obj.tris) {
			triangle triRotatedX;

			MultiplicaMatrizxVector(tri.p[0], triRotatedX.p[0], matRotX);
			MultiplicaMatrizxVector(tri.p[1], triRotatedX.p[1], matRotX);
			MultiplicaMatrizxVector(tri.p[2], triRotatedX.p[2], matRotX);

			tri.p[0] = triRotatedX.p[0];
			tri.p[1] = triRotatedX.p[1];
			tri.p[2] = triRotatedX.p[2];
		}

	}

	void RotateInY(mesh& Obj, float Angle) {
		mat4x4 matRotY;
		matRotY.m[0][0] = cosf(Angle);
		matRotY.m[1][1] = 1.0f;
		matRotY.m[2][0] = sinf(Angle);
		matRotY.m[0][2] = -sinf(Angle);
		matRotY.m[2][2] = cosf(Angle);
		matRotY.m[3][3] = 1.0f;

		for (auto& tri : Obj.tris) {
			triangle triRotatedY;

			MultiplicaMatrizxVector(tri.p[0], triRotatedY.p[0], matRotY);
			MultiplicaMatrizxVector(tri.p[1], triRotatedY.p[1], matRotY);
			MultiplicaMatrizxVector(tri.p[2], triRotatedY.p[2], matRotY);

			tri.p[0] = triRotatedY.p[0];
			tri.p[1] = triRotatedY.p[1];
			tri.p[2] = triRotatedY.p[2];
		}

	}

	void RotateInZ(mesh& Obj, float Angle) {
		mat4x4 matRotZ;
		matRotZ.m[0][0] = cosf(Angle);
		matRotZ.m[0][1] = sinf(Angle);
		matRotZ.m[1][0] = -sinf(Angle);
		matRotZ.m[1][1] = cosf(Angle);
		matRotZ.m[2][2] = 1.0f;
		matRotZ.m[3][3] = 1.0f;

		for (auto& tri : Obj.tris) {
			triangle triRotatedZ;

			MultiplicaMatrizxVector(tri.p[0], triRotatedZ.p[0], matRotZ);
			MultiplicaMatrizxVector(tri.p[1], triRotatedZ.p[1], matRotZ);
			MultiplicaMatrizxVector(tri.p[2], triRotatedZ.p[2], matRotZ);

			tri.p[0] = triRotatedZ.p[0];
			tri.p[1] = triRotatedZ.p[1];
			tri.p[2] = triRotatedZ.p[2];
		}

	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		Clear(olc::BLACK);

		// Matrices de rotación
		mat4x4 matRotX, matRotY, matRotZ;
		float fAngle = 2 * GetElapsedTime();
		if (GetKey(olc::D).bHeld) RotateInY(meshCube, fAngle);
		else if (GetKey(olc::A).bHeld) RotateInY(meshCube, -fAngle);

		if (GetKey(olc::W).bHeld) RotateInX(meshCube, fAngle);
		else if (GetKey(olc::S).bHeld) RotateInX(meshCube, -fAngle);

		if (GetKey(olc::RIGHT).bHeld) RotateInZ(meshCube, fAngle);
		else if (GetKey(olc::LEFT).bHeld) RotateInZ(meshCube, -fAngle);
		//fTheta = M_PI;

		//fTheta += 3 * GetElapsedTime();

		//RotateInX(meshCube, fAngle);

		// Rotación X
		matRotX.m[0][0] = 1.0f;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1.0f;

		// Rotación Y
		matRotY.m[0][0] = cosf(fTheta * 0.5f);
		matRotY.m[1][1] = 1.0f;
		matRotY.m[2][0] = sinf(fTheta * 0.5f);
		matRotY.m[0][2] = -sinf(fTheta * 0.5f);
		matRotY.m[2][2] = cosf(fTheta * 0.5f);
		matRotY.m[3][3] = 1.0f;

		// Rotación Z
		matRotZ.m[0][0] = cosf(fTheta * 0.5f);
		matRotZ.m[0][1] = sinf(fTheta * 0.5f);
		matRotZ.m[1][0] = -sinf(fTheta * 0.5f);
		matRotZ.m[1][1] = cosf(fTheta * 0.5f);
		matRotZ.m[2][2] = 1.0f;
		matRotZ.m[3][3] = 1.0f;

		//Dibujar triangulos
		for (auto tri : meshCube.tris) {
			triangle triProjected, triTranslated;

			/*
			triangle triRotatedZ, triRotatedZX, triRotatedZXY;
			
			MultiplicaMatrizxVector(tri.p[0], triRotatedZ.p[0], matRotX);
			MultiplicaMatrizxVector(tri.p[1], triRotatedZ.p[1], matRotX);
			MultiplicaMatrizxVector(tri.p[2], triRotatedZ.p[2], matRotX);

			MultiplicaMatrizxVector(triRotatedZ.p[0], triRotatedZX.p[0], matRotX);
			MultiplicaMatrizxVector(triRotatedZ.p[1], triRotatedZX.p[1], matRotX);
			MultiplicaMatrizxVector(triRotatedZ.p[2], triRotatedZX.p[2], matRotX);

			MultiplicaMatrizxVector(triRotatedZX.p[0], triRotatedZXY.p[0], matRotY);
			MultiplicaMatrizxVector(triRotatedZX.p[1], triRotatedZXY.p[1], matRotY);
			MultiplicaMatrizxVector(triRotatedZX.p[2], triRotatedZXY.p[2], matRotY);
			*/

			triTranslated = tri;
			triTranslated.p[0].z = tri.p[0].z + 3.0;
			triTranslated.p[1].z = tri.p[1].z + 3.0;
			triTranslated.p[2].z = tri.p[2].z + 3.0;

			MultiplicaMatrizxVector(triTranslated.p[0], triProjected.p[0], MatProj);
			MultiplicaMatrizxVector(triTranslated.p[1], triProjected.p[1], MatProj);
			MultiplicaMatrizxVector(triTranslated.p[2], triProjected.p[2], MatProj);

			triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
			triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
			triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

			triProjected.p[0].x *= 0.5f * float(ScreenWidth());
			triProjected.p[0].y *= 0.5f * float(ScreenHeight());
			triProjected.p[1].x *= 0.5f * float(ScreenWidth());
			triProjected.p[1].y *= 0.5f * float(ScreenHeight());
			triProjected.p[2].x *= 0.5f * float(ScreenWidth());
			triProjected.p[2].y *= 0.5f * float(ScreenHeight());

			DrawTriangle(triProjected.p[0].x, triProjected.p[0].y,
				         triProjected.p[1].x, triProjected.p[1].y,
				         triProjected.p[2].x, triProjected.p[2].y,
				         olc::WHITE);
		}
		
		return true;
	}
};

int main()
{
	Demo demo;
	if (demo.Construct(612, 480, 1, 1, false, true))
		demo.Start();
	return 0;
}