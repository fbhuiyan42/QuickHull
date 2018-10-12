#include <iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include <algorithm>
#include<GL/glut.h>
using namespace std;

#define BLACK 0, 0, 0

struct Point
{
    double x,y;
};

int N;
vector<Point> point;
vector<Point> output;

ostream& operator<<( ostream& os, const Point &p)
{
    return os<<p.x<<" "<<p.y<<endl;
}


void sortX(vector<Point>& point,int N)
{
    int flag = 1;
    Point temp;
    for(int i=0;i< N && flag;i++)
    {
         flag = 0;
         for (int j=0;j <N-1;j++)
         {
            if (point[j].x> point[j+1].x )
            {
                temp = point[j];
                point[j] = point[j+1];
                point[j+1] = temp;
                flag = 1;
            }
            else if(point[j].x == point[j+1].x)
            {
                 if (point[j].y < point[j+1].y)
                 {
                    temp = point[j];
                    point[j] = point[j+1];
                    point[j+1] = temp;
                    flag = 1;
                 }
            }
         }
     }
     return;
}

int distance(Point A, Point B, Point C)
{
    int num = (B.y - A.y) * (A.x - C.x)-(B.x - A.x) * (A.y - C.y) ;
    if (num < 0) num = -num;
    return num;
}

int turn(Point A, Point B, Point P)
{
    int num = (B.x - A.x) * (P.y - A.y) - (B.y - A.y) * (P.x - A.x);
    if (num > 0) return 1;
    else if (num == 0) return 0;
    else return -1;
}

int triangle(Point A, Point B, Point C, Point D)
{
    int num1 = turn(A,B,D);
    int num2 = turn(B,C,D);
    int num3 = turn(C,A,D);
    if (num1 > 0 && num2 > 0 && num3 > 0 ) return 1;
    else if(num1 < 0 && num2 < 0 && num3 < 0 ) return 1;
    else return 0;
}

void QuickHull(Point A, Point B, vector<Point> sets, vector<Point>& output)
{
    if (sets.size() == 0) return ;
    if (sets.size() == 1)
    {
        output.push_back(sets[0]);
        sets.erase (sets.begin());
        return;
    }
    int max_distance = -999;
    Point max_point;
    int index;
    for (int i = 0; i < sets.size(); i++)
    {
        Point point = sets[i];
        if (distance(A, B, point) > max_distance)
        {
            max_distance = distance(A, B, point);
            max_point=point;
            index=i;
        }
    }
    sets.erase (sets.begin()+index);
    /*for (int i = 0; i < sets.size(); i++)
    {
        Point p = sets[i];
        if (triangle(A, B,max_point, p) == 1) sets.erase (sets.begin()+i);
    }*/
    vector<Point> upperSetAP;
    vector<Point> upperSetPB;
    for (int i = 0; i < sets.size(); i++)
    {
        Point M = sets[i];
        if (turn(A, max_point, M) == 1)
        {
            upperSetAP.push_back(M);
        }
        else if (turn(max_point,B, M) == 1)
        {
            upperSetPB.push_back(M);
        }
    }
    QuickHull(A, max_point, upperSetAP, output);
    output.push_back(max_point);
    QuickHull(max_point, B, upperSetPB, output);
    return;
}

void HUll(vector<Point> point,int N,vector<Point>& output)
{
    vector<Point>::iterator i;
    sortX(point,N);
    Point a=point[0];
    Point b=point[point.size()-1];
	point.erase (point.begin());
	
    point.erase (point.begin()+(point.size()-1));
    vector<Point> upperSet;
    vector<Point> lowerSet;
    for (int i = 0; i < point.size(); i++)
    {
        Point p = point[i];
        if (turn(a, b, p) == -1) lowerSet.push_back(p);
        else if (turn(a, b, p) == 1) upperSet.push_back(p);
    }
    output.push_back(a);
    QuickHull(a, b, upperSet, output);
    output.push_back(b);
    QuickHull(output[1],output[0], lowerSet, output);
    //sortedOutput(output,output.size());
}



void display(){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(BLACK, 0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(-30,-30,50,	0,0,0,	0,0,1);

	

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW); 
	glBegin(GL_POINTS);
	glColor3f(1, 1, 1);
	for(int i=0;i<N;i++){
		glVertex2f(point[i].x*3,point[i].y*3);
	}
  glEnd();

	int i;
	glLineWidth(2.5); 
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);{
		for(i=0;i<output.size()-1;i++){
			glVertex3f(output[i].x*3,output[i].y*3,0);
			glVertex3f(output[i+1].x*3,output[i+1].y*3,0);
	    }
		glVertex3f(output[i].x*3,output[i].y*3,0);
		glVertex3f(output[0].x*3,output[0].y*3,0);
	}glEnd();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
}

void init(){
	//codes for initialization


	//clear the screen
	glClearColor(BLACK, 0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(70, 1, 0.1, 10000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	ifstream fin ("1005081_input.txt");
    cout<<"-------------Quick Hull-------------\n";
    fin>>N;
    for(int i=0;i<N;i++)
    {
        Point point1;
        fin>>point1.x;
        fin>>point1.y;
        point.insert(point.begin()+i, point1);
    }
    cout<<"The given points are : "<<endl;
    vector<Point>::iterator i;
    for(i=point.begin(); i!= point.end(); ++i)
    {
        cout<<*i;
    }
    HUll(point,N,output);
    cout<<"The points of Convex hull are : "<<endl;
    for(i=output.begin(); i!= output.end(); ++i)
    {
        cout<<*i;
    }
    fin.close();
	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("Center Object");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}


