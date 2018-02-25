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

struct dot{
    int x;
    int y;   
};
bool clicked=false;
bool clip=false;
bool added=false;
int style=0;
int dash=10;
int xi, yi;
int temp_x2,temp_y2;
point hold;




std::vector<point> lines_to_draw;
std::vector<point> clip_window;

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
    

        printf("x:%d y:%d\n",xi, yi );
	}
    if(bin == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){

        lines_to_draw.push_back(hold);

        clicked=false;
        xi=0;
        yi=0;

    }
}
 
void control(unsigned char key, int x, int y){
   
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

int x_intersect(point poly_line, point window_line){
    int px1,py1,px2,py2,wx1,wy1,wx2,wy2;
    px1=poly_line.str_x;
    py1=poly_line.str_y;
    px2=poly_line.end_x;
    py2=poly_line.end_y;
    wx1=window_line.str_x;
    wy1=window_line.str_y;
    wx2=window_line.end_x;
    wy2=window_line.end_y;

    int top=((((px1*py2)-(py1*px2))*(wx1-wx2))-((px1-px2)*((wx1*wy2)-(wy1*wx2))));
    int den=(((px1-px2)*(wy1-wy2))-((py1-py2)*(wx1-wx2)));

    return top/den;
}
int y_intersect(point poly_line, point window_line){
    int px1,py1,px2,py2,wx1,wy1,wx2,wy2;
    px1=poly_line.str_x;
    py1=poly_line.str_y;
    px2=poly_line.end_x;
    py2=poly_line.end_y;
    wx1=window_line.str_x;
    wy1=window_line.str_y;
    wx2=window_line.end_x;
    wy2=window_line.end_y;

    int top=((((px1*py2)-(py1*px2))*(wy1-wy2))-((py1-py2)*((wx1*wy2)-(wy1*wx2))));
    int den=(((px1-px2)*(wy1-wy2))-((py1-py2)*(wx1-wx2)));

    return top/den;
}

void sutherland_hodgeman(){
    std::vector<dot> new_dots;
    std::vector<point>new_lines;
    int px1,py1,px2,py2,wx1,wy1,wx2,wy2;
    int point_1,point_2;
    dot added_dot;

        for(auto window_line:clip_window){
            wx1=window_line.str_x;
            wy1=window_line.str_y;
            wx2=window_line.end_x;
            wy2=window_line.end_y;
            for(auto poly_line:lines_to_draw){
                px1=poly_line.str_x;
                py1=poly_line.str_y;
                px2=poly_line.end_x;
                py2=poly_line.end_y;

                point_1=((wx2-wx1)*(py1-wy1))-((wy2-wy1)*(px1-wx1));
                point_2=((wx2-wx1)*(py2-wy1))-((wy2-wy1)*(px2-wx1));
                if(point_1<0 && point_2<0){
                    added_dot.x=px1;
                    added_dot.y=py1; 
                    new_dots.push_back(added_dot);
                    printf("both inside\n");
                }//both inside second point added
                else if(point_1>=0 && point_2<0){
                    added_dot.x=px1;
                    added_dot.y=py1;
                    new_dots.push_back(added_dot);
                    added_dot.x=x_intersect(poly_line,window_line);
                    added_dot.y=y_intersect(poly_line,window_line);
                    new_dots.push_back(added_dot);
                    printf("first outside\n");
                }//first outside second point and point of intersection is added
                else if(point_1<0 && point_2>=0){
                    
                    added_dot.x=x_intersect(poly_line,window_line);
                    added_dot.y=y_intersect(poly_line,window_line);
                    new_dots.push_back(added_dot);
                    printf("second outside\n");
                }//second outside point of intersection is added
            }//for all polygon lines

        }//for all window lines
        glColor3f(0,0,1);

        for(auto my_dot:new_dots){
            glVertex2i(my_dot.x,my_dot.y);

        }
    

}

void clipping_window(){
        glColor3f(1,1,1);
        gl_line(441,glutGet(GLUT_WINDOW_HEIGHT)-593,1193,glutGet(GLUT_WINDOW_HEIGHT)-593);//down
        gl_line(1193,glutGet(GLUT_WINDOW_HEIGHT)-593,1193,glutGet(GLUT_WINDOW_HEIGHT)-233);//right
        gl_line(1193,glutGet(GLUT_WINDOW_HEIGHT)-233,441,glutGet(GLUT_WINDOW_HEIGHT)-233);//up
        gl_line(441,glutGet(GLUT_WINDOW_HEIGHT)-233,441,glutGet(GLUT_WINDOW_HEIGHT)-593);//left 
       

}

 
void display(){
    glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLineStipple(dash, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    if(!added){
        point left,right,up,down;

        down.str_x=441;
        down.str_y=glutGet(GLUT_WINDOW_HEIGHT)-593;
        down.end_x=1193;
        down.end_y=glutGet(GLUT_WINDOW_HEIGHT)-593;
        right.str_x=1193;
        right.str_y=glutGet(GLUT_WINDOW_HEIGHT)-593;
        right.end_x=1193;
        right.end_y=glutGet(GLUT_WINDOW_HEIGHT)-233;
        up.str_x=1193;
        up.str_y=glutGet(GLUT_WINDOW_HEIGHT)-233;
        up.end_x=441;
        up.end_y=glutGet(GLUT_WINDOW_HEIGHT)-233;
        left.str_x=441;
        left.str_y=glutGet(GLUT_WINDOW_HEIGHT)-233;
        left.end_x=441;
        left.end_y=glutGet(GLUT_WINDOW_HEIGHT)-593;
        clip_window.push_back(left);
        clip_window.push_back(up);
        clip_window.push_back(right);
        clip_window.push_back(down);
        added=true;
    }
    clipping_window();
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(1,0,0);

    for (auto i:lines_to_draw){
        gl_line(i.str_x,i.str_y,i.end_x,i.end_y);
    }
    gl_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
    if(clip){
        sutherland_hodgeman();
    }    
        
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
