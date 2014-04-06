//
//  reroot.c
//  proj4
//
//  Created by Renjun Zheng on 3/29/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
typedef struct tree{
    double width;
    double height;
    struct tree * left;
    struct tree * right;
}Tree;

typedef struct stack{
    struct tree * node;
    struct stack * next;
}Stack;


#include <stdio.h>

int main(int argc, char * argv[])
{
    if(argc != 3){
        printf("input not enough: ./proj3 input_file output_file\n");
        return -1;
    }
    
    
    return 0;
}

Tree *treeCreate(double width, double height)
{
    tree *root = malloc(sizeof(tree));
    if(root == NULL)
        printf("malloc failure when malloc tree node\n");
    else{
        root -> width = width;
        root -> height = height;
        root -> left = NULL;
        root -> right = NULL;
    }
    return root;
}

void stackPush(Stack *top, Tree *node)
{
    Stack *block = malloc(sizeof(Stack));
    if(block == NULL)
        printf("malloc failure when malloc block node\n");
    else{
        block -> node = node;
        block -> next = top;
        top = block;
    }
}

Tree *stackPop(Stack *top)
{
    if(top == NULL)
        return NULL;
    else{
        Tree *node = top -> node;
        Stack *head = top;
        top = head -> next;
        free(head);
        return node;
    }
}


