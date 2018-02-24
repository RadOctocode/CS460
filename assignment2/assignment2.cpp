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
bool clip=false;
int style=0;
int dash=10;
int dot=5;
int xi, yi;
int temp_x2,temp_y2;
point hold;



std::vector<point> lines_to_draw;

void gl_line(int start_x,int start_y,int end_x,int end_y){

    glVertex2i(start_x,start_y);//draw the first pixel
    glVertex2i(end_x,end_y);
}

void movement(int x, int y){
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
    

        //printf("x:%d y:%d\n",xi, yi );
	}
    if(bin == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

        lines_to_draw.push_back(hold);

        clicked=false;
        xi=0;
        yi=0;

    }
}
 
void control(unsigned char key,int x, int y){
   
    if(key=='s'){
        if(style!=2){
            style++;
        }
        else{
            style=0;
        }

    }
    if(key=='c'){
        clip=!clip;
    }
}

void clipping_window(){
    if(clip){
        glColor3f(1,1,1);

        gl_line(441,glutGet(GLUT_WINDOW_HEIGHT)-593,1193,glutGet(GLUT_WINDOW_HEIGHT)-593);
        gl_line(1193,glutGet(GLUT_WINDOW_HEIGHT)-593,1193,glutGet(GLUT_WINDOW_HEIGHT)-233);
        gl_line(1193,glutGet(GLUT_WINDOW_HEIGHT)-233,441,glutGet(GLUT_WINDOW_HEIGHT)-233);
        gl_line(441,glutGet(GLUT_WINDOW_HEIGHT)-233,441,glutGet(GLUT_WINDOW_HEIGHT)-593);  
    }
}
 
void display(){
    glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_LINES);
    clipping_window();
    glColor3f(1,0,0);

    for (auto i:lines_to_draw){
        gl_line(i.str_x,i.str_y,i.end_x,i.end_y);
    }
    gl_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
        
        
    glEnd();
    glutSwapBuffers();

}


int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

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
 glutSwapBuffers();
 glutDisplayFunc(display);
 glutMainLoop();

 return 0;
 
}
