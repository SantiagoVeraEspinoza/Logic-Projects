//
//  main.cpp
//  Fractal Tree
//
//  Created by Santiago Vera on 30/06/20.
//  Copyright © 2020 Mistic_Lion. All rights reserved.
//

#include <iostream>
#include <math.h>
#include "olcConsoleGameEngineSDL.h"
using namespace std;

float Rotate(float degree, float angle){
    float n_angle=0.0f;
    if (degree<=360.0f){
        if (angle-degree<0.0f){
            n_angle =(360.0+angle)-degree;
        }
        else if(angle-degree>360.0f){
           n_angle= (360.0-angle)-degree;
        }
        else n_angle=angle-degree;
    }
    return n_angle;
}

class Juego : public olcConsoleGameEngine
{
private:
    
    
    void Branch(int x, int y, float angle, int lenght){
        if (angle<=360 and angle>=0){
            angle=(angle*M_PI)/180;
            
            
            float x1=x+(cos(angle)*lenght);
            float y1=y-(sin(angle)*lenght);
            
            DrawLine(x, y, (int)x1, (int)y1, PIXEL_SOLID);
        }
    }
    
    int x1;
    int y1;
    float angle;
    float lenght;
    int degree=45;
    
    void Draw(float lenght, int x1, int y1){
        if (lenght>1){
        Branch(x1,y1,angle,lenght);
            
        x1=(int)(x1+(cos((angle*M_PI)/180)*lenght));
        y1=(int)(y1-(sin((angle*M_PI)/180)*lenght));
            
        int angle_2=angle;
            
        angle=Rotate(degree, angle);
        Draw(lenght*0.66, x1, y1);
            
        angle=angle_2;
        angle=Rotate(-degree, angle);
        Draw(lenght*0.66, x1, y1);
        }
    }
    
public:
    Juego(){}
    
    virtual bool OnUserCreate(){
        return true;
    }
    
    virtual bool OnUserUpdate(float fElapsedTime){
        x1=640;
        y1=720;
        angle=90.0f;
        lenght=100.0f;
        
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
        
        Draw(lenght, x1, y1);
        
        return true;
    }
    
    
};

int main() {
    
    Juego Demo;
    Demo.ConstructConsole(1280, 720, 1, 1);
    Demo.Start();
}
