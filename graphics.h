#include<windows.h>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<GL/glut.h>
#include<algorithm>
#include<stack>
using namespace std;


class Point{
public:
    float x,y;
};
class Vertex{
public:
    int index;
    Point P;
    int type;
};
void plotPt(Point pt){
    glColor3f(1.0,0.0,0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
        glVertex2f(pt.x,pt.y);
    glEnd();
    glFlush();
}
void plotPoly(vector<Vertex> polyg){
    glColor3f(0,1,0);
    glPointSize(5);
    glBegin(GL_LINE_LOOP);
        for(int i=0;i<polyg.size();i++){
            glVertex2f(polyg[i].P.x,polyg[i].P.y);
        }
    glEnd();
    glFlush();
}

void plotLine(Point pt,Point pt1){
    glColor3f(1,0,0);
    glLineWidth(1);
    glBegin(GL_LINES);
        glVertex2f(pt.x,pt.y);
        glVertex2f(pt1.x,pt1.y);
    glEnd();
    glFlush();
}

