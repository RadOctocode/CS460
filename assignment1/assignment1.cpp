#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <vector>
//g++ -o foo assignment1.cpp -lglut -lGLU -lGL -lm 
struct point{
    int str_x,str_y;
    int end_x,end_y;

};
bool clicked=false;
int style=0;
int dash=10;
int dot=5;
bool b_line=true;
bool mid_line=false;
int xi, yi;
int temp_x2,temp_y2;
point hold;



std::vector<point> lines_to_draw;

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
                     if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myX/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myX/dot)%3==0){
                    glVertex2i(myX,myY);
                }
                    myError=change_errorx+myError;

                    while(myError>=0.5){
                        myY=myY+(change_y/float_abs(change_y));
                        myError=myError-1;
                    }


                }
           
            
            }//if its a positive slope

            else{

                for(int myX=start_x;myX>end_x;myX--){

                     if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myX/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myX/dot)%3==0){
                    glVertex2i(myX,myY);
                }
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

                     if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myY/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myY/dot)%3==0){
                    glVertex2i(myX,myY);
                }   
                    myError=change_errory+myError;

                    while(myError>=0.5){
                        myX=myX+(change_x/float_abs(change_x));
                        myError=myError-1;
                    }


                }

            }//if the change in y is positive 
            
            else{

                for(int myY=start_y;myY>end_y;myY--){
                     if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myY/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myY/dot)%3==0){
                    glVertex2i(myX,myY);
                }    
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

             if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myX/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myX/dot)%3==0){
                    glVertex2i(myX,myY);
                }
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
                 if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myY/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myY/dot)%3==0){
                    glVertex2i(myX,myY);
                }
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

    if(slope==0){
        for(int i=start_y;i<end_y;i--){
            glVertex2i(myX,i);
        }
    }

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
                 if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myX/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myX/dot)%3==0){
                    glVertex2i(myX,myY);
                }
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

                if(style==0){
                    glVertex2i(myX,myY);
                }
                else if(style==1 && ((int)myY/dash)%3==0){
                    glVertex2i(myX,myY);
                }
                else if(style==2 && ((int)myY/dot)%3==0){
                    glVertex2i(myX,myY);
                }
            }
    }//plot usig y            
}


void midpoint_line(int start_x,int start_y,int end_x,int end_y){
    glColor3f(0,1,0);

    int x_quad=end_x-start_x;
    int y_quad=end_y-start_y;
    if(x_quad==0){
        for(int i=start_y;i<end_y;i++){
           if(style==0){
                    glVertex2i(start_x,i);
                }
                else if(style==1 && ((int)i/dash)%3==0){
                    glVertex2i(start_x,i);
                }
                else if(style==2 && ((int)i/dot)%3==0){
                    glVertex2i(start_x,i);
                }
        }


    }
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

void gl_line(int start_x,int start_y,int end_x,int end_y){
    glColor3f(0,0,1);

    glVertex2i(start_x,start_y);//draw the first pixel
    glVertex2i(end_x,end_y);

}

void wake_bren(){
    brensenham_line(70,glutGet(GLUT_WINDOW_HEIGHT)-33,129,glutGet(GLUT_WINDOW_HEIGHT)-161);
    brensenham_line(129,glutGet(GLUT_WINDOW_HEIGHT)-155,158,glutGet(GLUT_WINDOW_HEIGHT)-41);
    brensenham_line(158,glutGet(GLUT_WINDOW_HEIGHT)-41,180,glutGet(GLUT_WINDOW_HEIGHT)-164);
    brensenham_line(180,glutGet(GLUT_WINDOW_HEIGHT)-164,209,glutGet(GLUT_WINDOW_HEIGHT)-16);
    
    brensenham_line(215,glutGet(GLUT_WINDOW_HEIGHT)-156,246,glutGet(GLUT_WINDOW_HEIGHT)-34);
    brensenham_line(246,glutGet(GLUT_WINDOW_HEIGHT)-34,273,glutGet(GLUT_WINDOW_HEIGHT)-155);
    brensenham_line(227,glutGet(GLUT_WINDOW_HEIGHT)-107,268,glutGet(GLUT_WINDOW_HEIGHT)-96);
    
    brensenham_line(301,glutGet(GLUT_WINDOW_HEIGHT)-147,296,glutGet(GLUT_WINDOW_HEIGHT)-22);
    brensenham_line(299,glutGet(GLUT_WINDOW_HEIGHT)-88,350,glutGet(GLUT_WINDOW_HEIGHT)-32);
    brensenham_line(297,glutGet(GLUT_WINDOW_HEIGHT)-87,369,glutGet(GLUT_WINDOW_HEIGHT)-137);

    brensenham_line(420,glutGet(GLUT_WINDOW_HEIGHT)-30,382,glutGet(GLUT_WINDOW_HEIGHT)-30);
    brensenham_line(382,glutGet(GLUT_WINDOW_HEIGHT)-31,384,glutGet(GLUT_WINDOW_HEIGHT)-142);
    brensenham_line(384,glutGet(GLUT_WINDOW_HEIGHT)-142,428,glutGet(GLUT_WINDOW_HEIGHT)-140);
    brensenham_line(384,glutGet(GLUT_WINDOW_HEIGHT)-90,425,glutGet(GLUT_WINDOW_HEIGHT)-85);


}
void wake_mid(){
    midpoint_line(70,glutGet(GLUT_WINDOW_HEIGHT)-33,129,glutGet(GLUT_WINDOW_HEIGHT)-161);
    midpoint_line(129,glutGet(GLUT_WINDOW_HEIGHT)-155,158,glutGet(GLUT_WINDOW_HEIGHT)-41);
    midpoint_line(158,glutGet(GLUT_WINDOW_HEIGHT)-41,180,glutGet(GLUT_WINDOW_HEIGHT)-164);
    midpoint_line(180,glutGet(GLUT_WINDOW_HEIGHT)-164,209,glutGet(GLUT_WINDOW_HEIGHT)-16);
    
    midpoint_line(215,glutGet(GLUT_WINDOW_HEIGHT)-156,246,glutGet(GLUT_WINDOW_HEIGHT)-34);
    midpoint_line(246,glutGet(GLUT_WINDOW_HEIGHT)-34,273,glutGet(GLUT_WINDOW_HEIGHT)-155);
    midpoint_line(227,glutGet(GLUT_WINDOW_HEIGHT)-107,268,glutGet(GLUT_WINDOW_HEIGHT)-96);
    
    midpoint_line(301,glutGet(GLUT_WINDOW_HEIGHT)-147,296,glutGet(GLUT_WINDOW_HEIGHT)-22);
    midpoint_line(299,glutGet(GLUT_WINDOW_HEIGHT)-88,350,glutGet(GLUT_WINDOW_HEIGHT)-32);
    midpoint_line(297,glutGet(GLUT_WINDOW_HEIGHT)-87,369,glutGet(GLUT_WINDOW_HEIGHT)-137);

    midpoint_line(420,glutGet(GLUT_WINDOW_HEIGHT)-30,382,glutGet(GLUT_WINDOW_HEIGHT)-30);
    midpoint_line(382,glutGet(GLUT_WINDOW_HEIGHT)-31,384,glutGet(GLUT_WINDOW_HEIGHT)-142);
    midpoint_line(384,glutGet(GLUT_WINDOW_HEIGHT)-142,428,glutGet(GLUT_WINDOW_HEIGHT)-140);
    midpoint_line(382,glutGet(GLUT_WINDOW_HEIGHT)-90,425,glutGet(GLUT_WINDOW_HEIGHT)-85);

}
void wake_gl(){
    gl_line(770,glutGet(GLUT_WINDOW_HEIGHT)-33,829,glutGet(GLUT_WINDOW_HEIGHT)-161);
    gl_line(129+700,glutGet(GLUT_WINDOW_HEIGHT)-155,158+700,glutGet(GLUT_WINDOW_HEIGHT)-41);
    gl_line(158+700,glutGet(GLUT_WINDOW_HEIGHT)-41,180+700,glutGet(GLUT_WINDOW_HEIGHT)-164);
    gl_line(180+700,glutGet(GLUT_WINDOW_HEIGHT)-164,209+700,glutGet(GLUT_WINDOW_HEIGHT)-16);
    
    gl_line(215+700,glutGet(GLUT_WINDOW_HEIGHT)-156,246+700,glutGet(GLUT_WINDOW_HEIGHT)-34);
    gl_line(246+700,glutGet(GLUT_WINDOW_HEIGHT)-34,273+700,glutGet(GLUT_WINDOW_HEIGHT)-155);
    gl_line(227+700,glutGet(GLUT_WINDOW_HEIGHT)-107,268+700,glutGet(GLUT_WINDOW_HEIGHT)-96);
    
    gl_line(301+700,glutGet(GLUT_WINDOW_HEIGHT)-147,296+700,glutGet(GLUT_WINDOW_HEIGHT)-22);
    gl_line(299+700,glutGet(GLUT_WINDOW_HEIGHT)-88,350+700,glutGet(GLUT_WINDOW_HEIGHT)-32);
    gl_line(297+700,glutGet(GLUT_WINDOW_HEIGHT)-87,369+700,glutGet(GLUT_WINDOW_HEIGHT)-137);

    gl_line(420+700,glutGet(GLUT_WINDOW_HEIGHT)-30,382+700,glutGet(GLUT_WINDOW_HEIGHT)-30);
    gl_line(382+700,glutGet(GLUT_WINDOW_HEIGHT)-31,384+700,glutGet(GLUT_WINDOW_HEIGHT)-142);
    gl_line(384+700,glutGet(GLUT_WINDOW_HEIGHT)-142,428+700,glutGet(GLUT_WINDOW_HEIGHT)-140);
    gl_line(384+700,glutGet(GLUT_WINDOW_HEIGHT)-90,425+700,glutGet(GLUT_WINDOW_HEIGHT)-85);
}


void coord_bren(){
    brensenham_line(97,glutGet(GLUT_WINDOW_HEIGHT)-293,97,glutGet(GLUT_WINDOW_HEIGHT)-197);
    brensenham_line(97,glutGet(GLUT_WINDOW_HEIGHT)-287,170,glutGet(GLUT_WINDOW_HEIGHT)-319);
    brensenham_line(96,glutGet(GLUT_WINDOW_HEIGHT)-289,31,glutGet(GLUT_WINDOW_HEIGHT)-329);


}
void coord_mid(){
    midpoint_line(97,glutGet(GLUT_WINDOW_HEIGHT)-293,97,glutGet(GLUT_WINDOW_HEIGHT)-197);
    midpoint_line(97,glutGet(GLUT_WINDOW_HEIGHT)-287,170,glutGet(GLUT_WINDOW_HEIGHT)-319);
    midpoint_line(96,glutGet(GLUT_WINDOW_HEIGHT)-289,31,glutGet(GLUT_WINDOW_HEIGHT)-329);

}
void coord_gl(){
    gl_line(97+700,glutGet(GLUT_WINDOW_HEIGHT)-293,97+700,glutGet(GLUT_WINDOW_HEIGHT)-197);
    gl_line(97+700,glutGet(GLUT_WINDOW_HEIGHT)-287,170+700,glutGet(GLUT_WINDOW_HEIGHT)-319);
    gl_line(96+700,glutGet(GLUT_WINDOW_HEIGHT)-289,31+700,glutGet(GLUT_WINDOW_HEIGHT)-329);

}

void movement(int x, int y){
//glutGet(GLUT_WINDOW_HEIGHT)-
        if(clicked){
            int new_y=y;
            hold.str_x=xi;
            hold.str_y=yi;
            hold.end_x=x;
            hold.end_y=glutGet(GLUT_WINDOW_HEIGHT)-y;

            glutPostRedisplay();

        }
}

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(!clicked){
            clicked=!clicked;
            xi=x;
            yi=glutGet(GLUT_WINDOW_HEIGHT)-y;

        }
        else{

            xi=x;
            yi=glutGet(GLUT_WINDOW_HEIGHT)-y;
            lines_to_draw.push_back(hold);

        }
    

        
	}
    if(bin == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

        lines_to_draw.push_back(hold);

        clicked=false;
        xi=0;
        yi=0;

    }

}
 
void control(unsigned char key,int x, int y){
    if(key=='t'){
        b_line=!b_line;
        mid_line=!mid_line;
    }
    if(key=='s'){
        if(style!=2){
            style++;
        }
        else{
            style=0;
        }

    }


 }
 
void display(){
    glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    glBegin(GL_POINTS);

        if(b_line){
            wake_bren();
            coord_bren();
                for (auto i:lines_to_draw){
                    brensenham_line(i.str_x,i.str_y,i.end_x,i.end_y);
                }
                brensenham_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
        }
        else if(mid_line){
                wake_mid();
                coord_mid();
                for (auto i:lines_to_draw){
                    midpoint_line(i.str_x,i.str_y,i.end_x,i.end_y);
                }
                midpoint_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
 
        }
    glEnd();
    glFlush();
    if(style==1){
        glLineStipple(dash, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }
    else if(style==2){
        glLineStipple(dot, 0xAAAA);
        glEnable(GL_LINE_STIPPLE);
    }
    else if(style == 0){
        glDisable(GL_LINE_STIPPLE);
    }
    glBegin(GL_LINES);
        wake_gl();
        coord_gl();
        for (auto i:lines_to_draw){
            gl_line(i.str_x+700,i.str_y,i.end_x+700,i.end_y);
        }
        gl_line(hold.str_x+700,hold.str_y,hold.end_x+700,hold.end_y);

    glEnd();
    glFlush();

}


int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

 glLoadIdentity();
 glMatrixMode( GL_PROJECTION );
 glutInitWindowSize(1500,1000);
 glutInitWindowPosition(100,100);
 glutCreateWindow("My Window");
 glutPassiveMotionFunc(movement);
 glutMouseFunc(mouse);
 glutKeyboardFunc(control);
 gluOrtho2D(0,1500,0,1000);
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutDisplayFunc(display);
 glutMainLoop();

 return 0;
 
}
