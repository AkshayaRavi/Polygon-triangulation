#include<windows.h>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<GL/glut.h>
#include<algorithm>
#include<stack>
#include "graphics.h"
using namespace std;

class Face{
public:
    int findex;
    pair<int,int> outer;
    pair<int,int> inner;
};
class VertexD{
    public:
    int index;
    Point P;
    pair<int,int> eindex;
};
class HalfEdge{
public:
    pair<int,int> edge;
    int origin;
    HalfEdge *twin;
    int face;
    HalfEdge *next;
    HalfEdge *prev;

};
class Dcel{
public:
    vector<Face> f;
    vector<VertexD> v;
    HalfEdge  *h;
    Dcel(){
        h=NULL;
    }
    void displayDcel(){
        cout<<"\nDCEL STRUCTURE:\n";
        cout<<"FACE:\nIndex Edge\n";
        for(int i=0;i<f.size();i++){
            cout<<f[i].findex<<"   ("<<f[i].inner.first<<","<<f[i].inner.second<<")\n";
        }
        cout<<"VERTEX:\n";
        cout<<"Vertex Xcor Ycor IncidentEdge \n";
        for(int i=0;i<v.size();i++){
            cout<<v[i].index<<"    ("<<v[i].P.x<<","<<v[i].P.y<<")     e("<<v[i].eindex.first<<","<<v[i].eindex.second<<")\n";
        }
        cout<<"HALFEDGE:\n";
        cout<<"Edge Origin Twin Face next   prev\n";
       /* for(int i=0;i<h.size();i++){
            cout<<"("<<h[i].edge.first<<","<<h[i].edge.second<<")    "<<h[i].origin<<"  ("<<h[i].twin.first<<","<<h[i].twin.second<<")    "<<h[i].face<<"   ("<<h[i].next.first<<","<<h[i].next.second<<")    ("<<h[i].prev.first<<","<<h[i].prev.second<<")\n";
         }*/
        HalfEdge *itr=h;
        /*do{
            cout<<"("<<itr->edge.first<<","<<itr->edge.second<<")    "<<itr->origin<<"  "<<itr->face<<"   ("<<itr->next->edge.first<<","<<itr->next->edge.second<<")    ("<<itr->prev->edge.first<<","<<itr->prev->edge.second<<")\n";
            itr=itr->next;
        }while(itr!=h);*/

        itr=h;
        HalfEdge *nxtitr,*enditr=h;
        for(int i=0;i<f.size();i++){
            cout<<"START HALFEDGE FOR FACE #:"<<i+1;
            do{
                cout<<"("<<itr->edge.first<<","<<itr->edge.second<<")    "<<itr->origin<<"  "/*("<<itr->twin->edge.first<<","<<itr->twin->edge.second<<")*/<<itr->face<<"   ("<<itr->next->edge.first<<","<<itr->next->edge.second<<")    ("<<itr->prev->edge.first<<","<<itr->prev->edge.second<<")\n";
                if(itr->twin!=NULL) nxtitr=itr->twin;
                itr=itr->next;
            }while(itr!=enditr);
            itr=nxtitr;
            enditr=nxtitr;
        }
        cout<<"End triangle Dcel";
    }
    void init1(HalfEdge *t,int flag){
        if(flag==1){
        if(h){
            t->next=h;
            t->prev=h->prev;
            h->prev=t;
            t->prev->next=t;
        }
        else{
            h=t;
            h->next=h->prev=t;
        }
        }
        /*else{
            t->next=h;
            t->prev=h->prev;
            h->prev=t;
            t->prev->next=t;
        }*/
    }
    void constructDCEL(Vertex v1,Vertex v2,vector<Vertex> poly){
    if(v1.P.x > v2.P.x){Vertex temp=v1; v1=v2; v2=temp;}
    Face fd;
    fd.findex=f.size()+1;
    fd.inner=make_pair(v2.index,v1.index);
    HalfEdge *hd=new HalfEdge;
    HalfEdge *ht=new HalfEdge;
    hd->edge.first=v1.index;
    hd->edge.second=v2.index;
    hd->origin=v1.index;
    ht->edge.first=v2.index;
    ht->edge.second=v1.index;
    ht->origin=v2.index;
    ht->face=f.size()+1;
    f.push_back(fd);
    int p=poly[v1.index-2==-1?poly.size()-1:v1.index-2].index;
    int pn=poly[v2.index-2==-1?poly.size()-1:v2.index-2].index;
    HalfEdge *it=h;
    HalfEdge *itr1,*itr;
    HalfEdge *nxtitr,*enditr=h;
        for(int i=0;i<f.size()-1;i++){
            cout<<"START HALFEDGE FOR FACE #:"<<i+1;
            do{
                //cout<<"("<<itr->edge.first<<","<<itr->edge.second<<")    "<<itr->origin<<"  "/*("<<itr->twin->edge.first<<","<<itr->twin->edge.second<<")*/<<itr->face<<"   ("<<itr->next->edge.first<<","<<itr->next->edge.second<<")    ("<<itr->prev->edge.first<<","<<itr->prev->edge.second<<")\n";
                if(/*((it->edge.first==v2.index) && (it->edge.second==(v2.index%poly.size()+1)))||*/
                   ((it->edge.first==v2.index) /*&& (it->edge.second==(poly[pn-1].index))*/)){
                    itr=it;
                }
                if(/*((it->edge.first==poly[p-1].index) && (it->edge.second==v1.index))||*/
                   ((it->edge.first==poly[p-1].index) /*&& (it->edge.second==v1.index)*/)){
                    itr1=it;
                }
                if(it->twin!=NULL) nxtitr=it->twin;
                it=it->next;
            }while(it!=enditr);
            it=nxtitr;
            enditr=nxtitr;
        }
    if((itr->edge.first==v2.index)/* && (itr->edge.second==(v2.index%poly.size()+1))*/){
        cout<<"N;in";
        hd->face=itr->face;
        HalfEdge *temp=itr->prev;
        hd->next=itr;
        temp->next=ht;
        itr->prev=hd;
        ht->prev=temp;
    }
    if((itr1->edge.first==poly[p-1].index)/* && (itr1->edge.second==v1.index)*/){
        cout<<"P;in";
        HalfEdge *temp=itr1->next;
        hd->prev=itr1;
        itr1->next=hd;
        ht->next=temp;
        temp->prev=ht;
    }
    hd->twin=ht;
    ht->twin=hd;
    /*HalfEdge *temp=ht;
    do{
        temp->face=ht->face;
        temp=temp->next;
    }while(temp!=ht);*/
}

};

