#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>


//g++ -o foo assignment5.cpp -lglut -lGLU -lGL -lm
struct point{
	double x,y,z;
	point(){
	}
	point(double x_, double y_, double z_){
		x=x_;
		y=y_;
		z=z_;
	}
	point operator-(point right){
		point ret_val;

		ret_val.x=x-right.x;
		ret_val.y=y-right.y;
		ret_val.z=z-right.z;

		return ret_val;
	}
};

struct triangle{
	point a,b,c;
	triangle(){}
	triangle(point a_, point b_, point c_){
		a=a_;
		b=b_;
		c=c_;
	}

};

point display_points[16][16];
point control_points[4][4];
std::vector<triangle> surface_triangles;
int selected_point=0;
bool toggle_normal;


void draw_axis(){
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex3d(100,0,0);
	glVertex3d(-100,0,0);
	glVertex3d(0,100,0);
	glVertex3d(0,-100,0);
	glVertex3d(0,0,100);
	glVertex3d(0,0,-100);

	glEnd();
}

point calc_norm(triangle tri){
	point ret_val;

	point u=tri.b-tri.a;
	point v=tri.c-tri.a;

	ret_val.x=(u.y*v.z)-(u.z*v.y);
	ret_val.y=(u.z*v.x)-(u.x*v.z);
	ret_val.z=(u.x*v.y)-(u.y*v.x);

	return ret_val;
}//normal dotted v 

double dot_pro(point a,point b){
	return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);

}
void draw_points(){
	point cam=point(20,10,20);
	glColor3f(0.0, 1.0, 0.0);
	glPointSize(3);
	glBegin(GL_LINES);
	for (auto tri:surface_triangles){
		if (dot_pro(cam,calc_norm(tri))<=0){
			glVertex3d(tri.a.x,tri.a.z,tri.a.y);
			glVertex3d(tri.b.x,tri.b.z,tri.b.y);

			glVertex3d(tri.b.x,tri.b.z,tri.b.y);
			glVertex3d(tri.c.x,tri.c.z,tri.c.y);

			glVertex3d(tri.c.x,tri.c.z,tri.c.y);
			glVertex3d(tri.a.x,tri.a.z,tri.a.y);
		}

	}
	glEnd();
	//display points triangle mesh


	glColor3f(1.0, 0.0, 0.0);
	glPointSize(6);
	glBegin(GL_POINTS);
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			glVertex3d(control_points[i][j].x,control_points[i][j].z,control_points[i][j].y);
		}
	}
	glEnd();
	//control points
	if(selected_point==0){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][1].x,control_points[1][1].z,control_points[1][1].y);
		glEnd();
	} 
	else if(selected_point==1){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][2].x,control_points[1][2].z,control_points[1][2].y);
		glEnd();
	}
	else if(selected_point==2){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][1].x,control_points[2][1].z,control_points[2][1].y);
		glEnd();
	}
	else if(selected_point==3){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][2].x,control_points[2][2].z,control_points[2][2].y);
		glEnd();
	}
}

void init(){
	for(int y=0;y<16;y++){
		for(int x=0;x<16;x++){
			display_points[y][x]=point(x,y,0);
		}

	}//initialize the display points
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			control_points[y][x]=point((x*4),(y*4),0);

		}

	}//initialize the display points  

}

void reset_triangles(){
	surface_triangles.clear();

	for(int y=0; y<15; y++){//i
        for(int x=0; x<15; x++){//j
            surface_triangles.push_back(triangle(display_points[y+1][x],display_points[y][x+1], display_points[y+1][x+1]));
        }
	}//initialize triangles


}
//11, 13, 31, 33
point evalCurve(const point* my_points, const float &t){
	point ret_val;
	float b0 = (1 - t) * (1 - t) * (1 - t);
	float b1 = 3 * t * (1 - t) * (1 - t);
	float b2 = 3 * t * t * (1 - t);
	float b3 = t * t * t;

	double x_=b0*my_points[0].x + b1*my_points[1].x + b2*my_points[2].x + b3*my_points[3].x;
	double y_=b0*my_points[0].y + b1*my_points[1].y + b2*my_points[2].y + b3*my_points[3].y;
	double z_=b0*my_points[0].z + b1*my_points[1].z + b2*my_points[2].z + b3*my_points[3].z;

	ret_val=point(x_,y_,z_);

	return ret_val; 
}

point evalPatch( const float &u , const float &v){
	point uCurve[4];
	for (int i = 0; i < 4; ++i){
		uCurve[i] = evalCurve(control_points[i], u);
	}

	return evalCurve(uCurve, v);
}

void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  // Set projection mode
	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();
  	gluPerspective(60, 1, 1, 300000);

  	gluLookAt(20, 10, 20, 0, 0, 0, 0, 1, 0);
  	// Set model mode
  	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	for (int i=0; i<16; i++) {
        for (int j=0; j< 16; j++) {
            display_points[i][j] = evalPatch((float)i/15.0, (float)j/15.0);
        }
	}

	reset_triangles();
	draw_axis();
	draw_points();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
	if(key==27){
		exit(0);
	}
	if(key=='t'){
		if(selected_point==3){
			selected_point=0;
		}
		else{
			selected_point++;
		}
	}
	if(key=='w'){
		//control_peoints[1][1].z+=3;
		switch(selected_point){
			case 0:
				control_points[1][1].z+=3;
				break;
			case 1:
				control_points[1][2].z+=3;
				break;
			case 2:
				control_points[2][1].z+=3;
				break;

			case 3:
				control_points[2][2].z+=3;
				break;


		}
	}
	if(key=='a'){
		//mov_left();
		switch(selected_point){
			case 0:
				control_points[1][1].x-=3;
				break;
			case 1:
				control_points[1][2].x-=3;
				break;
			case 2:
				control_points[2][1].x-=3;
				break;
			case 3:
				control_points[2][2].x-=3;
				break;
		}
	}
	if(key=='s'){
		switch(selected_point){
			case 0:
				control_points[1][1].z-=3;
				break;
			case 1:
				control_points[1][2].z-=3;
				break;
			case 2:
				control_points[2][1].z-=3;
				break;
			case 3:
				control_points[2][2].z-=3;
				break;
		}
	}
	if(key=='d'){
		switch(selected_point){
			case 0:
				control_points[1][1].x+=3;
				break;
			case 1:
				control_points[1][2].x+=3;
				break;
			case 2:
				control_points[2][1].x+=3;
				break;
			case 3:
				control_points[2][2].x+=3;
				break;
		}
		//mov_right();
	}
	  glutPostRedisplay();

}


int main(int argc, char *argv[]){
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("CS 460");
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}