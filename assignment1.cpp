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

void midpoint_line(int start_x,int start_y,int end_x,int end_y){

    int myX = start_x;
    int myY = start_y;

    if(start_x>end_x){
        myX=end_x;
        myY=end_y;
        end_x=start_x;
        end_y=start_y;
        start_x=myX;
        start_y=myY;

    }

    float change_y=end_y-start_y;
    float change_x=end_x-start_x;
    float change=change_y-(change_x/2);
    float change2=change_x-(change_y/2);

    if((change_x<change_y)&&(start_y>end_y)){
        printf("work\n");
    }//if the change in y is larger than the change in x and start y is larger than end y

    glVertex2i(myX,myY);

    if(change_x>change_y){

    if(start_y<end_y){
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
    }//first and third 




    else{
        while(myX < end_x){
            printf("once\n");
            myX++;
            /*if(change > 0){
                change=change-change_y;

            }
            else{
                change=change+change_y+change_x;
                myY--;

            }*/
            glVertex2i(myX,myY);

        }
        //fourth wo swap
        //second with swap
    }//draws horizontals
    }//plot using x



    else{
        if(start_y<end_y){
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


        }//first and third


        else{

            while(myY>end_y){
               myY--;
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


    }//plot using y



}

void movement(int x, int y){
//glutGet(GLUT_WINDOW_HEIGHT)-
        if(clicked){
            int new_y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            glBegin(GL_POINTS);
            //brensenham_line(xi,yi,x,new_y);
            midpoint_line(xi,yi,x,new_y);
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
 gluOrtho2D(0,1000,0,1000);
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutMainLoop();

 return 0;
 
}
