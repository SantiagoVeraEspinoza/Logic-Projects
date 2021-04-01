#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Test : public olc::PixelGameEngine
{
private:
	//Variables

	struct Rect {
		olc::vf2d pos;
		olc::vf2d size;
		olc::Pixel color;
		olc::vf2d vel;
	};

	std::vector <Rect> R;

	olc::vf2d LastPos;

public:
	//Constructores

	Test()
	{
		sAppName = "Colisiones";
	}



public:
	//Funciones

	bool RectVSRect(float R1posx, float R1posy, float R1sizex, float R1sizey,
		float R2posx, float R2posy, float R2sizex, float R2sizey) {
		if (R1posx < (R2posx + R2sizex) and (R1posx + R1sizex) > R2posx) {
			if (R1posy < (R2posy + R2sizey) and (R1posy + R1sizey) > R2posy) {
				return true;
			}
		}
		return false;
	}

	void ResolveColision(int tarID = 1) {
		float Dif = 0.0001;
		if (R[0].vel.x > 0 and RectVSRect(R[0].pos.x, ((LastPos.y) + (Dif)), R[0].size.x, ((R[0].size.y) - (Dif * 2)),
			R[tarID].pos.x, R[tarID].pos.y, R[tarID].size.x - 5, R[tarID].size.y))
			R[0].pos.x = R[tarID].pos.x - R[0].size.x;
		else if (R[0].vel.x < 0 and RectVSRect(R[0].pos.x, ((LastPos.y) + (Dif)), R[0].size.x, ((R[0].size.y) - (Dif * 2)),
			R[tarID].pos.x + 5, R[tarID].pos.y, R[tarID].size.x - 5, R[tarID].size.y))
			R[0].pos.x = R[tarID].pos.x + R[tarID].size.x;

		else if (R[0].vel.y > 0 and RectVSRect(LastPos.x, R[0].pos.y, R[0].size.x, R[0].size.y,
			R[tarID].pos.x + 0.9999, R[tarID].pos.y, R[tarID].size.x - 0.9999, R[tarID].size.y - 5))
			R[0].pos.y = R[tarID].pos.y - R[0].size.y;
		else if (R[0].vel.y < 0 and RectVSRect(LastPos.x, R[0].pos.y, R[0].size.x, R[0].size.y,
			R[tarID].pos.x + 0.9999, R[tarID].pos.y + 5, R[tarID].size.x - 0.9999, R[tarID].size.y - 5))
			R[0].pos.y = R[tarID].pos.y + R[tarID].size.y;
	}

	void MovePlayer(float vel = 2) {
		vel = vel * GetElapsedTime() * 100;

		if (GetKey(olc::D).bHeld) R[0].vel.x = vel;
		else if (GetKey(olc::A).bHeld) R[0].vel.x = -vel;
		else R[0].vel.x = 0;

		if (GetKey(olc::W).bHeld) R[0].vel.y = -vel;
		else if (GetKey(olc::S).bHeld) R[0].vel.y = vel;
		else R[0].vel.y = 0;
	}

	void ApplyVel() {
		R[0].pos += R[0].vel;
	}

	void DrawObjects() {
		for (auto& Rect : R) {
			DrawRect(Rect.pos, Rect.size, Rect.color);
		}
	}

public:
	//Creación y actualizado

	bool OnUserCreate() override
	{
		R.push_back({});
		R.push_back({});
		R.push_back({});
		R.push_back({});
		R.push_back({});
		R.push_back({});

		R[0].pos = { 30.0 , 30.0 };
		R[0].size = { 50.0 , 40.0 };
		R[0].color = olc::GREEN;

		R[1].pos = { 90.0 , 90.0 };
		R[1].size = { 40.0 , 50.0 };
		R[1].color = olc::GREEN;

		R[2].pos = { 100.0 , 90.0 };
		R[2].size = { 40.0 , 70.0 };
		R[2].color = olc::GREEN;

		R[3].pos = { 200.0 , 120.0 };
		R[3].size = { 100.0 , 40.0 };
		R[3].color = olc::GREEN;

		R[4].pos = { 150.0 , 70.0 };
		R[4].size = { 10.0 , 10.0 };
		R[4].color = olc::GREEN;

		R[5].pos = { 190.0 , 20.0 };
		R[5].size = { 10.0 , 20.0 };
		R[5].color = olc::GREEN;


		return true;
	}



	bool OnUserUpdate(float fElapsedTime) override
	{
		//------------------------Calculos-----------------------
		olc::vf2d Mouse = { float(GetMouseX()), float(GetMouseY()) };
		LastPos = R[0].pos;

		//R[0].pos = Mouse;

		/*if (RectVSRect(R[0].pos.x, R[0].pos.y, R[0].size.x, R[0].size.y,
					   R[1].pos.x, R[1].pos.y, R[1].size.x, R[1].size.y)) R[1].color = olc::BLUE;
		else R[1].color = olc::GREEN;*/

		MovePlayer();
		ApplyVel();
		if (GetKey(olc::UP).bPressed) R[0].pos.y--;
		if (GetKey(olc::RIGHT).bPressed) R[0].pos.x++;

		//Resolver Colision
		for (int i = 1; i < R.size(); i++) {
			ResolveColision(i);
		}

		//------------------------Dibujado-----------------------
		Clear(olc::BLACK);
		DrawObjects();

		return true;
	}
};

int main()
{
	Test demo;

	if (demo.Construct(380, 240, 2, 2)) demo.Start();

	return 0;
}
