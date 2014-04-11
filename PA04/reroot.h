//
//  reroot.h
//  proj4
//
//  Created by Renjun Zheng on 3/29/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
#ifndef hw04_h
#define hw04_h

typedef struct tree{
    double width;
    double height;
    char cutline;
    struct tree * left;
    struct tree * right;
    struct tree * parent;
    double xcoord;
    double ycoord;
}Tree;

typedef struct stack{
    struct tree * node;
    struct stack * next;
}Stack;

typedef struct info{
    char cutline;//imaginary grandparent cutline
    double bestheight;
    double bestwidth;
    double size;
    int number;
    int index;
    int value;
}Info;

void preOrderPrint(Tree * root);
Tree *treeBuild(char *Filename);
Tree *treeCreate(double width, double height);
Stack *stackPush(Stack *top, Tree *node);
Stack *stackPop(Stack *top);
void postOrderPrint(Tree *root);
void treeWidthHeight(Tree *root);
void inOrderPrint(Tree *root);
void treeCoordinate(Tree *root, int num);
void outputPrint(FILE *fptr, Tree *root);
void treeReroot(Tree * root, int value, char cutline, Info *info, double leftwidth, double leftheight, double rightwidth, double rightheight);

#endif
