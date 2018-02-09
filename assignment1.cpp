#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
//g++ -o foo assignment1.cpp -lglut -lGLU -lGL -lm 
bool clicked=false;
int xi, yi;
float float_abs(float i){
    if(i<0){
        i=i*-1;
    }
    return i;

}

void draw_Line(int start_x,int start_y,int end_x,int end_y){
        glColor3f(1,0,0);

        float change_y=end_y-start_y;
        float change_x=end_x-start_x;
        float change_error=0;
        float myError=0;
        bool vertical=false;
        int myY=start_y;

        if(change_x!=0){
            change_error=float_abs(change_y/change_x);
        }
        else{
            vertical=true;

        }


        if(change_error<1 && !vertical){
           
            if(change_x > 0){


                for(int myX=start_x;myX<end_x;myX++){

                    glVertex2i(myX,myY);
                    myError=change_error+myError;

                    while(myError>=0.5){
                        myY=myY+(change_y/float_abs(change_y));
                        myError=myError-1;
                    }


                }
           
            
            }//if its a positive slope

            else{

                for(int myX=start_x;myX>end_x;myX--){

                    glVertex2i(myX,myY);
                    myError=change_error+myError;

                    while(myError>=0.5){
                        myY=myY+(change_y/float_abs(change_y));
                        myError=myError-1;
                    }


                }



            }//if its a negative 
        } //if the slope is smaller than 1 and they are not vertical
        else if(hange_error>1 && !vertical){
            std::cout<<"hi\n";
            

        }

       
        
}
void movement(int x, int y){

//glutGet(GLUT_WINDOW_HEIGHT)-
        if(clicked){
            int new_y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            glBegin(GL_POINTS);
            draw_Line(xi,yi,x,new_y);
            glEnd();
            glFlush();

        }


}

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        clicked=!clicked;
        xi=x;
        yi=glutGet(GLUT_WINDOW_HEIGHT)-y;
	}

}
 
 
int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

 glLoadIdentity();
 glMatrixMode( GL_PROJECTION );
 glutInitWindowSize(400,400);
 glutInitWindowPosition(100,100);
 glutCreateWindow("My Window");
 glutPassiveMotionFunc(movement);
 glutMouseFunc(mouse);
 gluOrtho2D(0,400,0,400);
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutMainLoop();

 return 0;
 
}
