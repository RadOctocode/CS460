#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <math.h>
#include <vector>
#include <stack>          // std::stack

//g++ -o foo assignment3.cpp -lglut -lGLU -lGL -lm
bool scene=true;
bool init=false;

int width=1000;
int height=1000;
int HEADER_SIZE = 54;
int WIDTH_OFFSET = 18;
int HEIGHT_OFFSET = 22;
int normal_x=192;
int normal_y=128;

int current_middle_x=;
int current_middle_y=;



double zoom=0;

float object_degree=0;
float rotation=0;

struct int_point{
  int x,y;
  int z=0;
  int_point(int my_x,int my_y,int my_z){
    x=my_x;
    y=my_y;
    z=my_z;
  }
  int_point(int my_x,int my_y){
    x=my_x;
    y=my_y;
  }
  int_point(){}
};

struct double_point{
  double x,y,z;
};

struct pixel{
  int x,y;
  unsigned char r,g,b;
  pixel(){}
  pixel(int x_,int y_, unsigned char r_, unsigned char g_, unsigned char b_){
    x=x_;
    y=y_;
    r=r_;
    b=b_;
    g=g_;

  }
};

struct obj_file{
  std::vector<double_point> vertices;
  std::vector<int_point> faces;
};

struct bmp_file{
  unsigned char* pixel_colors;
  int height;
  int width;
};

struct rectangle{
  int_point ll,lr,tr,tl;
  std::vector<pixel> pixels;
  rectangle(){}
  rectangle(int_point bl,int_point br,int_point ur,int_point ul){
    ll=bl;
    lr=br;
    tr=ur;
    tl=ul;
  }
};

rectangle texture_panels[4];


obj_file load_obj(){
  obj_file retval;
  std::ifstream file("teapot.obj");
  std::string header;
  
  while(!file.eof()){
    file>>header;
    if(header=="v"){
      double a,b,c;
      double_point temp_dou;

      file>>a>>b>>c;
      temp_dou.x=a;
      temp_dou.y=b;
      temp_dou.z=c;

      retval.vertices.push_back(temp_dou);

    }//make a double_point
    else if(header=="f"){
      int a,b,c;
      int_point temp_int;

      file>>a>>b>>c;
      temp_int.x=a;
      temp_int.y=b;
      temp_int.z=c;

      retval.faces.push_back(temp_int);

    }//make a face
  }

  return retval;
}

void create_obj(const obj_file &object){
  for (auto face: object.faces){

    glBegin(GL_LINE_LOOP);
    glVertex3d(object.vertices[face.x - 1].x, object.vertices[face.x - 1].y, object.vertices[face.x - 1].z);
    glVertex3d(object.vertices[face.y - 1].x, object.vertices[face.y - 1].y, object.vertices[face.y - 1].z);
    glVertex3d(object.vertices[face.z - 1].x, object.vertices[face.z - 1].y, object.vertices[face.z - 1].z);    
    glEnd();
  }//for every face draw a triangle
}

bmp_file load_bmp(){
    bmp_file retval;

    FILE* fd=fopen("flower.bmp","rb");
    unsigned char info[HEADER_SIZE];
    fread(info,sizeof(unsigned char), HEADER_SIZE, fd); //read the header

    int my_width  = *(int *)&info[WIDTH_OFFSET];
    int my_height = *(int *)&info[HEIGHT_OFFSET];


    int size=3 * width * height;  // three channels (RGB)

    unsigned char* pixels = (unsigned char *)malloc(sizeof(unsigned char)*size);
    fread(pixels, sizeof(unsigned char), size, fd);// read the pixels
    fclose(fd);

    
    for (int i =0; i <size; i=i+3){
        unsigned char temp_byte;
        temp_byte=pixels[i];
        pixels[i] = pixels[i+2];
        pixels[i+2] = temp_byte;
    }//swap B and R

    retval.pixel_colors=pixels;
    retval.width=my_width;
    retval.height=my_height;
  
   return retval;
}

void seperate_bmp(const bmp_file &texture){
  int_point bottom_left=int_point(0,0);
  int_point bottom_middle=int_point(texture.width/2,0);
  int_point bottom_right=int_point(texture.width,0);

  int_point top_left=int_point(0,texture.height);
  int_point top_middle=int_point(texture.width/2,texture.height);
  int_point top_right=int_point(texture.width,texture.height);

  int_point middle_left=int_point(0,texture.height/2);
  int_point middle_right=int_point(texture.width,texture.height/2);  
  int_point middle=int_point(texture.width/2,texture.height/2);

  texture_panels[0]=rectangle(bottom_left,bottom_middle,middle,middle_left);
  texture_panels[1]=rectangle(bottom_middle,bottom_right,middle_right,middle);
  texture_panels[2]=rectangle(middle,middle_right,top_right,top_middle);
  texture_panels[3]=rectangle(middle_left,middle,top_middle,top_left);

  
  for(int i=0;i<256;i++){
      for(int j=0;j<384;j++){

          int offset = j+384*i;
          pixel current_pixel;

          unsigned char r=texture.pixel_colors[3*offset + 0];
          unsigned char g=texture.pixel_colors[3*offset + 1];
          unsigned char b=texture.pixel_colors[3*offset + 2];
              
              

          if(i>=0&&i<=128&&j>=0&&j<=192){
            texture_panels[0].pixels.push_back(pixel(j,i,r,g,b));
          }
          if(i>=0&&i<=128&&j>=192&&j<=384){
            texture_panels[1].pixels.push_back(pixel(j,i,r,g,b));
          }
          else if(i>=128&&i<=256&&j>=192&&j<=384){
            texture_panels[2].pixels.push_back(pixel(j,i,r,g,b));
          }
          else if(i>=128&&i<=256&&j>=0&&j<=192){
            texture_panels[3].pixels.push_back(pixel(j,i,r,g,b));
          }
             




      }

    }

}

void draw_panel(const rectangle &panel){
  printf("%u\n",panel.pixels.size());
  for(auto pixel_:panel.pixels){
          glColor3ub(pixel_.r,pixel_.g,pixel_.b);
          glVertex2i(pixel_.x,pixel_.y);
  }

}

void display(){
  glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
  obj_file teapot=load_obj();
  bmp_file flower=load_bmp();
  if(!init){
    seperate_bmp(flower);
    init=!init;
  } 
  glColor3f(0,1,0);
  glLineWidth(1);

  //HOW DOES THIS WORK
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60, 1, 1, 500010);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);

  // Camera adjustment
  if(scene){

    //glDrawPixels(flower.width,flower.height, GL_RGB, GL_UNSIGNED_BYTE,flower.pixel_colors);
    //draw_texture(flower_panels);

    /*for(int i=0;i<flower.height;i++){
      for(int j=0;j<flower.width;j++){
          glBegin(GL_POINTS);
          int offset = j+flower.width*i;
          unsigned char r=flower.pixel_colors[3*offset + 0];
          unsigned char g=flower.pixel_colors[3*offset + 1];
          unsigned char b=flower.pixel_colors[3*offset + 2];
          glColor3ub(r,g,b);
          glVertex2i(j,i);
          glEnd();

      }

    }
*/
    glPushMatrix();
    glRotated(rotation, 0.0, 0.0, 1.0);
    glScalef(0.018, 0.02, 0.02);//scale down
    glTranslatef(-flower.width/2.0, -flower.height/2.0, 0.0);//center


    glBegin(GL_POINTS);
      draw_panel(texture_panels[0]);
      draw_panel(texture_panels[1]);
      draw_panel(texture_panels[2]);
      draw_panel(texture_panels[3]);

    glEnd();
  glPopMatrix();

  }

  if(!scene){

    glPushMatrix();

      glTranslatef(0.0, -5.6, 0.0);
      glScalef(3.0+zoom, 3.0+zoom, 3.0+zoom);    
      glRotatef(object_degree, 0, 1, 0);

      create_obj(teapot);
    glPopMatrix();
  }
  
  glutSwapBuffers();
}

void control(unsigned char key, int x, int y){
  if(key=='t'){
    scene=!scene;
  }

  if(key=='z'){
    if(zoom!=80){
      zoom=zoom+1;
    }
  }

  if(key=='x'){
    if(zoom!=-2){
      zoom=zoom-1;
    }
  }

  if(key=='r'){
    object_degree=object_degree+5;
    rotation+=30;
  }

  if(key=='e'){
    object_degree=object_degree-5;
    rotation-=30;
  }


  glutPostRedisplay();
}

/*void mouse_motion_handler(int x, int y) {
  int win_w = glutGet(GLUT_WINDOW_WIDTH);
  int win_h = glutGet(GLUT_WINDOW_HEIGHT);
  if (scene == 0) {
    auto new_center = Vertex(x-win_w/2+flower_width/2, -y+win_h/2+flower_height/2);
    // rotate with flower
    rotate(new_center);
    for (int i = 0; i < 4; i++) {
      flower_texture[i].v3 = new_center;
    }
  } else if (scene == 1) {
    center = Vertex(x-win_w/2, -y+win_h/2);
  }
}//change
*/


void mouse(int button, int state, int x, int y){


}

int main (int argc,char** argv){

  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glutInitWindowSize(width/2,height/2);
  glutInitWindowPosition(100,100);
  glutCreateWindow("assignment 4");
  glutKeyboardFunc(control);
  //glutMotionFunc(mouse_motion_handler);
  glutMouseFunc(mouse);
  gluOrtho2D(0,width,height,0);
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glutSwapBuffers();
  glutDisplayFunc(display);
  glutMainLoop();


 return 0;
 
}