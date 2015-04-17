//
//  search.cpp
//  Working
//
//  Created by Rahul Agarwal on 12/09/14.
//  Copyright (c) 2014 USC. All rights reserved.
//

#include "search.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <queue>
#include <algorithm>
#include <string>
#include <stack>
#include <iterator>
#include <fstream>

#define size 100

using namespace std;

struct  Node{
    char name[size];
    int no;
    bool status;
    struct Node* parent;
    int pathcost ;
} ;

struct Graph {
    int v;
    int **adjmat;
};

struct Graph g;
struct Node* nodes;
int i,j;


char src[size],dest[size];
int task;
static int nodenosrc = 0,nodenodest = 0;
struct Node* a = NULL;
struct Node* temp = NULL;
static int bfscost = 0;

void createGraph();
struct Node* bfs(int,int,int);
struct Node* dfs(int,int,int);
struct Node* ucs(int,int,int);
bool compareForBfsAndDfs(struct Node* , struct Node* );
bool compareForUcs(struct Node* , struct Node* );
void display(struct Node*, int);


FILE *fp;
FILE *ofp;


int main(){
    fp = fopen("./input.txt", "r");///Users/rahulagarwal/Desktop
    ofp = fopen("./output.txt", "w");///Users/rahulagarwal/Desktop/
    fscanf(fp,"%d",&task);
    fscanf(fp,"%s",src);
    fscanf(fp,"%s",dest);
    fscanf(fp,"%d",&g.v);
    
    
    nodenosrc = nodenodest = -1;
    //    cout <<task<<endl;
    //    cout <<src<<endl;
    //    cout <<dest<<endl;
    //    cout <<g.v<<endl;
    
    // array of nodes
    nodes = (struct Node*)malloc(sizeof(struct Node)*g.v);
    for(i = 0 ;i < g.v ; i++){
        fscanf(fp,"%s",nodes[i].name);
        
        nodes[i].no = i;
        nodes[i].pathcost = 0;
        nodes[i].status  = false;
    }
    
    for(i =0 ; i < g.v ; i++){
        
        if(!strcmp(nodes[i].name,src))
            nodenosrc = i;
        
    }
    
    nodes[nodenosrc].parent = NULL;
    nodes[nodenosrc].pathcost = 0;
    
    
    for(i =0 ; i < g.v ; i++){
        if(!strcmp(nodes[i].name ,dest))
            nodenodest = i;
        
    }
    
    // matrix representation
    g.adjmat = (int**)malloc(sizeof(int)*g.v*g.v);
    for(i = 0 ; i < g.v ;i++)
        g.adjmat[i] = (int*)malloc(sizeof(int)*g.v);
    
    // graph creation
    createGraph();
    
    // point to diffrent algorithms
    if(task == 1){
        a = bfs(nodenosrc,nodenodest,g.v);
        
        if (a == NULL){
            fprintf(ofp,"\n");
            fprintf(ofp,"NoPathAvailable");
            fprintf(ofp,"\n");
            // ofile<<endl;
            //ofile<<"NoPathAvailable";
            //ofile<<endl;
        }
        else {
            display(a, task);
        }
    }
    else if(task == 2){
        a =  dfs(nodenosrc,nodenodest,g.v);
        if (a == NULL){
            fprintf(ofp,"\n");
            fprintf(ofp,"NoPathAvailable");
            fprintf(ofp,"\n");
            //ofile<<endl;
            //ofile<<"NoPathAvailable";
            //ofile<<endl;
        }
        else {
            display(a, task);
        }
    }
    else if(task == 3){
        a =  ucs(nodenosrc,nodenodest,g.v);
        
        if (a == NULL){
            fprintf(ofp,"\n");
            fprintf(ofp,"NoPathAvailable");
            fprintf(ofp,"\n");
            
        }
        else {
            display(a, task);
        }
        
    }
    
    // freeing up the memory.
    
    
    
    a = NULL;
    temp = NULL;
    delete nodes;
    delete g.adjmat;
    
    return 0;
}

void createGraph(){
    
    for( i = 0 ;i < g.v ; i++){
        for( j = 0 ; j < g.v;j++){
            fscanf(fp,"%d",&(g.adjmat[i][j]));
            
            
            g.adjmat[j][i] = g.adjmat[i][j];
        }
        
    }
    
}

struct Node* bfs(int nodenosrc , int nodenodest ,int n){
    
    bool* visited = new bool[n];
    for(i = 0 ; i < n ;i++)
        visited[i] = false;
    
    list<struct Node*> queue;
    
    
    visited[nodenosrc] = true;
    queue.push_back(&nodes[nodenosrc]);
    
    if(queue.empty())
        return temp;
    //fprintf(ofp,"Log:");
    fprintf(ofp,"%s",nodes[nodenosrc].name);
    while(!queue.empty()){
        temp = queue.front();
        queue.pop_front();
        
        if(temp->name != nodes[nodenosrc].name){
            
            fprintf(ofp,"-");
            fprintf(ofp,"%s",temp->name);
        }
        if(temp->no == nodenodest){
            return temp;
        }
        // cout <<g.adjmat[temp->no][i]<<endl;
        for(i = 0; i < g.v;i++){
            if(g.adjmat[temp->no][i] != 0){
                if(!visited[i]){
                    
                    visited[i] = true;
                    queue.push_back(&nodes[i]);
                    nodes[i].parent = temp;
                    nodes[i].pathcost = nodes[i].parent->pathcost +  1;
                    
                }
            }
            
        }
        queue.sort(compareForUcs);
    }
    return NULL;
}

struct Node* dfs(int nodenosrc,int nodenodest,int n){
    
    
    bool* visited = new bool[n];
    for(i = 0 ; i < n ;i++)
        visited[i] = false;
    
    
    list <struct Node*> queue;
    list<struct Node*> tempqueue;
    
    
    visited[nodenosrc] = true;
    
    queue.push_front(&nodes[nodenosrc]);
    list<struct Node*>::iterator it1 = tempqueue.begin();
    
    
    if(queue.empty())
        return temp;
    
    //fprintf(ofp,"Log:");
    fprintf(ofp,"%s",nodes[nodenosrc].name);
    
    while(!queue.empty()){
        temp = queue.back();
        if(temp->name != nodes[nodenosrc].name){
            
            fprintf(ofp,"-");
            fprintf(ofp,"%s",temp->name);
            
        }
        queue.pop_back();
        temp->status = true;
        
        if(temp->no == nodenodest){
            return temp;
        }
        
        for(i = 0; i < g.v;i++){
            
            
            if(g.adjmat[temp->no][i] != 0){
                if(!visited[i]){
                    visited[i] = true;
                    tempqueue.push_back(&nodes[i]);
                    nodes[i].parent = temp;
                    nodes[i].pathcost =  nodes[i].parent->pathcost + g.adjmat[temp->no][i];
                }
            }
            
            
            
        }
        
        
        
        tempqueue.sort(compareForBfsAndDfs);
        tempqueue.reverse();
        for(it1=tempqueue.begin(); it1 != tempqueue.end(); ++it1) {
            queue.push_back(*it1);
            tempqueue.pop_front();
            
        }
        
        //        for(auto a: tempqueue){
        //            queue.push_back(a);
        //            tempqueue.pop_front();
        //        }
    }
    
    
    
    
    
    return NULL;
}

struct Node* ucs(int nodenosrc,int nodenodest,int n){
    
    bool* visited = new bool[n];
    for(i = 0 ; i < n ;i++)
        visited[i] = false;
    
    list<struct Node*> queue;
    //list<struct Node*> tempqueue;
    
    visited[nodenosrc] = true;
    queue.push_back(&nodes[nodenosrc]);
    list<struct Node*>::iterator it1 = queue.begin();
    if(queue.empty())
        return temp;
    //fprintf(ofp,"Log:");
    fprintf(ofp,"%s",nodes[nodenosrc].name);
    
    while(!queue.empty()){
        temp = queue.front();
        if(temp->name != nodes[nodenosrc].name){
            //        cout <<"-"<< temp->name;
            // ofile <<"-"<< temp->name;
            fprintf(ofp,"-");
            fprintf(ofp,"%s",temp->name);
        }
        queue.pop_front();
        
        if(temp->no == nodenodest){
            //cout <<temp->name;
            return temp;
        }
        
        for(i = 0; i < g.v;i++){
            if(g.adjmat[temp->no][i] != 0){
                
                if(!visited[i]){
                    visited[i] = true;
                    queue.push_back(&nodes[i]);
                    nodes[i].parent = temp;
                    nodes[i].pathcost =   nodes[i].parent->pathcost +  g.adjmat[temp->no][i];
                }
                else{
                    for( it1=queue.begin(); it1 != queue.end(); ++it1){
                        if(*it1 == (&nodes[i]))
                            if((*it1)->pathcost > (g.adjmat[temp->no][i] +  temp->pathcost)){
                                queue.remove(*it1);
                                //                                nodes[i].parent = temp;
                                //                                nodes[i].pathcost =  nodes[i].pathcost + nodes[i].parent->pathcost +  g.adjmat[temp->no][i];
                                queue.push_back(&nodes[i]);
                                nodes[i].parent = temp;
                                nodes[i].pathcost =   nodes[i].parent->pathcost +  g.adjmat[temp->no][i];
                                
                            }
                    }
                    
                }
            }
        }
        queue.sort(compareForUcs);
    }
    
    return NULL;
}

bool compareForBfsAndDfs(struct Node* lhs, struct Node* rhs){
    
        if(strcmp(lhs->name,rhs->name) < 0) {
            return true;
        }
        else if(strcmp(lhs->name,rhs->name) > 0) {
            return false;
        }
        else{
            return true;
        }
    
}

/*bool compareForUcs(struct Node* lhs , struct Node* rhs){
 if(lhs->pathcost!= rhs->pathcost)
 return lhs->pathcost < rhs->pathcost;
 else if(lhs->name != rhs->name)
 return lhs->name < rhs->name;
 else
 return 0;
 
 } */

bool compareForUcs(struct Node* lhs , struct Node* rhs){
    if(lhs->pathcost < rhs->pathcost)
        return true;
    else if(lhs->pathcost > rhs->pathcost)
        return false;
    else {
        if(strcmp(lhs->name,rhs->name) < 0) {
            return true;
        }
        else if(strcmp(lhs->name,rhs->name) > 0) {
            return false;
        }
        else{
            return true;
        }
    }
}


void display(struct Node* a, int task){
    
    list<string> queue;
    list<string>::iterator it1 = queue.begin();
    while(a->name != nodes[nodenosrc].name){
        queue.push_back(a->name);
        bfscost += g.adjmat[a->no][a->parent->no];
        a = a->parent;
        
    }
    
    fprintf(ofp,"\n");
    //fprintf(ofp,"Path:");
    
    queue.reverse();
    
    fprintf(ofp,"%s",nodes[nodenosrc].name);
    
    
    
    
    for (it1=queue.begin(); it1 != queue.end(); ++it1){
        
        fprintf(ofp,"-");
        fprintf(ofp,"%s",(*it1).c_str());
    }
    if(task != 1){
        fprintf(ofp,"\n");
        fprintf(ofp,"%d",nodes[nodenodest].pathcost);
        fprintf(ofp,"\n");
    }
    else{
        fprintf(ofp,"\n");
        fprintf(ofp,"%d",bfscost);
        fprintf(ofp,"\n");
    }
}



