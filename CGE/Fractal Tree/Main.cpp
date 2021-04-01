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


//Function that rotates angle by certain degree, takes the degree and the angle as parameters
float Rotate(float degree, float angle){
    float n_angle=0.0f;
    //If the degree is in between 360 or -360 it considers to rotate, if it exceeds those values it returns 0
    if (degree<=180.0f or degree>=(-180.0f)){
        //If the angle minus the degree is less than 0 the angle will be equal to the angle minus the degree plus 360, this avoids the function to return negative values.
        if (angle-degree<0.0f){
            n_angle =(angle-degree)+360.0f;
        }
        //The same thing but for it exceeding 360 degrees
        else if(angle-degree>360.0f ){
            n_angle= (angle-degree)-360.0f;
        }
        //If it do not tends to exceed any of those values it does the normal substraction
        else n_angle=angle-degree;
    }
    //Returns the angle
    return n_angle;
}

class Juego : public olcConsoleGameEngine
{
private:
    
    //
    void Branch(int x, int y, float angle, int lenght){
        if (angle<=360 and angle>=0){
            //Convert from degrees to radians for c++ format compatibility
            angle=(angle*M_PI)/180;
            
            //Finds the next x and y values using Pythagoras theorem
            float x1=x+round((cos(angle)*lenght));
            float y1=y-(sin(angle)*lenght);
            
            //Draws a line between those coordinates
            DrawLine(x, y, (int)x1, (int)y1, PIXEL_SOLID);
        }
    }
    
    int x1;
    int y1;
    float angle;
    float lenght;
    //Degree value is modifiable
    int degree=45;
    //Proportion of the lenght on every iteration (modifiable, has to be less than 1)
    float const lenght_proportion=0.66f;
    
    //Recursive function, it calls itself again to find each branch, has an exponential run time. It runs once every frame.
    void Draw(float lenght, int x1, int y1){
        //Does this as long as the value of the lenght is more than some value, in this case it's 1, the lesser the value, the larger the fractal tree is and the slower the program will be
        if (lenght>1){
        Branch(x1,y1,angle,lenght);
        //Establishes the x and y coordinates on the x and y coordinates of the end of the branch
        x1=(int)(x1+(cos((angle*M_PI)/180)*lenght));
        y1=(int)(y1-(sin((angle*M_PI)/180)*lenght));
            
        //Saves the current angle to avoid recursive issues with the angle, as it controls almost the entire function
            float angle_n=angle;
            
        //Calls recursion
        angle=Rotate(degree, angle);
        Draw(lenght*lenght_proportion, x1, y1);
            
            
        //Remembers the original angle
            angle=angle_n;
        angle=Rotate(-degree, angle);
        //Calls recursion again
        Draw(lenght*lenght_proportion, x1, y1);
        }
        
    }
    
public:
    Juego(){}
    
    virtual bool OnUserCreate(){
        return true;
    }
    
    virtual bool OnUserUpdate(float fElapsedTime){
        //Every frame the values are restored
        x1=640;
        y1=720;
        //Lenght and angle are modifiable
        angle=90.0f;
        lenght=150.0f;
        
        if(GetKey(VK_RIGHT).bHeld) degree=degree+1;
        else if(GetKey(VK_LEFT).bHeld) degree=degree-1;
        
        if(degree>180.0f) degree=180.0f;
        else if (degree<-180.0f) degree=-180.0f;
        
        //Fills screen with black
        Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
        
        //Calls Draw function
        Draw(lenght, x1, y1);
        return true;
    }
    
    
};

int main() {
    
    Juego Demo;
    //Constructed on a pixel console
    Demo.ConstructConsole(1280, 720, 1, 1);
    Demo.Start();
}
