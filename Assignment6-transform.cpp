#include <math.h>
#include <GL/glut.h>
#include<iostream>
using namespace std;


#define MAX 10

	int n,op,tx,ty,angle,shx,shy,y,z,xr,yr;
	float a[MAX][3],b[3][3],c[MAX][3],sx,sy;


/* Function to plot a point */
void setPixel(GLint x, GLint y) {
  glBegin(GL_POINTS);
  glVertex2i(x,y);
  glEnd();
}

// Function of Rounding.
inline int round(const GLfloat a)
{
    return int(a+0.5);
}

//  DDA  Line  Algorithm 
void lineDDA(int x0,int y0,int xEnd,int yEnd)
{
    int dx = xEnd-x0;
    int dy = yEnd-y0;
    int steps,k;
    GLfloat xIncrement,yIncrement,x=x0,y=y0;

    if(abs(dx) > abs(dy))
        steps = abs(dx);
    else
        steps = abs(dy);

    xIncrement = GLfloat(dx) / GLfloat(steps);
    yIncrement = GLfloat(dy) / GLfloat(steps);
    setPixel(round(x),round(y));

    for(k=1;k<steps;k++)
    {
        x+= xIncrement;
        y+= yIncrement;
        setPixel(round(x),round(y));
    }
    glFlush();
}


void axis()
{
	int i,x,y;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0,0.0,0.0,0);
	glClear(GL_COLOR_BUFFER_BIT);	
	glColor3f (0.0, 1.0, 0.0);
	lineDDA(320,30,320,450); //x axis
	lineDDA(20,240,620,240); //y axis
	glColor3f (0.0, 1.0, 1.0);
	lineDDA(0,0,640,480);
	lineDDA(0,480,640,0);

	for(i=-10,x=20;i<=10;i++,x+=30) //horizontal line numbering
	{
		glColor3f (0.0, 1.0, 0.0);
		lineDDA(x,238,x,242);
	}

	for(i=7,y=30;i>=-7;i--,y+=30) //vertical line numbering
	{
		glColor3f (0.0, 1.0, 0.0);
		lineDDA(315,y,325,y);
	 }
}

void accept()
{
	int i;
	cout<<"Enter the coordinate : ";
	for(i=0;i<n;i++)
	{
		cout<<i+1<<","<<i+1<<":";
		cin>>a[i][0]>>a[i][1];
		a[i][2]=1;
	 }
	 a[n][0]=a[0][0];
	 a[n][1]=a[0][1];
	 a[n][2]=1;
}

void show()
{
	int i;
	axis();
	glColor3f (1.0, 0.0, 0.0);

        //  INPUT DATA	
	for(i=0;i<n;i++)
		    lineDDA(320+a[i][0],240+a[i][1],320+a[(i+1)%n][0],240+a[(i+1)%n][1]);//homogeneous form
	

	glColor3f (1.0, 1.0, 1.0);

	//  OUTPUT DATA	
	for(i=0;i<n;i++)
	    lineDDA(320+c[i][0],240+c[i][1],320+c[(i+1)%n][0],240+c[(i+1)%n][1]);//homogeneous form
}


void mul()
{
	int i,j,k;
	for(i=0;i<n;i++)
	{
		for(j=0;j<3;j++)
		{
			c[i][j]=0;
		}
	}

	for(i=0;i<n;i++)
	{
		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
			{
				c[i][j]=c[i][j]+a[i][k]*b[k][j];
			 }
		 }
	 }

	
	
}


void scal()
{
	b[0][0]=sx;
	b[1][1]=sy;
	b[2][2]=1;
	b[0][1]=b[1][0]=b[1][2]=b[0][2]=b[2][1]=b[2][0]=0;
}


void rot_ref()
{
	float rad;
	int sign;

	rad=(angle*3.14)/180;

	if(z==1)
	sign=1;
	else
	sign=-1;
	b[0][0]=b[1][1]=cos(rad);
	b[2][2]=1;
	b[0][1]=sign*sin(rad);
	b[1][0]=-sign*sin(rad);
	b[2][0]=-xr*cos(rad)+yr*sign*sin(rad)+xr;
	b[2][1]=-xr*sin(rad)+yr*(-sign)*cos(rad)+yr;
	b[0][2]=b[1][2]=0;
}



void reflection()
{
	int i;		
	b[2][2]=1;
	b[0][1]=b[1][0]=b[1][2]=b[0][2]=b[2][1]=b[2][0]=0;
	
	switch(z)
	{
		case 1 :  b[0][0]=1;
			  b[1][1]=-1;
			  break;

		case 2 :  b[0][0]=-1;
			  b[1][1]=1;
			  break;
		
		case 3 :  b[0][0]=-1;
			  b[1][1]=-1;
			  break;
		
		case 4 :  b[0][0]=0;
			  b[1][1]=0;
 			  b[0][1]=1;
			  b[1][0]=1;
			  break;

		case 5 :  b[0][0]=0;
			  b[1][1]=0;
 			  b[0][1]=-1;
			  b[1][0]=-1;
			  break;
		
	}	
}


void choice(void)
{
		cout<<"\n\n***********   2D  Transformations   *********";		
		cout<<"\n\nEnter the no of vertices of polygon : ";
		cin>>n;
		accept();
		cout<<"\n***********MENU**********";
		cout<<"\n \n1) Scaling \n2) Rotation of arbitary  \n3) Reflection";
		cout<<"\nEnter your choice : ";
		cin>>op;
		switch(op)
		{
			
		       case 1:  cout<<"\nThe polygon before scaling";
				cout<<"\nEnter the sx";
				cin>>sx;
				cout<<"\nEnter the sy";
				cin>>sy;
				scal();
				mul();
				show();		
				break;


		      case 2:   cout<<"\nThe polygon befor rotation";
				cout<<"\nEnter the angle : ";
				cin>>angle;
				cout<<"\nPress 1 for anticlockwise and 2 for clockwise : ";
				cin>>z;
				cout<<"Enter the x and y coordinate : ";
				cin>>xr>>yr;
				rot_ref();
				mul();
				show();
				break;



			case 3: cout<<"\nThe polygon before Reflection";
				cout<<"\n-----------------------------";
				cout<<"\n\t1. Against X-axis\n\t2. Against Y-axis\n\t3. Against Origin";
				cout<<"\n\t4. X = Y\n\t5. X = -Y\n\tEnter you Choice : ";
				cin>>z;
 				reflection();
				mul();
				show();
				break;

				
		      default: cout<<"Invalid option";

		 }
}



void init() 
{  
  /* Set the initial display mode */
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  /* Set the initial window position and size */
  glutInitWindowPosition(0,0);
  glutInitWindowSize(640,480);
  /* Create the window with title "2D Transformations" */
  glutCreateWindow("2D Transformations");
  /* Set clear color to white */
  glClearColor(1.0,1.0,1.0,1.0);
  /* Set fill color to black */
  glColor3f(0.0,0.0,0.0);
  /* glViewport(0 , 0 , 640 , 480); */
  /* glMatrixMode(GL_PROJECTION); */
  /* glLoadIdentity(); */
	glPointSize(1.0f);
  gluOrtho2D(0 , 640 , 0 , 480);

  
}

int main(int argc, char **argv) 
{
  glutInit(&argc, argv);
  choice();

  init();
  glutDisplayFunc(show);

  glutMainLoop();
  return 0;
}
