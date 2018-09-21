#include<windows.h>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<GL/glut.h>
#include<algorithm>
#include "triangulation.h"
using namespace std;
//
//class Point{
//public:
//    float x,y;
//};
//class Vertex{
//public:
//    int index;
//    Point P;
//    int type;
//};
Dcel Dc;
vector<Vertex> poly;
vector<Vertex> eventQ;
vector<pair<int,int> > Tree;
vector<int> helper;
vector<pair<int,int> > ans;

void init(){
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,640,0,480);
}
//void plotPt(Point pt){
//    glColor3f(1.0,0.0,0.0);
//    glPointSize(5);
//    glBegin(GL_POINTS);
//        glVertex2f(pt.x,pt.y);
//    glEnd();
//    glFlush();
//}
//void plotPoly(){
//    glColor3f(0,1,0);
//    glPointSize(5);
//    glBegin(GL_LINE_LOOP);
//        for(int i=0;i<poly.size();i++){
//            glVertex2f(poly[i].P.x,poly[i].P.y);
//        }
//    glEnd();
//    glFlush();
//}
//
//void plotLine(Point pt,Point pt1){
//    glColor3f(1,0,0);
//    glLineWidth(2);
//    glBegin(GL_LINES);
//        glVertex2f(pt.x,pt.y);
//        glVertex2f(pt1.x,pt1.y);
//    glEnd();
//    glFlush();
//}
/*bool comp(Vertex v1,Vertex v2){
    if(v1.P.y==v2.P.y)
        return v1.P.x<v2.P.x;
    else
        return v1.P.y>v2.P.y;
}*/
/*bool compx(pair<int,int> a,pair<int,int> b){
    return(a.second <b.second);
}*/
///*
//void constructDCEL(Vertex v1,Vertex v2){
//    if(v1.P.x > v2.P.x){Vertex temp=v1; v1=v2; v2=temp;}
//    Face fd;
//    fd.findex=Dc.f.size()+1;
//    fd.inner=make_pair(v2.index,v1.index);
//    HalfEdge *hd=new HalfEdge;
//    HalfEdge *ht=new HalfEdge;
//    hd->edge.first=v1.index;
//    hd->edge.second=v2.index;
//    hd->origin=v1.index;
//    ht->edge.first=v2.index;
//    ht->edge.second=v1.index;
//    ht->origin=v2.index;
//    ht->face=Dc.f.size()+1;
//    Dc.f.push_back(fd);
//    int p=poly[v1.index-2==-1?poly.size()-1:v1.index-2].index;
//    int pn=poly[v2.index-2==-1?poly.size()-1:v2.index-2].index;
//    HalfEdge *it=Dc.h;
//    HalfEdge *itr1,*itr;
//    HalfEdge *nxtitr,*enditr=Dc.h;
//        for(int i=0;i<Dc.f.size()-1;i++){
//            cout<<"START HALFEDGE FOR FACE #:"<<i+1;
//            do{
//                //cout<<"("<<itr->edge.first<<","<<itr->edge.second<<")    "<<itr->origin<<"  "/*("<<itr->twin->edge.first<<","<<itr->twin->edge.second<<")*/<<itr->face<<"   ("<<itr->next->edge.first<<","<<itr->next->edge.second<<")    ("<<itr->prev->edge.first<<","<<itr->prev->edge.second<<")\n";
//                if(/*((it->edge.first==v2.index) && (it->edge.second==(v2.index%poly.size()+1)))||*/
//                   ((it->edge.first==v2.index) /*&& (it->edge.second==(poly[pn-1].index))*/)){
//                    itr=it;
//                }
//                if(/*((it->edge.first==poly[p-1].index) && (it->edge.second==v1.index))||*/
//                   ((it->edge.first==poly[p-1].index) /*&& (it->edge.second==v1.index)*/)){
//                    itr1=it;
//                }
//                if(it->twin!=NULL) nxtitr=it->twin;
//                it=it->next;
//            }while(it!=enditr);
//            it=nxtitr;
//            enditr=nxtitr;
//        }
//    if((itr->edge.first==v2.index)/* && (itr->edge.second==(v2.index%poly.size()+1))*/){
//        cout<<"N;in";
//        hd->face=itr->face;
//        HalfEdge *temp=itr->prev;
//        hd->next=itr;
//        temp->next=ht;
//        itr->prev=hd;
//        ht->prev=temp;
//    }
//    if((itr1->edge.first==poly[p-1].index)/* && (itr1->edge.second==v1.index)*/){
//        cout<<"P;in";
//        HalfEdge *temp=itr1->next;
//        hd->prev=itr1;
//        itr1->next=hd;
//        ht->next=temp;
//        temp->prev=ht;
//    }
//    hd->twin=ht;
//    ht->twin=hd;
//    /*HalfEdge *temp=ht;
//    do{

//        temp->face=ht->face;
//        temp=temp->next;
//    }while(temp!=ht);*/
//}*/
void handleStartVertex(Vertex cur){
    cout<<"\nSTART:"<<cur.index<<"\n\t";
    Tree.push_back(make_pair(cur.index,cur.P.x));
    helper[cur.index-1]=cur.index;
    cout<<"Edge"<<cur.index<<" is inserted and helper("<<cur.index<<")="<<cur.index<<"\n";
}
void handleEndVertex(Vertex cur){
    cout<<"\nEND:"<<cur.index<<"\n\t";
    int prev=cur.index-2;
    if(cur.index-2==-1)
        prev=poly.size()-1;
    if(poly[helper[prev]-1].type==3){
        cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n";
        plotLine(cur.P,poly[helper[prev]-1].P);
        Dc.constructDCEL(cur,poly[helper[prev]-1],poly);
        ans.push_back(make_pair(cur.index,helper[prev]));
    }
    for(int i=0;i<Tree.size();i++){
        if(Tree[i].first==helper[prev]){
            cout<<"Delete"<<Tree[i].first<<"from the tree\n\t";
            Tree[i].first=-1;
        }
        Tree.erase(Tree.begin()+i);
    }
}
void handleMergeVertex(Vertex cur){
    cout<<"\nMERGE:"<<cur.index<<"\n\t";
    int prev=cur.index-2;
    if(cur.index-2==-1)
        prev=poly.size()-1;
    cout<<"Prev node:"<<prev+1<<"\n\t";
    if(poly[helper[prev]-1].type==3){
        cout<<"is a merge vertex.\n\tInsert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n\t";
        plotLine(cur.P,poly[helper[prev]-1].P);
        Dc.constructDCEL(cur,poly[helper[prev]-1],poly);
        ans.push_back(make_pair(cur.index,helper[prev]));
    }
    for(int i=0;i<Tree.size();i++){
        if(Tree[i].first==helper[prev]){
            Tree[i].first=-1;
            Tree.erase(Tree.begin()+i);
            cout<<"Edge"<<prev+1<<"deleted from the tree"<<"\n\t";
            break;
        }
    }
    if(Tree.size()!=0){
    cout<<"Left neighbor:";
    Vertex left;
    if(Tree.size()==1){
        left=poly[Tree[0].first-1];
    }
    for(int i=0;i<Tree.size();i++){
        //cout<<"Iterating node:"<<Tree[i].second<<" "<<Tree[i].first;
        left=poly[Tree[i].first-1];
        if(Tree[i].second > cur.P.x){
            left=poly[Tree[i-1].first-1];
            //cout<<Tree[i-1].first;
            break;
        }
        left=poly[Tree[i].first-1];
        cout<<left.index<<"\n\t";
    }
    //cout<<":::";
    if(poly[helper[left.index-1]-1].type==3){
        cout<<"is a Merge vertex.\n\t Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
        plotLine(cur.P,poly[helper[left.index-1]-1].P);
        Dc.constructDCEL(cur,poly[helper[left.index-1]-1],poly);
        ans.push_back(make_pair(cur.index,helper[left.index-1]));
    }
    helper[left.index-1]=cur.index;
    cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n";
    }
}
void handleSplitVertex(Vertex cur){
    Vertex left;
    cout<<"\nSPLIT:"<<cur.index<<"\n\t";
    if(Tree.size()!=0){
    if(Tree.size()==1){
        left=poly[Tree[0].first-1];
    }
    for(int i=0;i<Tree.size();i++){
        if(Tree[i].second> cur.P.x){
            left=poly[Tree[i-1].first-1];
            break;
        }
        left=poly[Tree[i].first-1];
    }
    cout<<"Left neighbor:"<<left.index<<"\n\t";
    cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
    Dc.constructDCEL(cur,poly[helper[left.index-1]-1],poly);
    ans.push_back(make_pair(cur.index,helper[left.index-1]));
    plotLine(cur.P,poly[helper[left.index-1]-1].P);
    helper[left.index-1]=cur.index;
    cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n\t";
    }
    Tree.push_back(make_pair(cur.index,cur.P.x));
    cout<<"Insert"<<cur.index<<" into the tree ans set helper("<<cur.index<<")="<<cur.index<<"\n";
    helper[cur.index-1]=cur.index;
}
/*bool right(Vertex cur,Vertex prev){
    if(cur.P.y < prev.P.y)
        return true;
    else if(cur.P.y==prev.P.y){
        if(cur.P.x < prev.P.x)
            return false;
    }
    return false;
}*/
void handleRegularVertex(Vertex cur){
    int prev=cur.index-2;
    if(prev==-1) prev=poly.size()-1;
    cout<<"\nREGULAR:"<<cur.index<<"\n\t";
    if(right(cur,poly[prev])){
        cout<<"if(right) case:\n\t";
        if(cur.index-2==-1)
            prev=poly.size()-1;
        if(poly[helper[prev]-1].type==3){
            cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[prev]<<"\n\t";
            Dc.constructDCEL(cur,poly[helper[prev]-1],poly);
            plotLine(cur.P,poly[helper[prev]-1].P);
            ans.push_back(make_pair(cur.index,helper[prev]));
        }
        for(int i=0;i<Tree.size();i++){
            if(Tree[i].first==helper[prev]){
                cout<<"Delete"<<Tree[i].first<<"from the tree\n\t";
                Tree[i].first=-1;
                Tree.erase(Tree.begin()+i);
                break;
            }

        }
        Tree.push_back(make_pair(cur.index,cur.P.x));
        helper[cur.index-1]=cur.index;
        cout<<"Insert"<<cur.index<<" into the tree ans set helper("<<cur.index<<")="<<cur.index<<"\n";
    }
    else{
        cout<<"Else case:\n\t";
        Vertex left;
        if(Tree.size()!=0){
            if(Tree.size()==1){
                left=poly[Tree[0].first-1];
            }
            for(int i=0;i<Tree.size();i++){
                if(Tree[i].second> cur.P.x){
                    left=poly[Tree[i-1].first-1];
                    break;
                }
                left=poly[Tree[i].first-1];
            }
            cout<<"Left neighbor:"<<left.index<<"\n\t";
            if(poly[helper[left.index-1]-1].type==3){
                cout<<"Insert Diagonal between "<<cur.index<<" and "<<helper[left.index-1]<<"\n\t";
                Dc.constructDCEL(cur,poly[helper[left.index-1]-1],poly);
                plotLine(cur.P,poly[helper[left.index-1]-1].P);
                ans.push_back(make_pair(cur.index,helper[left.index-1]));
            }
            helper[left.index-1]=cur.index;
            cout<<"Set helper("<<left.index<<")="<<cur.index<<"\n\t";
        }
    }
}
bool angle(Point a, Point b,Point c){
    int area=(b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y);
    return area>=0;
}
void identifyVertexType(){
    for(int i=0;i<poly.size();i++){
        int prev=i-1,next=i+1;
        if(i-1 ==-1)
            prev=poly.size()-1;
        if(i==poly.size()-1)
            next=0;
        if((poly[prev].P.y < poly[i].P.y) && (poly[i].P.y > poly[next].P.y)){
            if(angle(poly[i].P,poly[next].P,poly[prev].P)){
                poly[i].type=1;
            }
            else{
                poly[i].type=4;
            }
        }
        else if((poly[prev].P.y > poly[i].P.y) && (poly[i].P.y < poly[next].P.y)){
            if(angle(poly[i].P,poly[next].P,poly[prev].P)){
                poly[i].type=5;
            }
            else{
                poly[i].type=3;
            }
        }
        else{
            poly[i].type=2;
        }
    }
}
void monotonePartition(){
    int n=poly.size();
    identifyVertexType();
    eventQ=poly;
    sort(eventQ.begin(),eventQ.end(),comp);
    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index type\n";
    for(int i=0;i<n;i++){
        cout<<eventQ[i].P.x<<" "<<eventQ[i].P.y<<" "<<eventQ[i].index<<"   "<<eventQ[i].type<<"\n\t";
    }
    int frontQ=0;
    while(frontQ!=n){
        Vertex current=eventQ[frontQ];
        if(current.type==1){
            //cout<<"start";
            handleStartVertex(current);
        }
        else if(current.type==2){
            //cout<<"reg";
            handleRegularVertex(current);
        }
        else if(current.type==3){
            //cout<<"merge";
            handleMergeVertex(current);
        }

        else if(current.type==4){
            //cout<<"split";
            handleSplitVertex(current);
        }
        else if(current.type==5){
            //cout<<"end";
            handleEndVertex(current);
        }
        sort(Tree.begin(),Tree.end(),compx);
        cout<<"\nTree structure at iteration "<<frontQ+1<<":\n\t";
        for(int i=0;i<Tree.size();i++){
            cout<<Tree[i].first<<" ";
        }
        frontQ++;
    }
    cout<<"\nEnd of algorithm. Partitioned into monotone pieces\n\n";
    cout<<"No of diagonals inserted:"<<ans.size();
    cout<<"\n The diagonals are inserted between:\n";
    for(int i=0;i<ans.size();i++){
        cout<<ans[i].first<<"\t"<<ans[i].second<<"\n";
    }
}
static void display(void){
    glFlush();
}
int winHeight=480;
void mousePlot(int button,int state,int x,int y){
    static int countv=0;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        y=winHeight-y;
        Point pt;
        pt.x=x;
        pt.y=y;
        plotPt(pt);
        Vertex v;
        v.P=pt;
        v.index=countv+1;
        poly.push_back(v);
        countv++;
        helper.push_back(-1);
    }
    else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
            plotPoly(poly);
            Face fd;
            fd.findex=1;
            fd.inner=make_pair(poly[0].index,poly[1].index);
            Dc.f.push_back(fd);
            for(int i=0;i<poly.size();i++){
                VertexD vd;
                vd.P=poly[i].P;
                vd.index=poly[i].index;
                vd.eindex.first=poly[i].index;
                vd.eindex.second=poly[(i+1)%poly.size()].index;
                Dc.v.push_back(vd);
                HalfEdge *hd=new HalfEdge();
                hd->edge.first=poly[i].index;
                hd->edge.second=poly[(i+1)%poly.size()].index;
                hd->origin=poly[i].index;
                hd->face=1;
                hd->twin=NULL;
                Dc.init1(hd,1);
                cout<<"#?";
            }
            Dc.displayDcel();
            monotonePartition();
            HalfEdge *it=Dc.h;
            HalfEdge *nxtitr,*enditr=Dc.h;
            for(int i=0;i<Dc.f.size();i++){
                vector<Vertex> mon;
                Vertex temp;
                cout<<"START HALFEDGE FOR FACE #:"<<i+1;
                int ct=1;
                do{
                cout<<"("<<it->edge.first<<","<<it->edge.second<<")    "<<it->origin<<"  "/*("<<itr->twin->edge.first<<","<<itr->twin->edge.second<<")*/<<it->face<<"   ("<<it->next->edge.first<<","<<it->next->edge.second<<")    ("<<it->prev->edge.first<<","<<it->prev->edge.second<<")\n";
                if(it->twin!=NULL) nxtitr=it->twin;
                int xt=it->edge.first;
                temp.index=ct;
                temp.P=poly[xt-1].P;
                mon.push_back(temp);
                it=it->next;
                ct++;
            }while(it!=enditr);
            it=nxtitr;
            enditr=nxtitr;
            Triangulate(mon);
        }
            //Triangulate(poly);
            //Dc.displayDcel();
    }
}

int main(int argc,char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);
    glutCreateWindow("Monotone Partition");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mousePlot);
    glutMainLoop();
}
