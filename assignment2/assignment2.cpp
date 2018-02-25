#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <vector>
//g++ -o foo assignment2.cpp -lglut -lGLU -lGL -lm 
struct point{
    int str_x,str_y;
    int end_x,end_y;
};

struct dot{
    int x;
    int y; 
    int case_num=0;
  
};
bool clicked=false;
bool clip=false;
bool clipped=false;
bool added=false;
int dash=10;
int xi, yi;
int other_poly=0;
int temp_x2,temp_y2;
point hold;
dot temp;



std::vector<point> lines_to_draw;
std::vector<dot> clip_window;
std::vector<dot> polygon_vertex;

void gl_line(int start_x,int start_y,int end_x,int end_y){
    glVertex2i(start_x,start_y);//draw the first pixel
    glVertex2i(end_x,end_y);
}

void movement(int x, int y){
        if(clicked&&!clipped){
            int new_y=y;
            hold.str_x=xi;
            hold.str_y=yi;
            hold.end_x=x;
            hold.end_y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            temp.x=x;
            temp.y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            glutPostRedisplay();

        }
}

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(!clicked){
            clicked=!clicked;
            xi=x;
            yi=glutGet(GLUT_WINDOW_HEIGHT)-y;
            temp.x=xi;
            temp.y=yi;
            if(!clipped){
            polygon_vertex.push_back(temp);
            }


        }
        else{
            xi=x;
            yi=glutGet(GLUT_WINDOW_HEIGHT)-y;
            lines_to_draw.push_back(hold);
            temp.x=xi;
            temp.y=yi;
            if(!clipped){
            polygon_vertex.push_back(temp);
            }

        }
        printf("\tx:%d y:%d\n",x,glutGet(GLUT_WINDOW_HEIGHT)-y );

	}
    if(bin == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        
        if(!clipped){
            polygon_vertex.push_back(temp);
        }
        lines_to_draw.push_back(hold);

        clicked=false;
        xi=0;
        yi=0;

    }
}
 
void control(unsigned char key, int x, int y){
   
    if(key=='c'){
        clip=true;
        glutPostRedisplay();

    }
    if(key=='f'){

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
std::vector<dot> clipping(dot win_str, dot win_end){
    std::vector<dot> new_dot;
    dot next_p_dot;
    dot holding_dot;
    point window_edge;
    point poly_edge;

    window_edge.str_x=win_str.x;
    window_edge.str_y=win_str.y;
    window_edge.end_x=win_end.x;
    window_edge.end_y=win_end.y;

    for(int j=0;j<polygon_vertex.size();j++){
        if(j==polygon_vertex.size()-1){
            next_p_dot=polygon_vertex[0];
        }
        else{
            next_p_dot=polygon_vertex[j+1];
        }
        //printf("start x:%d y:%d\n",polygon_vertex[j].x,polygon_vertex[j].y );
        //printf("end x:%d y:%d\n", next_p_dot.x,next_p_dot.y);
        poly_edge.str_x=polygon_vertex[j].x;
        poly_edge.str_y=polygon_vertex[j].y;
        poly_edge.end_x=next_p_dot.x;
        poly_edge.end_y=next_p_dot.y;

        int str_pos=((win_end.x-win_str.x)*(polygon_vertex[j].y-win_str.y))-((win_end.y-win_str.y)*(polygon_vertex[j].x-win_str.x));
        int end_pos=((win_end.x-win_str.x)*(next_p_dot.y-win_str.y))-((win_end.y-win_str.y)*(next_p_dot.x-win_str.x));
        if(str_pos<0 && end_pos<0){
            holding_dot.x=next_p_dot.x;
            holding_dot.y=next_p_dot.y;
            holding_dot.case_num=1;            
            new_dot.push_back(holding_dot);
        }//only second added

        if(str_pos>=0 && end_pos<0){
            holding_dot.x=next_p_dot.x;
            holding_dot.y=next_p_dot.y;
            holding_dot.case_num=2;            
            new_dot.push_back(holding_dot);
            holding_dot.x=x_intersect(poly_edge,window_edge);
            holding_dot.y=y_intersect(poly_edge,window_edge);
            holding_dot.case_num=2;            
            new_dot.push_back(holding_dot);
        }//intersection and second added

        else if(str_pos<0&&end_pos>=0){
            holding_dot.x=x_intersect(poly_edge,window_edge);
            holding_dot.y=y_intersect(poly_edge,window_edge);
            holding_dot.case_num=3;            

            new_dot.push_back(holding_dot);
        }//intersection added
        else{

        }
        //printf("str:%d end:%d\n",str_pos,end_pos);
    }//end of for loop

    return new_dot;
}

void sutherland_hodgeman(){
    dot next_dot;
    for(int i=0;i<4;i++){
        if(i==3){
            next_dot=clip_window[0];
        }
        else{
            next_dot=clip_window[i+1];
        }
        //i and next dot are the line for the window
        //polygon_vertex.clear();
        std::vector<dot> new_polygon_vertex=clipping(clip_window[i],next_dot);
        polygon_vertex=new_polygon_vertex;
   }//end of for window
   clipped=true;
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
        dot left,right,up,down;

        down.x=441;
        down.y=glutGet(GLUT_WINDOW_HEIGHT)-593;

        right.x=1193;
        right.y=glutGet(GLUT_WINDOW_HEIGHT)-593;

        up.x=1193;
        up.y=glutGet(GLUT_WINDOW_HEIGHT)-233;

        left.x=441;
        left.y=glutGet(GLUT_WINDOW_HEIGHT)-233;

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
    if(!clipped){
        for (auto i:lines_to_draw){
            gl_line(i.str_x,i.str_y,i.end_x,i.end_y);
        }
    }
    gl_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);

    if(clip&&!clipped){
        sutherland_hodgeman();

    }
    if(clipped){
        dot temp_dot;
        for(auto i:polygon_vertex){
            glColor3f(0,0,1);
            glVertex2i(i.x,i.y);
            glVertex2i(i.x+10,i.y+10);
            printf("x:%d y:%d case_num:%d\n",i.x,i.y,i.case_num);
        }
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
