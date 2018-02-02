#include <GL/glut.h>
#include <GL/gl.h>

int clicked=0;
 
void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		
	    float xfloat, yfloat;
        
        if(!clicked){
            clicked=1;
            glBegin(GL_POINTS);
		    xfloat=x;
		    yfloat=glutGet(GLUT_WINDOW_HEIGHT)-y;
		    glColor3f(1,0,0);
		    glVertex3f(xfloat,yfloat,0.0);
		    glEnd();
		    glFlush();
        }

        else{
            clicked=0;
        
        
        
        }

	}
}
 
 
int main (int argc,char** argv){
 glutInit(&argc,argv);
 glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
 glutInitWindowSize(400,400);
 glutInitWindowPosition(100,100);
 glutCreateWindow("My Window");
 glutMouseFunc(mouse);
 gluOrtho2D(0,400,0,400);
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glClear(GL_COLOR_BUFFER_BIT);
 glFlush();
 glutMainLoop();
 return 0;
 
}
