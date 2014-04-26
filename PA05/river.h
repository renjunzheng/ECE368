//
//  river.h
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#ifndef hw05_h
#define hw05_h

typedef struct _vertex{
    int x; //x coordinate of the vertex
    int yt; //top coordinate of the vertex
    int yb; //bottome coordinate of the vertex
}Vertex;

typedef struct _bridge{
    int index;
    struct _bridge * next;
}Bridge;

#endif