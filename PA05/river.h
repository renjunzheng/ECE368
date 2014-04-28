//
//  river.h
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#ifndef hw05_h
#define hw05_h

typedef struct _bridge{
    int index;
    int turns;
    struct _bridge * next;
}Bridge;

typedef struct _vertex{
    int index;
    int x; //x coordinate of the vertex
    int yt; //top coordinate of the vertex
    int yb; //bottome coordinate of the vertex
    struct _vertex *next;
    Bridge *node;
}Vertex;
#endif

Vertex *buildVertex(int row, int column, int poles[row][column], int *index);
Vertex *createVertex(int x, int y, int index);
Bridge *createBridge(int index, int source, Vertex *head);