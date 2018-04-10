#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <stack>          // std::stack

//g++ -o foo assignment3.cpp -lglut -lGLU -lGL -lm
bool scene=true;

int width=1000;
int height=1000;
int HEADER_SIZE = 54;
int WIDTH_OFFSET = 18;
int HEIGHT_OFFSET = 22;

double zoom=0;

float object_degree=0;
float rotation=0;


struct int_point{
  int x,y;
  int z=0;
};

struct double_point{
  double x,y,z;
};

struct pixel{
  int x,y;
  unsigned char r,g,b;
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
};





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



void display(){
  glClearColor(00.0f, 00.0f, 00.0f, 1.0f );
  glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT );
  obj_file teapot=load_obj();
  bmp_file flower=load_bmp();
  //rectangle* flower_panels=seperate_bmp(flower);
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

    glPushMatrix();
    glRotated(rotation, 0.0, 0.0, 1.0);
    glScalef(0.036, 0.04, 0.04);
    glTranslatef(-flower.width/2.0, -flower.height/2.0, 0.0);
    for(int i=0;i<flower.height;i++){
      for(int j=0;j<flower.width;j++){
          glBegin(GL_POINTS);
          int offset = j+flower.width*i;
          unsigned char r=flower.pixel_colors[3*offset + 0];
          unsigned char g=flower.pixel_colors[3*offset + 1];
          unsigned char b=flower.pixel_colors[3*offset + 2];
          glColor3ub(r,g,b);
          //glColor3f(1,1,0);
          glVertex2i(j,i);
              glEnd();

      }

    }

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