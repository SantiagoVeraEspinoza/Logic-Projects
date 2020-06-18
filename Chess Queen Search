#include <iostream>
#include <vector>
#include <math.h>
#include <ctime>
#include <unistd.h>
#include <conio.h>
using namespace std;

bool encontrarfalla(int a, int b, int w)
//Funcion para encontrar falla
	{
	int x1= a-((a/w)*w);
	int y1= a/w;
	int x2= b-((b/w)*w);
	int y2= b/w;
	bool falla;
	
	if ((x1+y1)==(x2+y2) or (x1-y1)==(x2-y2))
		{
		falla = false;
		}
		
	else 
		{
		falla = true;
		}
		return falla;
	}
	
int main()
	{
	int tablero = 8;
	int j;
	int u=0;
	int h=0;
	int a=0;
	int cont=0;
	int combinaciones =pow((tablero*tablero),2);
	string ans;
	vector <int> reina;
	vector <int> num;
	
		do{
		usleep (0.001);
		//Limpiar variables
		num.clear();
		reina.clear();
		u=0;
		j=0;
			
		cont=0;
		for (int i=0; i<tablero;i++)
			{
			srand (a);
			num.push_back(i);
			}
		//Generar numeros aleatorios sin que se repita x o y
		for (int i=0; i<tablero;i++)
			{
			if (num.size()>0)
				{
				int indice= rand()%num.size();
				reina.push_back((i*tablero)+num[indice]);
				num.erase((num.begin()+indice));
				}
			}
			
	for(u=0; u<tablero; u++){
			h=0;
			for(h=0; h<tablero; h++){
				if(encontrarfalla(reina [u], reina [h], tablero)==false) cont++;
				if(h==u) cont--;
		}
		}
		
		cout << "\n\nHubo " << cont << " interposiciones";
		
		if (cont==0)
			{
			cout << "\n\n-------------------------\n\nRespuesta encontrada:\n\n";
			//Muestra los indices
			for(int b=0; b<tablero; b++){
				cout << "\n" << reina[b] << "\n";
				}
			//Pregunta si se muestra el resultado
			cout << "\n\nSe encontro 1 resultado correcto, mostrar coordenadas?\n\n";
			cin >> ans;
			if (ans=="Si" or ans=="SI" or ans=="si")
				{
				cout << "\n\nDe acuerdo, mostrando:\n\n";
				for (int o=0; o<tablero;o++)
					{
					cout << "Casilla (reina) " << o+1 << ":\nX= " << reina[o]-((reina[o]/tablero)*tablero) << " Y= " << reina[o]/tablero << "\n\n";
					}
					string resp;
					cin >> resp;
					}
					}
					
			a++;
		}while(a<(10*(pow(10, tablero))));
	}

