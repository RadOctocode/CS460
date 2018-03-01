#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <stack>          // std::stack
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
point hold;

dot temp;

bool clicked=false;
bool clip=false;
bool clipped=false;
bool added=false;
bool flood=false;
bool view=false;
bool resize_view=false;
bool resize_window=false;
bool move_window=false;

int xi, yi;
int other_poly=0;
int temp_x2,temp_y2;

int flood_x=-1;
int flood_y=-1;

int vxi=31;
int vyi=212;
int vx2=343;
int vy2=429;

int wxi=441;
int wyi=407;
int wx2=1193;
int wy2=767;


std::vector<point> lines_to_draw;
std::vector<dot> view_window;
std::vector<dot> polygon_vertex;
std::vector<dot> new_polygon;
std::vector<dot> color;

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

        }
        if(resize_view){
            vx2=x;
            vyi=y;
        }

    
        if(resize_window){
            wx2=x;
            wy2=glutGet(GLUT_WINDOW_HEIGHT)-y;
        }

        if(move_window){
            int window_length=std::abs(wxi-wx2); 
            int window_height=std::abs(wyi-wy2);
            wxi=x;
            wx2=x+window_length;
            wy2=glutGet(GLUT_WINDOW_HEIGHT)-y;
            wyi=(wy2-window_height);
        }

        glutPostRedisplay();
}

void floodfill(int x, int y, unsigned char* seed){
        unsigned char current_p[4];

        glReadPixels(x,y,1,1,GL_RGB, GL_UNSIGNED_BYTE, current_p);
            glColor3f(1,0,0);
            glBegin(GL_POINTS);
            glVertex2i(x,y);
            glEnd();
            glutSwapBuffers();
            //check correct color
        if(seed[0]==current_p[0] && seed[1]==current_p[1] && seed[2]==current_p[2] ){
            dot valid_pixel; 
                //color red
                //glBegin(GL_POINTS);
                //color
            valid_pixel.x=x;
            valid_pixel.y=y;
            //printf("x:%d y:%d\n",x,y );
            color.push_back(valid_pixel);
                //add
            
            floodfill(x+1,y,seed);
            floodfill(x-1,y,seed);
            floodfill(x,y-1,seed);
            floodfill(x,y+1,seed);
               
        }          
}//using a dfs

void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        if(!view){
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
        if(flood){
            flood_x=x;
            flood_y=glutGet(GLUT_WINDOW_HEIGHT)-y;
            unsigned char bg[4];
            glReadPixels(flood_x,flood_y,1,1,GL_RGB, GL_UNSIGNED_BYTE, bg);
            floodfill(flood_x,flood_y,bg);
            glutPostRedisplay();


        }
        }

        if(view){
            //top right to scale viewport
            if(std::abs(x-vx2)<35 && std::abs(y-vyi)<35){
                resize_view=!resize_view;
            }
            //top right to scale clip
            else if(std::abs(x-(wx2))<35 && std::abs(y-(glutGet(GLUT_WINDOW_HEIGHT)-wy2))<35){
                resize_window=!resize_window;
            }
            //top left to move window for panning
            else if(std::abs(x-(wxi))<35 && std::abs(y-(glutGet(GLUT_WINDOW_HEIGHT)-wy2))<35){
                move_window=!move_window;
            }
        }
        //printf("what i think it is x:%d y:%d\n", x, y);
               // printf("what i think it is x:%d y:%d\n", (glutGet(GLUT_WINDOW_HEIGHT)-wxi), y);

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
        printf("clipped\n");
        glutPostRedisplay();

    }
    if(key=='f'){
        
        flood=!flood;
        printf("floodfill mode: %d\n",flood);
    }
    if(key=='v'){
        view=!view;
        printf("view port mode: %d\n",view);
        glutPostRedisplay();


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

std::vector<dot> clip_left(){
    int win_str_x=wxi;
    int win_str_y=glutGet(GLUT_WINDOW_HEIGHT)-wyi;
    int win_end_x=wxi;
    int win_end_y=glutGet(GLUT_WINDOW_HEIGHT)-wy2;

    dot holding_dot;
    point left;

    left.str_x=wxi;
    left.str_y=glutGet(GLUT_WINDOW_HEIGHT)-wyi;
    left.end_x=wxi;
    left.end_y=glutGet(GLUT_WINDOW_HEIGHT)-wy2;
    std::vector<dot> new_dot;
    dot str_line;
    dot end_line;

    for(auto i:lines_to_draw){ 
        str_line.x=i.str_x;
        str_line.y=i.str_y;
        end_line.x=i.end_x;
        end_line.y=i.end_y;

        int str_d=((i.str_x-win_str_x)*(win_end_y-win_str_y))-((i.str_y-win_str_y)*(win_end_x-win_str_x));
        int end_d=((i.end_x-win_str_x)*(win_end_y-win_str_y))-((i.end_y-win_str_y)*(win_end_x-win_str_x));

        if(str_d<0 && end_d<0){
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_d>=0 && end_d<0){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_d<0&&end_d>=0){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);

        }

    }
    return new_dot;
}

std::vector<dot> clip_right(){
    int win_str_x=wx2;
    int win_str_y=glutGet(GLUT_WINDOW_HEIGHT)-wyi;//593
    int win_end_x=wx2;
    int win_end_y=glutGet(GLUT_WINDOW_HEIGHT)-wy2;
    
    dot holding_dot;

    point left;

    left.str_x=wx2;
    left.str_y=glutGet(GLUT_WINDOW_HEIGHT)-wyi;
    left.end_x=wx2;
    left.end_y=glutGet(GLUT_WINDOW_HEIGHT)-wy2;
    std::vector<dot> new_dot;
    dot str_line;
    dot end_line;

    for(auto i:lines_to_draw){ 
        str_line.x=i.str_x;
        str_line.y=i.str_y;
        end_line.x=i.end_x;
        end_line.y=i.end_y;

        int str_d=((i.str_x-win_str_x)*(win_end_y-win_str_y))-((i.str_y-win_str_y)*(win_end_x-win_str_x));
        int end_d=((i.end_x-win_str_x)*(win_end_y-win_str_y))-((i.end_y-win_str_y)*(win_end_x-win_str_x));

        if(str_d>=0 && end_d>=0){
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_d<0 && end_d>=0){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_d>=0&&end_d<0){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);

        }

    }
    return new_dot;
}

std::vector<dot> clip_up(){
    int win_str_x=wxi;
    int win_str_y=wy2;
    int win_end_x=wx2;
    int win_end_y=wy2;
    dot holding_dot;
    point left;
    left.str_x=wxi;
    left.str_y=wy2;
    left.end_x=wx2;
    left.end_y=wy2;
    std::vector<dot> new_dot;
    dot str_line;
    dot end_line;

    for(auto i:lines_to_draw){


        str_line.x=i.str_x;
        str_line.y=i.str_y;
        end_line.x=i.end_x;
        end_line.y=i.end_y;
       


        if(str_line.y<wy2 && end_line.y<wy2){
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_line.y>=wy2 && end_line.y<wy2){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_line.y<wy2 && end_line.y>=wy2){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);

        }
    }

    return new_dot;
}

std::vector<dot> clip_down(){
    /*
    int wxi=441;
    int wyi=407;
    int wx2=1193;
    int wy2=767;
    */
    int win_str_x=wxi;
    int win_str_y=wyi;
    int win_end_x=wx2;
    int win_end_y=wyi;
    dot holding_dot;
    point left;
    left.str_x=wxi;
    left.str_y=wyi;
    left.end_x=wx2;
    left.end_y=wyi;
    std::vector<dot> new_dot;
    dot str_line;
    dot end_line;

    for(auto i:lines_to_draw){ 

        str_line.x=i.str_x;
        str_line.y=i.str_y;
        end_line.x=i.end_x;
        end_line.y=i.end_y;

       
        

        if(str_line.y>wyi && end_line.y>wyi){
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_line.y<=wyi && end_line.y>wyi){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);
            holding_dot.x=i.end_x;
            holding_dot.y=i.end_y;
            new_dot.push_back(holding_dot);

        }

        else if(str_line.y>wyi && end_line.y<=wyi){
            holding_dot.x=x_intersect(i,left);
            holding_dot.y=y_intersect(i,left);            
            new_dot.push_back(holding_dot);

        }

    }

    return new_dot;
}

void sutherland_hodgeman(){
    dot next_dot;
    std::vector<dot> new_polygon_vertex;
    new_polygon_vertex=clip_left();
    std::vector<point> v;
    for(int i=0;i<new_polygon_vertex.size();i++){
        point holding;
        if(i==new_polygon_vertex.size()-1){
            next_dot=new_polygon_vertex[0];
        }
        else{
            next_dot=new_polygon_vertex[i+1];
        }
        holding.str_x=new_polygon_vertex[i].x;
        holding.str_y=new_polygon_vertex[i].y;
        holding.end_x=next_dot.x;
        holding.end_y=next_dot.y;
        v.push_back(holding);
    } 
    polygon_vertex=new_polygon_vertex; 
    lines_to_draw.clear(); 
    lines_to_draw=v;
    v.clear();
    new_polygon_vertex.clear();  


    new_polygon_vertex=clip_right();
    for(int i=0;i<new_polygon_vertex.size();i++){

        point holding;

        if(i==new_polygon_vertex.size()-1){
            next_dot=new_polygon_vertex[0];
        }
        else{
            next_dot=new_polygon_vertex[i+1];
        }
        holding.str_x=new_polygon_vertex[i].x;
        holding.str_y=new_polygon_vertex[i].y;
        holding.end_x=next_dot.x;
        holding.end_y=next_dot.y;
        v.push_back(holding);
    } 

    polygon_vertex=new_polygon_vertex; 
    lines_to_draw.clear(); 
    lines_to_draw=v;  
    v.clear();
    new_polygon_vertex.clear();  


    new_polygon_vertex=clip_up();
    for(int i=0;i<new_polygon_vertex.size();i++){
        point holding;
        if(i==new_polygon_vertex.size()-1){
            next_dot=new_polygon_vertex[0];
        }
        else{
            next_dot=new_polygon_vertex[i+1];
        }
        holding.str_x=new_polygon_vertex[i].x;
        holding.str_y=new_polygon_vertex[i].y;
        holding.end_x=next_dot.x;
        holding.end_y=next_dot.y;
        v.push_back(holding);
    } 
        printf("\n");

    polygon_vertex=new_polygon_vertex;  
    lines_to_draw = v;
    v.clear();
    new_polygon_vertex.clear();  

    new_polygon_vertex=clip_down();
    for(int i=0;i<new_polygon_vertex.size();i++){
        point holding;
        if(i==new_polygon_vertex.size()-1){
            next_dot=new_polygon_vertex[0];
        }
        else{
            next_dot=new_polygon_vertex[i+1];
        }
        holding.str_x=new_polygon_vertex[i].x;
        holding.str_y=new_polygon_vertex[i].y;
        holding.end_x=next_dot.x;
        holding.end_y=next_dot.y;
        v.push_back(holding);
    } 
    polygon_vertex=new_polygon_vertex;  
    lines_to_draw = v;


   clipped=true;
}

void viewport_window(int x, int y, int x2, int y2){
        glColor3f(1,1,1);
        gl_line(x,glutGet(GLUT_WINDOW_HEIGHT)-y,x2,glutGet(GLUT_WINDOW_HEIGHT)-y);//down
        gl_line(x2,glutGet(GLUT_WINDOW_HEIGHT)-y,x2,glutGet(GLUT_WINDOW_HEIGHT)-y2);//right
        gl_line(x2,glutGet(GLUT_WINDOW_HEIGHT)-y2,x,glutGet(GLUT_WINDOW_HEIGHT)-y2);//up
        gl_line(x,glutGet(GLUT_WINDOW_HEIGHT)-y2,x,glutGet(GLUT_WINDOW_HEIGHT)-y);//left
        //printf("y2: %d\n",glutGet(GLUT_WINDOW_HEIGHT)-y2);
        //printf("y: %d\n",glutGet(GLUT_WINDOW_HEIGHT)-y);
}


void clipping_window(int x, int y, int x2, int y2){
        glColor3f(1,1,1);
        gl_line(x,y,x2,y);//down
        gl_line(x2,y,x2,y2);//right
        gl_line(x2,y2,x,y2);//up
        gl_line(x,y2,x,y);//left 
       
}

 
void display(){
    glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //viewport
    glLoadIdentity();
    gluOrtho2D( 0.0, 1500, 0,1000 );
    //viewport

    glLineStipple(10, 0xAAAA);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    clipping_window(wxi,wyi,wx2,wy2);
    viewport_window(vxi,vyi,vx2,vy2);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(1,0,0);

    for (auto i:lines_to_draw){
        gl_line(i.str_x,i.str_y,i.end_x,i.end_y);

    }
    if(!clipped&&!view){
        gl_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
    }   

    if(clip&&!clipped){
        sutherland_hodgeman();

    }
    if(clipped){
        dot next_dot;
        glColor3f(0,0,1);

        std::vector<point> v;
        for(int i=0;i<polygon_vertex.size();i++){
            point holding;
            if(i==polygon_vertex.size()-1){
                next_dot=polygon_vertex[0];
            }
            else{
                next_dot=polygon_vertex[i+1];
            }
            holding.str_x=polygon_vertex[i].x;
            holding.str_y=polygon_vertex[i].y;
            holding.end_x=next_dot.x;
            holding.end_y=next_dot.y;
            v.push_back(holding);
        } 
        lines_to_draw=v;
        for (auto i:lines_to_draw){
            gl_line(i.str_x,i.str_y,i.end_x,i.end_y);
            glutPostRedisplay();
        }
    }
    

    glEnd();
    glColor3f(1,0,0);

    glBegin(GL_POINTS);
    for(auto i:color){
            glVertex2i(i.x,i.y);
            
            
    }        
    glEnd();

    glTranslatef(vxi, glutGet(GLUT_WINDOW_HEIGHT)-vy2, 0);
    glScalef((float)(vx2-vxi)/(float)(wx2-wxi), (float)((glutGet(GLUT_WINDOW_HEIGHT)-vyi)-(glutGet(GLUT_WINDOW_HEIGHT)-vy2))/(float)(wy2-wyi), 1.0);
    glTranslatef(-1*wxi, -1*wyi, 0);

    //redraw lines
   glBegin(GL_LINES);

    for (auto i:lines_to_draw){
        gl_line(i.str_x,i.str_y,i.end_x,i.end_y);

    }
    if(!clipped){
        gl_line(hold.str_x,hold.str_y,hold.end_x,hold.end_y);
    }  
    glEnd();
    //redraw_lines
    //redraw pixels
    glBegin(GL_POINTS);
    for(auto i:color){
        glVertex2i(i.x,i.y);
    }        
    glEnd();
    //redraw pixels
    glLoadIdentity();
    gluOrtho2D( 0.0, 1500, 0,1000 );

    glutSwapBuffers();
}


int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

 glLoadIdentity();
 glMatrixMode( GL_PROJECTION );
 glutInitWindowSize(1500,1000);
 glutInitWindowPosition(100,100);
 glutCreateWindow("clipping filling and viewport");
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
