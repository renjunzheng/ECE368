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
    char cutline;
    struct tree * left;
    struct tree * right;
}Tree;

typedef struct stack{
    struct tree * node;
    struct stack * next;
}Stack;


#include <stdio.h>
void preOrderPrint(Tree * root);
Tree *treebuild(char *Filename);
int stackEmpty(Stack *top);
Tree *treeCreate(double width, double height);
void stackPush(Stack *top, Tree *node);
Tree *stackPop(Stack *top);

int main(int argc, char * argv[])
{
    if(argc != 3){
        printf("input not enough: ./proj3 input_file output_file\n");
        return -1;
    }
    char *input_file = argv[1];
    char *output_file = argv[2];
    
    Tree *root = treeBuild(input_file);
    preOrderPrint(root);
    return 0;
}

void preOrderPrint(Tree * root)
{
    if(root == NULL)return;
    if(root -> cutline == 'H' || root -> cutline == 'V')
        printf("%c",root -> cutline);
    else
        printf("(%le,%le)",root -> width, root->height);
    preOrderPrint(root -> left);
    preOrderPrint(root -> right);

}


Tree *treebuild(char *Filename)
{
    FILE *fptr = fopen(Filename,"r");
    if(fptr == NULL){
        printf("fail to read the input file\n");
        return NULL;
    }
    Stack *head = malloc(sizeof(Stack));
    head -> node = NULL;
    head -> next = NULL;
    double width = 0;
    double height = 0;
    char cutline;
    Tree *root;
    while(fscanf(fptr, "%c", &cutline) == 1){
        if(cutline == "("){
            while((fscanf(fptr, "%le", &width) == 1) && (fscanf(fptr, "%le", &height) == 1)){
                Tree *node = treeCreate(width,height);
                stackPush(head, node);
            }
        }else if(cutline == 'V'){
            if(stackEmpty(head) == 0){
                Tree *vnode = treeCreate(0,0);
                vnode -> right = stackPop(head);
                vnode -> left = stackPop(head);
                stackPush(head, vnode);
                root = vnode;
            }
            //even though it's unlikely the file store V or H before node, but it's better to check
        }else if(cutline == 'H'){
            if(stackEmpty(head) == 0){
                Tree *hnode = treeCreate(-1,-1);
                hnode -> right = stackPop(head);
                hnode -> left = stackPop(head);
                stackPush(head, hnode);
                root = hnode;
            }
        }
    }
    return root;
}

int stackEmpty(Stack *top)
{
    if(top -> node == NULL)return 1;
    else return 0;
}


Tree *treeCreate(double width, double height)
{
    Tree *root = malloc(sizeof(Tree));
    if(root == NULL)
        printf("malloc failure when malloc tree node\n");
    else{
        root -> width = width;
        root -> height = height;
        root -> left = NULL;
        root -> right = NULL;
        if(width == 0 && height == 0)
            root -> cutline = 'V';
        else if(width == -1 && height == -1)
            root -> cutline = 'H';
        else
            root -> cutline = '-';
    }
    return root;
}

void stackPush(Stack *top, Tree *node)
{
    if(stackEmpty(top) == 0){
        Stack *block = malloc(sizeof(Stack));
        if(block == NULL)
            printf("malloc failure when malloc block node\n");
        else{
            block -> node = node;
            block -> next = top;
            top = block;
        }
    }else{
        top -> node = node;
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


