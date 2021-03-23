#define OLC_PGE_APPLICATION
#define M_PI 
#include "olcPixelGameEngine.h"
#include <math.h>

class Test : public olc::PixelGameEngine
{
private:
	//Variables de simulación
	int cont = 0;
	float TimeSinceStart;
	float distelev;
	bool alterpos = false;

	//Variables de control
	const float LimitFPS = 60.0f;
	bool Pause = false;

	//Estructuras
	struct Charge {
		olc::vf2d pos;
		float q;
		float rad = 10.0f;
		olc::vf2d forcepol;
		olc::vf2d forcerect;
		bool dir;
		int ID;
	};

	struct MenuCircle {
		olc::vf2d pos;
		float rad;
		olc::Pixel color;
	};

	struct MenuRect {
		olc::vf2d pos;
		olc::vf2d size;
		olc::Pixel color;
	};

	//Vectores
	std::vector <Charge> Q;
	std::vector <MenuCircle> C;
	std::vector <MenuRect> R;

	//Variables menus
	olc::vf2d Menu;
	olc::vf2d PlusBtn;
	olc::vf2d MinusBtn;
	olc::vf2d AddBall;
	olc::vf2d ReduceBall;
	std::string TextoCarga;
	std::string TextoFuerza;


public:
	Test()
	{
		sAppName = "Colisiones";
	}



public:

	void AddCharge() {
		Q.push_back({});
		cont = Q.size() - 1;
		Q[cont].pos = { float(ScreenWidth()) / 2,float(ScreenHeight()) / 2 };

		//Evitar que dos cargas se muevan muy rapido por superposición
		if (alterpos) {
			Q[cont].pos.x++;
			Q[cont].pos.y--;
			alterpos = false;
		}
		else {
			Q[cont].pos.x--;
			Q[cont].pos.y++;
			alterpos = true;
		}

		Q[cont].q = 0;
		Q[cont].ID = cont;
	}
	void DeleteCharge() {
		if (cont >= (R.size() - 1)) cont--;
		if(Q.size() > 1) Q.pop_back();
	}

	bool PointVSRect(olc::vf2d Point, olc::vf2d Rectpos, olc::vf2d Rectsize) {
		if (Point.x >= Rectpos.x and Point.x <= Rectpos.x + Rectsize.x and 
			Point.y >= Rectpos.y and Point.y <= Rectpos.y + Rectsize.y){
			//std::cout << "Colision" << std::endl;
			return true;
		}
		else return false;
	}

	bool PointVsCircle(olc::vf2d Point, olc::vf2d Circle, float rad) {
		float dist = ((Point.x - Circle.x)* (Point.x - Circle.x)) + ((Point.y - Circle.y) * (Point.y - Circle.y));

		if (dist < rad*rad) return true;
		else return false;
	}

	void ResolveCollision(Charge& Q1, Charge& Q2) {
		float varx = Q1.pos.x - Q2.pos.x;
		float vary = Q1.pos.y - Q2.pos.y;
		float dist = abs((varx * varx) + (vary * vary));
		//std::cout << Q1.ID << "<->" << Q2.ID << "=" << dist << std::endl;
		//std::cout << ((rad * 2) * (rad * 2)) << std::endl;
		if (dist <= ((Q1.rad+Q2.rad) * (Q1.rad+Q2.rad))) {
			dist = pow(dist, 0.5);
			float gap = Q1.rad + Q2.rad - dist;

			float correctx = (varx * gap) / (2 * dist);
			float correcty = (vary * gap) / (2 * dist);

			Q1.pos.x += correctx;
			Q1.pos.y += correcty;
			Q2.pos.x -= correctx;
			Q2.pos.y -= correcty;
		}
	}

	void DrawVertical(olc::vf2d Pos, olc::Pixel p, float rad=10.0f) {
		float y1 = Pos.y - rad;
		float y2 = Pos.y + rad;

		DrawLine(Pos.x, y1, Pos.x, y2, p);
	}

	void DrawHorizontal(olc::vf2d Pos, olc::Pixel p, float rad=10.0f) {
		float x1 = Pos.x - rad;
		float x2 = Pos.x + rad;

		DrawLine(x1, Pos.y, x2, Pos.y, p);
	}
	void CalculateForce(Charge& Obj1, Charge& Obj2) {
		float varx = (Obj1.pos.x - Obj2.pos.x);
		float vary = (Obj1.pos.y - Obj2.pos.y);
		float m = vary / varx;
		float angle;

		distelev = ((varx * varx) + (vary * vary))/100.0f;

		Obj1.forcepol.x = (9 * Obj1.q * Obj2.q)/(distelev*8);
		Obj2.forcepol.x = Obj1.forcepol.x;

		//std::cout << Obj1.forcepol.x << std::endl;

		if (varx > 0 and vary < 0) { //Cuadrante 1
			Obj1.forcepol.y = atan(vary / varx);
			Obj1.forcerect.x = -Obj1.forcepol.x*(cos(Obj1.forcepol.y));
			Obj1.forcerect.y = -Obj1.forcepol.x*(sin(Obj1.forcepol.y));
		}
		else if (varx < 0 and vary < 0) { //Cuadrante 2
			Obj1.forcepol.y = 3.1416 - atan(abs(vary / varx));
			Obj1.forcerect.x = -Obj1.forcepol.x * (cos(Obj1.forcepol.y));
			Obj1.forcerect.y = Obj1.forcepol.x * (sin(Obj1.forcepol.y));
		}
		else if (varx < 0 and vary > 0) { //Cuadrante 3
			Obj1.forcepol.y = -(3.1416 - atan(abs(vary / varx)));
			Obj1.forcerect.x = -Obj1.forcepol.x * (cos(Obj1.forcepol.y));
			Obj1.forcerect.y = Obj1.forcepol.x * (sin(Obj1.forcepol.y));
		} 
		else if (varx > 0 and vary > 0) { //Cuadrante 4
			Obj1.forcepol.y = -atan(abs(vary / varx));
			Obj1.forcerect.x = -Obj1.forcepol.x * (cos(Obj1.forcepol.y));
			Obj1.forcerect.y = Obj1.forcepol.x * (sin(Obj1.forcepol.y));
		}
		else {
			return;
		}

		MoveCircle(Obj1, Obj2);

		//std::cout << Obj1.forcerect.x << " - " << Obj1.forcerect.y << std::endl;
	}

	void MoveCircle(Charge& Obj1, Charge Obj2) {
		//std::cout << Obj1.q * Obj2.q << std::endl;
		if ((Obj1.q * Obj2.q) < 0.0f) {
			Obj1.pos.x = Obj1.pos.x - Obj1.forcerect.x;
			Obj1.pos.y = Obj1.pos.y - Obj1.forcerect.y;
		}
		else if ((Obj1.q * Obj2.q) > 0.0f) {
			Obj1.pos.x = Obj1.pos.x - Obj1.forcerect.x;
			Obj1.pos.y = Obj1.pos.y - Obj1.forcerect.y;
		}
		else return;	
	}

public:

	void SetValues() {
		//Limpia el vector
		Q.clear();

		//Establecer valores
		Q.push_back({});
		Q[0].pos.x = ScreenWidth() / 3;
		Q[0].pos.y = ScreenHeight() / 3;
		Q[0].q = 2;
		Q[0].ID = 0;

		Q.push_back({});
		Q[1].pos.x = ScreenWidth() / 2;
		Q[1].pos.y = ScreenHeight() / 2;
		Q[1].q = -2;
		Q[1].ID = Q[0].ID + 1;


		Q.push_back({});
		Q[2].pos.x = (2 * ScreenWidth()) / 3;
		Q[2].pos.y = (2 * ScreenHeight()) / 3;
		Q[2].q = 2;
		Q[2].ID = Q[1].ID + 1;

		//Cambiar el valor de cont para evtiar acceder a vectores no existentes
		cont = (Q.size()-1);
	}

	void DrawMenu() {
		FillRect(Menu.x, 0.0f, float(ScreenWidth()), Menu.y, olc::Pixel(84, 153, 143));
		DrawLine(Menu.x, 0.0f, Menu.x, Menu.y, olc::WHITE);

		for (auto& Circle : C) {
			FillCircle(Circle.pos, Circle.rad, Circle.color);
			DrawHorizontal(Circle.pos, olc::Pixel(120, 137, 235));
		}
		DrawVertical(C[0].pos, olc::Pixel(120, 137, 235));

		for (auto& Rect : R) {
			FillRect(Rect.pos, Rect.size, Rect.color);
		}
		
		DrawString(ScreenWidth()-52.0f, 90.0f, "Carga:", olc::WHITE);

		DrawString(ScreenWidth() - 56.0f, 130.0f, "Fuerza:", olc::WHITE);
		
		if(Q[cont].q >=0) DrawString(ScreenWidth() - 35, 100.0f, TextoCarga, olc::WHITE);
		else DrawString(ScreenWidth() - 40.0f, 100.0f, TextoCarga, olc::WHITE);

		if(Q[cont].q !=0) DrawString(ScreenWidth() - 58.0f, 110.0f, "x10^9 C", olc::WHITE);
		else DrawString(ScreenWidth() - 26.0f, 100.0f, "C", olc::WHITE);

		olc::vf2d Line = { float(ScreenWidth())-30, 124.0f };
		DrawHorizontal(Line, olc::WHITE, 30.0f);

		DrawString(ScreenWidth() - 54.0f, 140.0f, TextoFuerza, olc::WHITE);
		FillRect(float(ScreenWidth()) - 6.0, 140.0f, 8.0f, 8.0f, olc::Pixel(84, 153, 143));
		DrawString(ScreenWidth() - 35.0f, 150.0f, "N", olc::WHITE);
	}

	bool OnUserCreate() override
	{
		//Menus y texto
		C.push_back({});
		C.push_back({});
		R.push_back({});
		R.push_back({});

		Menu = {ScreenWidth() - 60.0f, float(ScreenHeight())};
		C[0].pos = PlusBtn = { ScreenWidth() - 30.0f, 20.0f };
		C[1].pos = MinusBtn = { ScreenWidth() - 30.0f, 55.0f };
		C[0].rad = C[1].rad = 15.0f;

		R[0].pos = { float(ScreenWidth())-57.0f, float(ScreenHeight()) - 40.0f };
		R[1].pos = { float(ScreenWidth())-57.0f, float(ScreenHeight()) - 20.0f };
		R[0].size = R[1].size = {55.0, 18.0};


		//Vectores

		Q.push_back({});
		Q.push_back({});
		Q.push_back({});

		SetValues();

		return true;
	}

public:


	

	bool OnUserUpdate(float fElapsedTime) override
	{
		//------------------------Calculos-----------------------

		//Limitar FPS
		double dExpectedTime = 1.0 / LimitFPS;
		if (dExpectedTime >= GetElapsedTime()) Sleep((dExpectedTime - GetElapsedTime()) * 1000);
		
		//Limpiar el texto de las cargas (x10^9 Coulombs)
		TextoCarga.clear();
		TextoFuerza.clear();

		//TimeSinceStart = TimeSinceStart + GetElapsedTime();
		//Arreglo 2D del mouse
		olc::vf2d Mouse = { float(GetMouseX()), float(GetMouseY()) };

		//Método de pausa
		if (GetKey(olc::P).bPressed and !Pause) Pause = true;
		else if (GetKey(olc::P).bPressed and Pause) Pause = false;

		//Cambiar de carga
		if (GetKey(olc::Z).bPressed) cont++;
		if (cont >= Q.size())cont = 0;
		if (Mouse.x < Menu.x) {
			for (auto& Charge : Q) {
				if (PointVsCircle(Mouse, Charge.pos, Charge.rad)) cont = Charge.ID;
			}
		}

		//Cambiar signo de la carga
		if (GetKey(olc::C).bPressed) Q[cont].q = Q[cont].q * -1;

		
		//Seguir al mouse
		if (GetMouse(0).bHeld and GetMouseX()<Menu.x) {
			Q[cont].pos.x = GetMouseX();
			Q[cont].pos.y = GetMouseY();
		}

		//Checar colisiones en botones circulares
		if (PointVsCircle(Mouse, C[0].pos, C[0].rad)) {
			C[0].color = olc::Pixel(230, 176, 149);
			if (GetMouse(0).bPressed and Q[cont].q < 9) {
				Q[cont].q++;
			}
			if (GetMouse(0).bHeld) {
				C[0].color = olc::Pixel(153, 107, 84);
			}
		}
		else C[0].color = olc::Pixel(250, 217, 201);

		if (PointVsCircle(Mouse, C[1].pos, C[1].rad)) {
			C[1].color = olc::Pixel(230, 176, 149);
			if (GetMouse(0).bPressed and Q[cont].q > -9) {
				Q[cont].q--;
			}
			if (GetMouse(0).bHeld) {
				C[1].color = olc::Pixel(153, 107, 84);
			}
		} 
		else C[1].color = olc::Pixel(250, 217, 201);

		//Aumentar y disminuir la carga con teclas
		if(GetKey(olc::RIGHT).bPressed and Q[cont].q < 9) Q[cont].q++;
		else if (GetKey(olc::LEFT).bPressed and Q[cont].q > -9) Q[cont].q--;

		//Checar colisiones en botones rectangulares
		if (PointVSRect(Mouse, R[0].pos, R[0].size)) {
			//std::cout << "Colision" << std::endl;
			R[0].color = olc::Pixel(230, 176, 149);
			if (GetMouse(0).bPressed) {
				AddCharge();
			}
			if (GetMouse(0).bHeld) {
				R[0].color = olc::Pixel(153, 107, 84);
			}
		}
		else R[0].color = olc::Pixel(250, 217, 201);

		if (PointVSRect(Mouse, R[1].pos, R[1].size)) {
			//std::cout << "Colision" << std::endl;
			R[1].color = olc::Pixel(230, 176, 149);
			if (GetMouse(0).bPressed) {
				DeleteCharge();
			}
			if (GetMouse(0).bHeld) {
				R[1].color = olc::Pixel(153, 107, 84);
			}
		}
		else R[1].color = olc::Pixel(250, 217, 201);

		//Añadir y eliminar cargas con teclas
		if (GetKey(olc::UP).bPressed) AddCharge();
		else if (GetKey(olc::DOWN).bPressed) DeleteCharge();

		//Calcular fuerza y resolver colisiones
		if (!Pause) {
			for (auto& Q1 : Q) {
				for (auto& Q2 : Q) {
					if (Q1.ID != Q2.ID) {
						CalculateForce(Q1, Q2);
						ResolveCollision(Q1, Q2);
					}
				}
			}
		}
		
		//Reestablecer valores predetermindaos
		if (GetKey(olc::R).bPressed) SetValues();

		//Cambiar el valor de los textos
		TextoCarga += std::to_string(int(Q[cont].q));
		TextoFuerza += std::to_string(abs(Q[cont].forcepol.x));


		//------------------------Dibujado-----------------------
		FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

		for (auto& Charge : Q) {
			if (Charge.ID != cont) {
				DrawCircle(Charge.pos, Charge.rad, olc::GREEN);
				
				if (Charge.q > 0) DrawVertical(Charge.pos, olc::Pixel(0, 255, 0));
				if (Charge.q != 0) DrawHorizontal(Charge.pos, olc::Pixel(0, 255, 0));
			}
			else {
				DrawCircle(Charge.pos, Charge.rad, olc::RED);
				
				if (Charge.q > 0) DrawVertical(Charge.pos, olc::Pixel(255, 0, 0));
				if (Charge.q != 0) DrawHorizontal(Charge.pos, olc::Pixel(255, 0, 0));
			}
			

		}
		DrawMenu();

		return true;
	}
};

int main()
{
	Test demo;

	if (demo.Construct(480, 270, 2, 2)) demo.Start();

	return 0;
}
