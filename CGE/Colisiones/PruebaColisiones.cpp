//
//  main.cpp
//  Prueba Colisiones
//
//  Created by Santiago Vera on 06/08/20.
//  Copyright © 2020 Mistic_Lion. All rights reserved.
//

#include <iostream>
#include <vector>
#include "olcConsoleGameEngineSDL.h"
using namespace std;

class Demo : public olcConsoleGameEngine{
private:
    
    float m=0;
    float var_ind=0;
    
    float P1_x=50.0;
    float P1_y=50.0;
   
    
public:
    
    Demo(){
        
    }
    
    ~Demo(){
        
    }
    
    void CreateFunction(float x1, float y1, float x2, float y2){
        m=((y2-y1)/(x2-x1));
        var_ind=(-1*x1*m)+y1;
        
        cout << "Tu funcion es: \ny=" << m << "x+(" << var_ind  << ")";
        
    }
    
    bool DrawFunction(float x1, float y1, float x2, float y2){
        float y;
        CreateFunction(x1, y1, x2, y2);
        
        for(int x=0; x<ScreenWidth(); x++){
            for(int y=0; y<ScreenHeight(); y++){
                
                if(y==(int)((m*x)+var_ind) or x==(int)((y-var_ind)/m)) Draw(x, y, FG_GREEN);
            }
            y=(m*x)+var_ind;
            
            
        }
        
        
        return true;
    }
    
    virtual bool OnUserCreate(){
        return true;
    }
    
    virtual bool OnUserUpdate(float fElapsedTime){
        Fill(0, 0, ScreenWidth(), ScreenHeight(), FG_BLACK);
        
        if(GetMouse(0).bPressed){
            P1_x=GetMouseX();
            P1_y=GetMouseY();
        }
        
        DrawFunction(P1_x, P1_y, GetMouseX(), GetMouseY());
        
        Draw(P1_x, P1_y, FG_RED);
        return true;
    }
    
};

int main(){
    
    Demo Juego;
    
    Juego.CreateFunction(8, 7, 23, 67);
    
    if (Juego.ConstructConsole(240, 180, 1, 1)){
        Juego.Start();
    }
 
    
}
