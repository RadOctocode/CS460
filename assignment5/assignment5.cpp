#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <stdio.h>


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
	bool operator==(point right){

		return ((x==right.x)&&(y==right.y)&&(z==right.z));
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
float LightAmbient[] = { 0.1, 0.1, 0.1, 1.0 };
float LightDiffuse[] = { 0.7, 0.7, 0.7, 1.0 };
float LightSpecular[] = { 0.4, 0.4, 0.4, 1.0 };
float LightPosition[] = { 0.75, 1.75, 0.75, 0.0 };
float shini=50.0;
float diff=0.7;

int selected_point=0;

bool toggle_render=false;
bool smooth=false;

point camera=point(20,20,20);



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
}

double dot_pro(point a,point b){
	
	return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
}
void draw_cont(){
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(6);
	glBegin(GL_POINTS);
	for(int i=0;i<4;i++){
		for(int j=0;j<4;j++){
			glVertex3d(control_points[i][j].x,control_points[i][j].y,control_points[i][j].z);
		}
	}
	glEnd();
	//control points
	if(selected_point==0){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[0][0].x,control_points[0][0].y,control_points[0][0].z);
		glEnd();
	} 
	else if(selected_point==1){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[0][1].x,control_points[0][1].y,control_points[0][1].z);
		glEnd();
	}
	else if(selected_point==2){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[0][2].x,control_points[0][2].y,control_points[0][2].z);
		glEnd();
	}
	else if(selected_point==3){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[0][3].x,control_points[0][3].y,control_points[0][3].z);
		glEnd();
	}

	else if(selected_point==4){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][0].x,control_points[1][0].y,control_points[1][0].z);
		glEnd();
	} 
	else if(selected_point==5){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][1].x,control_points[1][1].y,control_points[1][1].z);
		glEnd();
	}
	else if(selected_point==6){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][2].x,control_points[1][2].y,control_points[1][2].z);
		glEnd();
	}
	else if(selected_point==7){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[1][3].x,control_points[1][3].y,control_points[1][3].z);
		glEnd();
	}

	else if(selected_point==8){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][0].x,control_points[2][0].y,control_points[2][0].z);
		glEnd();
	} 
	else if(selected_point==9){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][1].x,control_points[2][1].y,control_points[2][1].z);
		glEnd();
	}
	else if(selected_point==10){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][2].x,control_points[2][2].y,control_points[2][2].z);
		glEnd();
	}
	else if(selected_point==11){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[2][3].x,control_points[2][3].y,control_points[2][3].z);
		glEnd();
	}

	else if(selected_point==12){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[3][0].x,control_points[3][0].y,control_points[3][0].z);
		glEnd();
	} 
	else if(selected_point==13){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[3][1].x,control_points[3][1].y,control_points[3][1].z);
		glEnd();
	}
	else if(selected_point==14){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[3][2].x,control_points[3][2].y,control_points[3][2].z);
		glEnd();
	}
	else if(selected_point==15){
		glColor3f(0.0, 0.0, 1.0);
		glPointSize(6);
		glBegin(GL_POINTS);
		glVertex3d(control_points[3][3].x,control_points[3][3].y,control_points[3][3].z);
		glEnd();
	}

}

void draw_points(point cam){
	glColor3f(0.0, 1.0, 0.0);
	glPointSize(3);
	glBegin(GL_LINES);
	for (auto tri:surface_triangles){
		if (dot_pro(cam,calc_norm(tri))>0){
			glVertex3d(tri.a.x,tri.a.y,tri.a.z);
			glVertex3d(tri.b.x,tri.b.y,tri.b.z);

			glVertex3d(tri.b.x,tri.b.y,tri.b.z);
			glVertex3d(tri.c.x,tri.c.y,tri.c.z);

			glVertex3d(tri.c.x,tri.c.y,tri.c.z);
			glVertex3d(tri.a.x,tri.a.y,tri.a.z);
		}

	}
	glEnd();
	//display points triangle mesh
	draw_cont();
}

point avg_norm(point p){
	double count=0;
	double x_=0;
	double y_=0;
	double z_=0;

	for(auto tri:surface_triangles){
		bool a_true=(p==tri.a);
		bool b_true=(p==tri.b);
		bool c_true=(p==tri.c);
		if(a_true||b_true||c_true){
			point norm_tri=calc_norm(tri);
			x_=norm_tri.x+x_;
			y_=norm_tri.y+y_;
			z_=norm_tri.z+z_;
			count++;

		}
	}
	return point(x_/count,y_/count,z_/count);
}

void render_points(point cam,double shine,float diffuse){
	//SETUP
	float a_refl[]={0.0, 1.0, 0.0, 1.0};
	float d_refl[]={0.0, diffuse, 0.0, 1.0};
	float s_refl[]={1.0,1.0,1.0,1.0};
	glMaterialfv(GL_FRONT,GL_AMBIENT,a_refl);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,d_refl);
	glMaterialfv(GL_FRONT,GL_SPECULAR,s_refl);
	glMaterialf(GL_FRONT,GL_SHININESS,shine);
	//SETUP

	for (int i =0;i<15;i++){
		for (int j = 0; j <15; j++){
			glColor3d(0.0,1.0,0.0);
			point pt1 = display_points[j][i];
    		point pt2 = display_points[j+1][i];
    		point pt3 = display_points[j][i+1];
    		point pt4 = display_points[j+1][i+1];

			glBegin(GL_TRIANGLES);
			triangle tri1=triangle(pt1,pt2,pt3);
			triangle tri2=triangle(pt3,pt2,pt4);
			point temp1=calc_norm(tri1);
			point temp2=calc_norm(tri2);
			if(dot_pro(cam,temp1)){
				point n = avg_norm(pt1);
				glNormal3f(n.x,n.y,n.z);
				glVertex3d(pt1.x,pt1.y,pt1.z);
				n = avg_norm(pt2);
				glNormal3f(n.x,n.y,n.z);
				glVertex3d(pt2.x,pt2.y,pt2.z);
				n = avg_norm(pt3);
				glNormal3f(n.x,n.y,n.z);
				glVertex3d(pt3.x,pt3.y,pt3.z);
			}

			if(dot_pro(cam,temp2)){
				point n = avg_norm(pt3);
				glNormal3f(n.x,n.y,n.z);
				 glVertex3f(pt3.x, pt3.y, pt3.z);
				n = avg_norm(pt2);
				glNormal3f(n.x,n.y,n.z);
				 glVertex3f(pt2.x, pt2.y, pt2.z);
				n = avg_norm(pt4);
				glNormal3f(n.x,n.y,n.z);
				 glVertex3f(pt4.x, pt4.y, pt4.z);

			}

			glEnd();
		}

	}

	draw_cont();

}

void init(){ 
	glClearColor(0.0, 0.0, 0.0, 1.0);
 	glEnable(GL_NORMALIZE);
	selected_point=0;
	toggle_render=false;
	diff=0.7;
	shini=50.0;
	camera=point(20,20,20);

	for(int y=0;y<16;y++){
		for(int x=0;x<16;x++){
			display_points[y][x]=point(x,0,y);
		}

	}//initialize the display points
	for(int y=0;y<4;y++){
		for(int x=0;x<4;x++){
			control_points[y][x]=point((x*4),0,(y*4));

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

  	gluLookAt(camera.x, camera.y, camera.z, 0, 0, 0, 0, 1, 0);
  	// Set model mode
  	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();


	for (int i=0; i<16; i++) {
        for (int j=0; j< 16; j++) {
            display_points[i][j] = evalPatch(i/15.0, j/15.0);
        }
	}


	reset_triangles();
	draw_axis();
	if(!toggle_render){
		draw_points(camera);
	}
	else{

		glPushMatrix();
        glTranslatef(LightPosition[0], LightPosition[1], LightPosition[2]);
        glutSolidSphere(1,16,16);
		glPopMatrix();

		render_points(camera,shini,diff);
	}

	glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
	if(key==27){
		exit(0);
	}

//control 
	if(key=='t'){
		if(selected_point==15){
			selected_point=0;
		}
		else{
			selected_point++;
		}
	}
	if(key=='r'){
		if(selected_point==0){
			selected_point=15;
		}
		else{
			selected_point--;
		}
	}
//control
	
//move selected
	if(key=='w'){
		//control_peoints[1][1].z+=3;
		switch(selected_point){
			case 0:
				control_points[0][0].y+=3;
				break;
			case 1:
				control_points[0][1].y+=3;
				break;
			case 2:
				control_points[0][2].y+=3;
				break;
			case 3:
				control_points[0][3].y+=3;
				break;

			case 4:
				control_points[1][0].y+=3;
				break;
			case 5:
				control_points[1][1].y+=3;
				break;
			case 6:
				control_points[1][2].y+=3;
				break;
			case 7:
				control_points[1][3].y+=3;
				break;

			case 8:
				control_points[2][0].y+=3;
				break;
			case 9:
				control_points[2][1].y+=3;
				break;
			case 10:
				control_points[2][2].y+=3;
				break;
			case 11:
				control_points[2][3].y+=3;
				break;

			case 12:
				control_points[3][0].y+=3;
				break;
			case 13:
				control_points[3][1].y+=3;
				break;
			case 14:
				control_points[3][2].y+=3;
				break;
			case 15:
				control_points[3][3].y+=3;
				break;

		}
	}
	if(key=='a'){
		//mov_left();
		switch(selected_point){
			case 0:
				control_points[0][0].x-=3;
				break;
			case 1:
				control_points[0][1].x-=3;
				break;
			case 2:
				control_points[0][2].x-=3;
				break;
			case 3:
				control_points[0][3].x-=3;
				break;

			case 4:
				control_points[1][0].x-=3;
				break;
			case 5:
				control_points[1][1].x-=3;
				break;
			case 6:
				control_points[1][2].x-=3;
				break;
			case 7:
				control_points[1][3].x-=3;
				break;

			case 8:
				control_points[2][0].x-=3;
				break;
			case 9:
				control_points[2][1].x-=3;
				break;
			case 10:
				control_points[2][2].x-=3;
				break;
			case 11:
				control_points[2][3].x-=3;
				break;

			case 12:
				control_points[3][0].x-=3;
				break;
			case 13:
				control_points[3][1].x-=3;
				break;
			case 14:
				control_points[3][2].x-=3;
				break;
			case 15:
				control_points[3][3].x-=3;
				break;
		}
	}
	if(key=='s'){
		switch(selected_point){
			case 0:
				control_points[0][0].y-=3;
				break;
			case 1:
				control_points[0][1].y-=3;
				break;
			case 2:
				control_points[0][2].y-=3;
				break;
			case 3:
				control_points[0][3].y-=3;
				break;

			case 4:
				control_points[1][0].y-=3;
				break;
			case 5:
				control_points[1][1].y-=3;
				break;
			case 6:
				control_points[1][2].y-=3;
				break;
			case 7:
				control_points[1][3].y-=3;
				break;

			case 8:
				control_points[2][0].y-=3;
				break;
			case 9:
				control_points[2][1].y-=3;
				break;
			case 10:
				control_points[2][2].y-=3;
				break;
			case 11:
				control_points[2][3].y-=3;
				break;

			case 12:
				control_points[3][0].y-=3;
				break;
			case 13:
				control_points[3][1].y-=3;
				break;
			case 14:
				control_points[3][2].y-=3;
				break;
			case 15:
				control_points[3][3].y-=3;
				break;
		}
	}
	if(key=='d'){
		switch(selected_point){
			case 0:
				control_points[0][0].x+=3;
				break;
			case 1:
				control_points[0][1].x+=3;
				break;
			case 2:
				control_points[0][2].x+=3;
				break;
			case 3:
				control_points[0][3].x+=3;
				break;

			case 4:
				control_points[1][0].x+=3;
				break;
			case 5:
				control_points[1][1].x+=3;
				break;
			case 6:
				control_points[1][2].x+=3;
				break;
			case 7:
				control_points[1][3].x+=3;
				break;

			case 8:
				control_points[2][0].x+=3;
				break;
			case 9:
				control_points[2][1].x+=3;
				break;
			case 10:
				control_points[2][2].x+=3;
				break;
			case 11:
				control_points[2][3].x+=3;
				break;

			case 12:
				control_points[3][0].x+=3;
				break;
			case 13:
				control_points[3][1].x+=3;
				break;
			case 14:
				control_points[3][2].x+=3;
				break;
			case 15:
				control_points[3][3].x+=3;
				break;
		}
		//mov_right();
	}
//move selected


//lighting
	if(key=='q'){
		smooth=(!smooth);
		if(!smooth){
				glShadeModel(GL_FLAT);
			}
			if(smooth){
				glShadeModel(GL_SMOOTH);
			}
	}

	if(key=='e'){

		toggle_render=!toggle_render;
		if(toggle_render){
			if(!smooth){
				glShadeModel(GL_FLAT);
			}
			if(smooth){
				glShadeModel(GL_SMOOTH);
			}
    		glEnable(GL_DEPTH_TEST);
    		glEnable(GL_LIGHTING);
    		glEnable(GL_LIGHT0);
    		glEnable(GL_COLOR_MATERIAL);

     		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
     		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
     		glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	 		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

		}
		else{
			glDisable(GL_LIGHT0);
      		glDisable(GL_DEPTH_TEST);
			glDisable(GL_LIGHTING);
		}
	}
//lighting


//move light
	if(key=='y'){
   		LightPosition[1] -= 0.5;
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	}

	if(key=='h'){
   		LightPosition[1] += 0.5;
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	}

	if(key=='g'){
   		LightPosition[0] += 0.5;
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	}

	if(key=='j'){
   		LightPosition[0] -= 0.5;
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	}
//move light
//diff
	if(key=='1'){
		diff+=0.1;
		render_points(camera,shini,diff);

	}
	if(key=='2'){
		diff-=0.1;
		render_points(camera,shini,diff);

	}
//diff
//spec
	if(key=='3'){
		shini+=1;
		render_points(camera,shini,diff);

	}
	if(key=='4'){
		if(shini>1){
			shini-=1;
		}
		render_points(camera,shini,diff);

	}
//spec
//pos
	if(key=='5'){
		camera.x=camera.x+5;
		draw_points(camera);

	}
	if(key=='6'){
		camera.x=camera.x-5;
		draw_points(camera);

	}
//pos

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