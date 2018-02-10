#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
//g++ -o foo assignment1.cpp -lglut -lGLU -lGL -lm 
bool clicked=false;
bool stipple=false;
bool b_line=true;
bool mid_line=false;
int xi, yi;
float float_abs(float i){
    if(i<0){
        i=i*-1;
    }
    return i;

}

void brensenham_line(int start_x,int start_y,int end_x,int end_y){
        glColor3f(1,0,0);

        float change_y=end_y-start_y;
        float change_x=end_x-start_x;
        float change_errorx=0;
        float change_errory=0;
        float myError=0;
        bool vertical=false;
        int myY=start_y;
        int myX=start_x;
        if(change_y!=0){

            change_errory=float_abs(change_x/change_y);
        }//prevents horizontal y things

        if(change_x!=0){
            change_errorx=float_abs(change_y/change_x);
        }
        else{
            vertical=true;

        }

        if(change_errorx<1 && !vertical){
           
            if(change_x > 0){


                for(int myX=start_x;myX<end_x;myX++){

                    glVertex2i(myX,myY);
                    myError=change_errorx+myError;

                    while(myError>=0.5){
                        myY=myY+(change_y/float_abs(change_y));
                        myError=myError-1;
                    }


                }
           
            
            }//if its a positive slope

            else{

                for(int myX=start_x;myX>end_x;myX--){

                    glVertex2i(myX,myY);
                    myError=change_errorx+myError;

                    while(myError>=0.5){
                        myY=myY+(change_y/float_abs(change_y));
                        myError=myError-1;
                    }


                }



            }//if its a negative 
        } //if the slope is smaller than 1 and they are not vertical plot using x
        else {
            if(change_y > 0){

                for(int myY=start_y;myY<end_y;myY++){

                    glVertex2i(myX,myY);
                    myError=change_errory+myError;

                    while(myError>=0.5){
                        myX=myX+(change_x/float_abs(change_x));
                        myError=myError-1;
                    }


                }

            }//if the change in y is positive 
            
            else{

                for(int myY=start_y;myY>end_y;myY--){

                    glVertex2i(myX,myY);
                    myError=change_errory+myError;

                    while(myError>=0.5){
                        myX=myX+(change_x/float_abs(change_x));
                        myError=myError-1;
                    }


                }


            }
            

        }//otherwise plot using y      
}

void pospos(int start_x,int start_y,int end_x,int end_y){
 int myX = start_x;
 int myY = start_y;
 float change_y=end_y-start_y;
 float change_x=end_x-start_x;
 float change=change_y-(change_x/2);
 float change2=change_x-(change_y/2);
 change = 2*change_y;
 change2 = 2* (change_y-change_x);
 if(change_x>change_y){
    while(myX < end_x){
            myX++;
            if(change < 0){
                change=change+change_y;
            }
            else{
                change=change+change_y-change_x;
                myY++;
            }
            glVertex2i(myX,myY);
    }

 }
 else{
    while(myY<end_y){
                myY++;
                if(change2<0){
                    change2=change2+change_x;

                }
                else{
                    change2=change2+change_x-change_y;
                    myX++;
                }
                 glVertex2i(myX,myY);
    }



 }
}

void negpos(int start_x,int start_y,int end_x,int end_y){
    int change_x = end_x - start_x;//change in x
    int change_y = -1 * (end_y - start_y);//change in y
    float change = 2 * change_y - change_x;//decs

    float E = 2 * change_y;//east
    float NE = 2 * (change_y - change_x);//"north east"

    float myX=start_x;
    float myY=start_y;
    float slope=0;
    
    if(change_x != 0){
        slope = change_y/change_x;
    }//if its not a horizontal line


    glVertex2f(myX,myY);//draw the first pixel

    if(change_x>change_y){
       
        float x_sep = 1;//distance between pixels
        float y_sep = 1;//distance between pixels
            
            while(myX < end_x){
                if(change <= 0){
                    myX+= x_sep;
                    change += E;

                }//draw side
                else{
                    change += NE;
                    myX+= x_sep;
                    myY-= y_sep;
       
                }//draw down
                glVertex2f(myX,myY);
            }
    }//plot using x

    else{
        
        float draw_rate = 1000;//how many pixels do i draw
        float x_sep = change_x/draw_rate;
        float y_sep = -(change_y/draw_rate);
            
            while(myX < end_x){
                if(change <= 0){
                    change += E;
                    myX+= x_sep;
                }//draw side
                else{
                    change += NE;
                    myX += x_sep;
                    myY += y_sep;
                }//draw down

                glVertex2f(myX,myY);
            }
    }//plot usig y

            
}


void midpoint_line(int start_x,int start_y,int end_x,int end_y){
    glColor3f(0,1,0);

    int x_quad=end_x-start_x;
    int y_quad=end_y-start_y;

    if((x_quad>0)&&(y_quad>0)){
        pospos(start_x,start_y,end_x,end_y);
    }//quad1
    else if((x_quad<0)&&(y_quad>0)){
        negpos(end_x,end_y,start_x,start_y);
        
    }//quad2
    else if((x_quad<0)&&(y_quad<0)){
        pospos(end_x,end_y,start_x,start_y);
    }//quad3
    else if((x_quad>0)&&(y_quad<0)){
        negpos(start_x,start_y,end_x,end_y);



    }//quad4
   



}




void movement(int x, int y){
//glutGet(GLUT_WINDOW_HEIGHT)-
        if(clicked){
            int new_y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            glBegin(GL_POINTS);
            if(b_line){
                brensenham_line(xi,yi,x,new_y);
            }
            else if(mid_line){
            midpoint_line(xi,yi,x,new_y);
            }
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
    if(bin == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        xi=x;
        yi=glutGet(GLUT_WINDOW_HEIGHT)-y;
    }
}
 
 
int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

 glLoadIdentity();
 glMatrixMode( GL_PROJECTION );
 glutInitWindowSize(1000,1000);
 glutInitWindowPosition(100,100);
 glutCreateWindow("My Window");
 glutPassiveMotionFunc(movement);
 glutMouseFunc(mouse);
 glutDisplayFunc(show);
 gluOrtho2D(0,1000,0,1000);
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutMainLoop();

 return 0;
 
}
