#include <GL/glut.h>
#include <GL/gl.h>

//gcc -o foo assignment1.c -lglut -lGLU -lGL -lm 
int clicked=0;
int x1, y1, x2, y2;
void mouse(int bin, int state , int x , int y) {
	if(bin == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		
        
		glColor3f(1,0,0);
        
        if(!clicked){
            clicked=1;
            x1=x;
		    y1=glutGet(GLUT_WINDOW_HEIGHT)-y;
        }

        else{
            clicked=0;
            x2=x;
            y2=y;
            for(int i=x1,j=y1;i<=x2;i++){
                glBegin(GL_POINTS);
                x1=x;
		        y1=glutGet(GLUT_WINDOW_HEIGHT)-y;
                glVertex3f(i,j,0.0);
                printf("x-%d y-%d",i,j);
		        glEnd();
	            glFlush(); 
            
            }
        
        
        
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
