#include<windows.h>
#include<iostream>
#include<vector>
#include<stdlib.h>
#include<algorithm>
#include<stack>
#include "Dcel.h"
//#include "graphics.h"
using namespace std;
vector<Vertex> poly2;
vector<Vertex> eventQ1;
float areaofTriangle(Point a,Point b,Point c){
    return((b.x-a.x)*(c.y-a.y)-(c.x-a.x)*(b.y-a.y));
}

bool Left(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) > 0;
}
bool LeftOn(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) >= 0;
}
bool Collinear(Point a,Point b,Point c){
    return areaofTriangle(a,b,c) == 0;
}
bool Xor(bool x,bool y){
    return !x ^ !y;
}
bool Intersection(Point a,Point b,Point c,Point d){
    if(Collinear(a,b,c)||Collinear(a,b,d)||Collinear(c,d,a)||Collinear(c,d,b))
        return false;
    return Xor(Left(a,b,c),Left(a,b,d))&& Xor(Left(c,d,a),Left(c,d,b));

}
bool Between(Point a,Point b,Point c){
    if(!Collinear(a,b,c))
        return false;
    if(a.x!=b.x){
        return(((a.x<=c.x)&&(c.x<=b.x))||((a.x>=c.x)&&(c.x>=b.x)));
    }
    else
        return(((a.y<=c.y)&&(c.y<=b.y))||((a.y>=c.y)&&(c.y>=b.y)));
}
bool Intersect(Point a,Point b,Point c,Point d){
    if(Intersection(a,b,c,d)){
        return true;
    }
    else if(Between(a,b,c)||Between(a,b,d)||Between(c,d,a)||Between(c,d,b))
        return true;
    else{
        return false;
    }
}
bool isDiagonal(Vertex a,Vertex b){
    for(int i=0;i<poly2.size()-1;i++){
        if((poly2[i].index!=a.index)&& (poly2[i+1].index!=a.index)&&(poly2[i].index!=b.index)&& (poly2[i+1].index!=b.index)&&Intersect(a.P,b.P,poly2[i].P,poly2[i+1].P)){
            return false;
        }
    }
    if((poly2[poly2.size()-1].index!=a.index)&& (poly2[0].index!=a.index)&&(poly2[poly2.size()-1].index!=b.index)&& (poly2[0].index!=b.index)&&Intersect(a.P,b.P,poly2[poly2.size()-1].P,poly2[0].P)){
            return false;
        }
    return true;
}
bool InCone(Vertex a,Vertex b){
    if(LeftOn(a.P,poly2[a.index].P,poly2[a.index-2].P)){
        return(Left(a.P,b.P,poly2[a.index-2].P) && Left(b.P,a.P,poly2[a.index].P));
    }
    return(!(Left(a.P,b.P,poly2[a.index].P) && Left(b.P,a.P,poly2[a.index-2].P)));
}
bool Diagonal(Vertex a, Vertex b){
 //   cout<<"++"<<isDiagonal(a,b);
    return(InCone(a,b) && InCone(b,a) && isDiagonal(a,b));
}
bool comp(Vertex v1,Vertex v2){
    if(v1.P.y==v2.P.y)
        return v1.P.x<v2.P.x;
    else
        return v1.P.y>v2.P.y;
}
bool compx(pair<int,int> a,pair<int,int> b){
    return(a.second <b.second);
}

bool right(Vertex cur,Vertex prev){
    if(cur.P.y < prev.P.y)
        return true;
    else if(cur.P.y==prev.P.y){
        if(cur.P.x < prev.P.x)
            return false;
    }
    return false;
}
void Triangulate(vector<Vertex> poly1){
    poly2=poly1;
    int n=poly1.size();
    eventQ1=poly1;
    sort(eventQ1.begin(),eventQ1.end(),comp);
    cout<<"Event queue:\n";
    cout<<"Xcor Ycor index\n";
    for(int i=0;i<n;i++){
        cout<<"("<<eventQ1[i].P.x<<","<<eventQ1[i].P.y<<")   "<<eventQ1[i].index<<"\n";
    }
    int frontQ=0;
    stack <int> s;
    cout<<"\nPush segment"<<eventQ1[0].index;
    cout<<"\nPush segment"<<eventQ1[1].index;
    s.push(eventQ1[0].index);
    s.push(eventQ1[1].index);
    frontQ=2;
    while(frontQ!=n){
        int check=eventQ1[frontQ].index-2;
        if(check==-1)check=poly1.size()-1;
        if(!(poly1[s.top()-1].index==(poly1[eventQ1[frontQ].index-1].index+1)||
        poly1[s.top()-1].index==(poly1[eventQ1[frontQ].index-1].index-1))){
            while(!s.empty()){
                int ind=s.top();
                int qind=eventQ1[frontQ].index,prev1,next1;
                    if(qind==1) prev1=poly1.size();
                    else prev1=qind-1;
                    if(qind==poly1.size())next1=1;
                    else next1=qind+1;
                    if(ind!=next1 && ind!=prev1){
                        cout<<"\nDifferent chain: Insert diagonal between "<<s.top()<<" and "<<eventQ1[frontQ].index;
                        plotLine(poly1[s.top()-1].P,poly1[eventQ1[frontQ].index-1].P);
                        //constructDCEL(poly1[eventQ[frontQ].index-1],poly1[s.top()-1]);
                    }
                cout<<"\nPop out segment"<<s.top();
                s.pop();
            }
            cout<<":"<<s.empty();
            cout<<"\nPush segment"<<eventQ1[frontQ-1].index;
            s.push(eventQ1[frontQ-1].index);
            cout<<"\nPush segment"<<eventQ1[frontQ].index;
            s.push(eventQ1[frontQ].index);
        }
        else{
            int ch=1;
            int pind;
            while(ch==1){
                int ind=s.top();
                int c=eventQ1[frontQ].index;
                if(Diagonal(poly1[ind-1],poly1[c-1])){
                    int qind=eventQ1[frontQ].index,prev1,next1;
                    if(qind==1) prev1=poly1.size();
                    else prev1=qind-1;
                    if(qind==poly1.size())next1=1;
                    else next1=qind+1;
                    if(ind!=next1 && ind!=prev1){
                    cout<<"\nSame chain: Insert diagonal between "<<s.top()<<" and "<<eventQ1[frontQ].index;
                    plotLine(poly1[s.top()-1].P,poly1[eventQ1[frontQ].index-1].P);
                    //constructDCEL(poly1[eventQ[frontQ].index-1],poly1[s.top()-1]);
                    pind=s.top();
                    cout<<"\nPop out segment"<<pind;
                    //s.pop();
                    }
                    s.pop();
                }
                else{

                    pind=s.top();
                    s.pop();
                    ch=0;
                }
            }
            cout<<"\nPush segment"<<pind;
            s.push(pind);
            s.push(eventQ1[frontQ].index);
        }
        frontQ++;

    }
    cout<<"\n!!!!  Polygon is triangulated  !!!!\n";
}


