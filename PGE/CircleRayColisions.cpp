#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

private:

	//Variables de control
	bool CanMoveCircle = false;
	bool MovmentPoint = false;

	//Variables de posición
	olc::vf2d p1 = { 10,20 };
	olc::vf2d p2 = { 30,5 };
	olc::vf2d B1 = { 0,0 };

	olc::vf2d Intersct1;
	olc::vf2d Intersct2;

	//Estructuras
	struct circle {
		olc::vf2d pos;
		float rad;
		olc::vf2d vel;
		olc::vf2d lastpos;
	};

	//Vectores
	std::vector<circle> ObjCircles;

public:

	void SetAtributes() {
		ObjCircles.clear();

		ObjCircles.push_back({});
		ObjCircles[0].pos = { (float)ScreenWidth()/2 , (float)ScreenHeight()/2 };
		ObjCircles[0].rad = 5;

		ObjCircles.push_back({});
		ObjCircles[1].pos = { (float)ScreenWidth() / 2 , (float)ScreenHeight() / 2 };
		ObjCircles[1].rad = 10;
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here

		SetAtributes();

		return true;
	}

//------------------------------------------------------------------------------------------------------------------------------------------------------

public:

	void SetCirclePos(olc::vf2d newpos, int circle) {
		ObjCircles[circle].pos = newpos;
	}

	bool PointVSCircle(olc::vf2d pointpos, circle target) {
		if ((pointpos - target.pos).mag() < target.rad) return true;
		else return false;
	}

	/*void LineVSCircle(olc::vf2d p1, olc::vf2d p2, circle target) {
		float Lineleght = (p1 - target.pos).mag();
		float varx = p1.x - target.pos.x;
		float vary = p1.y - target.pos.y;

		float m = (p1.y - p2.y) / (p1.x - p2.x);

		float ang = acos(target.rad / Lineleght);

		olc::vf2d B1 = { (varx * cos(ang) + vary * sin(ang))*target.rad/Lineleght + target.pos.x,
						 (vary * cos(ang) - varx * sin(ang))* target.rad / Lineleght + target.pos.y };
		olc::vf2d B2 = { (varx * cos(-ang) + vary * sin(-ang)) * target.rad / Lineleght + target.pos.x,
						 (vary * cos(-ang) - varx * sin(-ang)) * target.rad / Lineleght + target.pos.y };

		float m1 = (p1.y - B1.y) / (p1.x - B1.x);
		float m2 = (p1.y - B2.y) / (p1.x - B2.x);

		std::cout << m1 << " - " << m2 << " - " << m << std::endl;

		//std::cout << B1.x << " " << B1.y << std::endl;

		
	}*/
	// ↑ Función antigua con traslación de puntos

	bool LineVSCircle(olc::vf2d p1, olc::vf2d p2, circle target) {
		float varx = p1.x - p2.x;
		float vary = p1.y - p2.y;

		float A = -2 * target.pos.x;
		float B = -2 * target.pos.y;
		float C = target.pos.x * target.pos.x + target.pos.y * target.pos.y - target.rad * target.rad;

		if (varx != 0) {
			float m = vary / varx;

			float h = p1.x;
			float k = p1.y;
			float n = -m * h + k;

			float x2 = 1 + (m * m);
			float x = A + (2 * m * n) + (B * m);
			float ind = (n * n) + (B * n) + C;

			//std::cout << x2 << " " << x << " " << ind << std::endl;

			float delta = (x * x) - (4 * x2 * ind);

			//std::cout << -x << std::endl;

			if (delta >= 0) {
				//std::cout << "Colision" << std::endl;
				Intersct1.x = (-x + pow(delta, 0.5)) / (2 * x2);
				Intersct1.y = m * Intersct1.x + n;

				Intersct2.x = (-x - pow(delta, 0.5)) / (2 * x2);
				Intersct2.y = m * Intersct2.x + n;

				SortIntersectPoints();

				return true;
			}
			else {

				return false;
			}



			//std::cout << delta << std::endl;
		}
		else if (varx == 0 and p1.x >= target.pos.x - target.rad and p1.x <= target.pos.x + target.rad) {
			//std::cout << "Colision" << std::endl;
			float ind = p1.x * p1.x + p1.x * A + C;

			float delta = (B * B) - (4 * ind);
			Intersct1.x = p1.x;
			Intersct1.y = (-B + pow(delta, 0.5)) / (2);

			Intersct2.x = p1.x;
			Intersct2.y = (-B - pow(delta, 0.5)) / (2);

			SortIntersectPoints();

			return true;
		}
		else {

			return false;
		}

	}

	void SortIntersectPoints() {
		float dist1;
		float dist2;
		for (int i = 0; i < 2; i++) {
			dist1 = (p1 - Intersct1).mag();
			dist2 = (p1 - Intersct2).mag();
		}
		if (dist1 > dist2) {
			olc::vf2d temp = Intersct1;
			Intersct1 = Intersct2;
			Intersct2 = temp;
		}
	}

	void SetMovementPoints(olc::vf2d mouse) {
		/*if (GetKey(olc::P).bPressed and !MovmentPoint) MovmentPoint = true;
		else if (GetKey(olc::P).bPressed and MovmentPoint) MovmentPoint = false;*/

		if (GetMouse(0).bHeld) {
			p2 = mouse;
			/*if (MovmentPoint) p1 = mouse;
			else if (!MovmentPoint) p2 = mouse;*/
		}
	}

	void DrawMovementLine(olc::vf2d p1, olc::vf2d p2) {
		DrawLine(p1, p2, olc::WHITE);
		Draw(p1, olc::GREEN);
		Draw(p2, olc::RED);
	}

	void DrawCircles() {
		for (auto circle : ObjCircles) {
			DrawCircle(circle.pos, circle.rad, olc::WHITE);
		}
	}


	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called each frame, so update things here
		//-----------------------------------------
		//Cálculos
		olc::vf2d mouse = {(float)GetMouseX(), (float)GetMouseY()};

		p1 = ObjCircles[0].pos;

		for (auto &circle : ObjCircles) {
			circle.lastpos = circle.pos;
		}

		if (GetMouse(1).bPressed and PointVSCircle(mouse, ObjCircles[0])) CanMoveCircle = true;
		else if (GetMouse(1).bReleased) CanMoveCircle = false;
			
		if(CanMoveCircle) SetCirclePos(mouse, 0);

		SetMovementPoints(mouse);

		LineVSCircle(p1, p2, ObjCircles[1]);

		//-----------------------------------------
		//Dibujado
		Clear(olc::BLACK);

		DrawCircles();
		DrawMovementLine(p1, p2);

		if (LineVSCircle(p1, p2, ObjCircles[1])) {
			FillCircle(Intersct1, 1, olc::MAGENTA);
			FillCircle(Intersct2, 1, olc::BLUE);
		}

		//Draw(B1, olc::YELLOW);
		

		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 2, 2, false, false))
		demo.Start();
	return 0;
}

/*
Bibliografía:

Sangaku S.L. (2021) Intersección de una circunferencia y una recta.
sangakoo.com. Recuperado el 18/04/2021 de:
https://www.sangakoo.com/es/temas/interseccion-de-una-circunferencia-y-una-recta

*/
