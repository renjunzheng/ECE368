//
//  reroot.c
//  proj4
//
//  Created by Renjun Zheng on 3/29/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include "reroot.h"

int main(int argc, char * argv[])
{
    if(argc != 3){
        printf("input not enough: ./proj3 input_file output_file\n");
        return -1;
    }
    char *input_file = argv[1];
    char *output_file = argv[2];
    
    Tree *root = treeBuild(input_file);
    treeWidthHeight(root);
    printf("Preorder:");
    preOrderPrint(root);
    printf("\n\nInorder:");
    inOrderPrint(root);
    printf("\n\nPostorder:");
    postOrderPrint(root);
    printf("\n\nWidth:%le\nHeight:%le\n\n",root -> width, root -> height);
    treeCoordinate(root, -1);
    //find the left most
    Tree *leftmost = root;
    while(leftmost -> left != NULL)
        leftmost = leftmost -> left;
    printf("X-coordinate:  %le\nY-coordinate:  %le\n",leftmost -> xcoord, leftmost -> ycoord);
    FILE *fptr2 = fopen(output_file, "w");
    outputPrint(fptr2, root);
    fclose(fptr2);

    Info *info = malloc(sizeof(Info));
    info -> number = 0;
    info -> index = -1;
    info -> bestwidth = root -> width;
    info -> bestheight = root -> height;
    int value = 0;
    treeReroot(root, value, 'H', info, root -> left -> width, root -> left -> height, root -> right -> width, root -> right -> height);
    
    printf("\n\n\n\nbest%le\n\n%le\n\n\n",  info -> bestwidth, info -> bestheight);
    return 0;
}

//recursively calculate the tree width, height
void treeWidthHeight(Tree *root)
{
    if(root -> left == NULL || root -> right == NULL)return;
    else{
        treeWidthHeight(root -> left);
        treeWidthHeight(root -> right);
        if(root -> cutline == 'H'){
            if(root -> left -> width > root -> right -> width)
                root -> width = root -> left -> width;
            else
                root -> width = root -> right -> width;
            root -> height = root -> left -> height + root -> right -> height;
        }else if(root -> cutline == 'V'){
            if(root -> left -> height > root -> right -> height)
                root -> height = root -> left -> height ;
            else
                root -> height = root -> right -> height;
            root -> width = root -> left -> width + root -> right -> width;
        }
    }
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

void postOrderPrint(Tree *root)
{
    if(root == NULL) return;
    postOrderPrint(root -> left);
    postOrderPrint(root -> right);
    if(root -> cutline == 'H' || root -> cutline == 'V')
        printf("%c",root -> cutline);
    else
        printf("(%le,%le)",root -> width, root->height);
}

Tree *treeBuild(char *Filename)
{
    FILE *fptr = fopen(Filename,"r");
    if(fptr == NULL){
        printf("fail to read the input file\n");
        return NULL;
    }
    Stack *head = malloc(sizeof(Stack));
    double width = 0;
    double height = 0;
    char cutline;
    while(fscanf(fptr, "%c", &cutline) == 1){
        if(cutline == '('){
            fscanf(fptr, "%lf", &width);
            fscanf(fptr, "%c", &cutline);
            fscanf(fptr, "%lf", &height);
            Tree *node = treeCreate(width,height);
            head = stackPush(head, node);
            fscanf(fptr, "%c", &cutline);
        }else if(cutline == 'V'){
            Tree *vnode = malloc(sizeof(Tree));
            vnode -> cutline = 'V';
            vnode -> right = head -> node;
            vnode -> right -> parent = vnode;
            head = stackPop(head);
            vnode -> left = head -> node;
            vnode -> left -> parent = vnode;
            head = stackPop(head);
            head = stackPush(head, vnode);
            //even though it's unlikely the file store V or H before node, but it's better to check
        }else if(cutline == 'H'){
            Tree *hnode = malloc(sizeof(Tree));
            hnode -> cutline = 'H';
            hnode -> right = head -> node;
            hnode -> right -> parent = hnode;
            head = stackPop(head);
            hnode -> left = head -> node;
            hnode -> left -> parent = hnode;
            head = stackPop(head);
            head = stackPush(head, hnode);
        }
    }
    fclose(fptr);
    Tree *root = malloc(sizeof(Tree));
    root = head -> node;
    free(head);
    return root;
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
        root -> cutline = '-';
        root -> parent = NULL;
        root -> xcoord = 0;
        root -> ycoord = 0;
    }
    return root;
}

Stack *stackPush(Stack *top, Tree *node)
{
    Stack *block = malloc(sizeof(Stack));
    if(block == NULL)
        printf("malloc failure when malloc block node\n");
    else{
        block -> node = node;
        block -> next = top;
    }
    return block;
}

Stack *stackPop(Stack *top)
{
    if(top == NULL)
        return NULL;
    else{
        Stack *head = malloc(sizeof(Stack));
        head = top -> next;
        free(top);
        return head;
    }
}

void inOrderPrint(Tree * root)
{
    if(root == NULL)
        return;
    inOrderPrint(root -> left);
    if(root -> cutline == 'H' || root -> cutline == 'V')
        printf("%c",root -> cutline);
    else
        printf("(%le,%le)",root -> width, root->height);
    inOrderPrint(root -> right);
}

void outputPrint(FILE *fptr, Tree *root)
{
    if(root == NULL)return;
    outputPrint(fptr, root -> left);
    outputPrint(fptr, root -> right);
    if(root -> cutline == 'H' || root -> cutline == 'V')
        return;
    else
        fprintf(fptr,"%le %le %le %le\n", root -> width, root -> height, root -> xcoord, root -> ycoord);
}

void treeCoordinate(Tree *root, int num)
{
    if(root == NULL)return;
    if(num == 1){
        if(root -> parent -> cutline == 'H'){
            root -> xcoord = root -> parent -> xcoord;
            root -> ycoord = root -> parent -> ycoord + root -> parent -> right -> height;
        }else if(root -> parent -> cutline == 'V'){
            root -> ycoord = root -> parent -> ycoord;
            root -> xcoord = root -> parent -> xcoord;
        }
    }else if(num == 0){
        if(root -> parent -> cutline == 'H'){
            root -> xcoord = root -> parent -> xcoord;
            root -> ycoord = root -> parent -> ycoord;
        }else if(root -> parent -> cutline == 'V'){
            root -> ycoord = root -> parent -> ycoord;
            root -> xcoord = root -> parent -> xcoord + root -> parent -> left -> width;
        }
    }
    treeCoordinate(root -> left, 1);
    treeCoordinate(root -> right, 0);
}

double bigger(double a, double b){
    if(a > b)
        return a;
    else
        return b;
}

void treeReroot(Tree * root, int value, char cutline, Info *info, double leftwidth, double leftheight, double rightwidth, double rightheight)
{
    if(root == NULL) return;
    double widthtemp = 0;
    double heighttemp = 0;
    if(value > 1){
        if(cutline == 'H' && root -> parent -> cutline == 'H' && info -> number == 0 && info -> index == 0){
            rightwidth = bigger(root -> parent -> right -> width , rightwidth);
            rightheight = root -> parent -> right -> height + rightheight;
            widthtemp = bigger(root -> width, rightwidth);
            heighttemp = rightheight + root -> height;
        }else if(cutline == 'H' && root -> parent -> cutline == 'H' && info -> number == 0 && info -> index == 1){
            leftwidth = bigger(root -> parent -> right -> width, leftwidth);
            leftheight = root -> parent -> right -> height + leftheight;
            widthtemp = bigger(root -> width, leftwidth);
            heighttemp = leftheight + root -> height;
        }else if(cutline == 'H' && root -> parent -> cutline == 'H' && info -> number == 1 && info -> index == 0){
            rightwidth = bigger(root -> parent -> left -> width, rightwidth);
            rightheight = root -> parent -> left -> height + rightheight;
            widthtemp = bigger(root -> width, rightwidth);
            heighttemp = rightheight + root -> height;
        }else if(cutline == 'H' && root -> parent -> cutline == 'H' && info -> number == 1 && info -> index == 1){
            leftwidth = bigger(root -> parent -> left -> width, leftwidth);
            leftheight = root -> parent -> left -> height + leftheight;
            widthtemp = bigger(root -> width, leftwidth);
            heighttemp = leftheight + root -> height;
        }else if(cutline == 'H' && root -> parent -> cutline == 'V' && info -> number == 0 && info -> index == 0){
            rightwidth = bigger(root -> parent -> right -> width, rightwidth);
            rightheight = root -> parent -> right -> height + rightheight;
            heighttemp = bigger(root -> height, rightheight);
            widthtemp = rightwidth + root -> width;
        }else if(cutline == 'H' && root -> parent -> cutline == 'V' && info -> number == 0 && info -> index == 1){
            leftwidth = bigger(root -> parent -> right -> width, leftwidth);
            leftheight = root -> parent -> right -> height + leftheight;
            heighttemp = bigger(root -> height, leftheight);
            widthtemp = leftwidth + root -> width;
        }else if(cutline == 'H' && root -> parent -> cutline == 'V' && info -> number == 1 && info -> index == 0){
            rightwidth = bigger(root -> parent -> left -> width, rightwidth);
            rightheight = root -> parent -> left -> height + rightheight;
            heighttemp = bigger(rightheight, root -> height);
            widthtemp = rightwidth + root -> width;
        }else if(cutline == 'H' && root -> parent -> cutline == 'V' && info -> number == 1 && info -> index == 1){
            leftwidth = bigger(root -> parent -> left -> width, leftwidth);
            leftheight = root -> parent -> left -> height + leftheight;
            heighttemp = bigger(root -> height, leftheight);
            widthtemp = leftwidth + root -> width;
        }else if(cutline == 'V' && root -> parent -> cutline == 'H' && info -> number == 0 && info -> index == 1){
            leftwidth = leftwidth + root -> parent -> right -> width;
            leftheight = bigger(root -> parent -> right -> height, leftheight);
            widthtemp = bigger(leftwidth, root -> width);
            heighttemp = leftheight + root -> height;
        }else if(cutline == 'V' && root -> parent -> cutline == 'H' && info -> number == 0 && info -> index == 0){
            rightheight = bigger(root -> parent -> right -> height, rightheight);
            rightwidth = rightwidth + root -> parent -> right -> width;
            widthtemp = bigger(root -> width, rightwidth);
            heighttemp = rightheight + root -> height;
        }else if(cutline == 'V' && root -> parent -> cutline == 'H' && info -> number == 1 && info -> index == 1){
            leftheight = bigger(root -> parent -> left -> height, leftheight);
            leftwidth = leftwidth + root -> parent -> left -> width;
            widthtemp = bigger(root -> width, leftwidth);
            heighttemp = leftheight + root -> height;
        }else if(cutline == 'V' && root -> parent -> cutline == 'H' && info -> number == 1 && info -> index == 0){
            rightheight = bigger(root -> parent -> left -> height, rightheight);
            rightwidth = rightwidth + root -> parent -> left -> width;
            widthtemp = bigger(root -> width, rightwidth);
            heighttemp = rightheight + root -> height;
        }else if(cutline == 'V' && root -> parent -> cutline == 'V' && info -> number == 0 && info -> index == 1){
            leftwidth = root -> parent -> right -> width + leftwidth;
            leftheight = bigger(root -> parent -> right -> height, leftheight);
            heighttemp = bigger(root -> height, leftheight);
            widthtemp = leftwidth + root -> width;
        }else if(cutline == 'V' && root -> parent -> cutline == 'V' && info -> number == 0 && info -> index == 0){
            rightwidth = root -> parent -> right -> width + rightwidth;
            rightheight = bigger(root -> parent -> right -> height, rightheight);
            heighttemp = bigger(root -> height, rightheight);
            widthtemp = rightwidth + root -> width;
        }else if(cutline == 'V' && root -> parent -> cutline == 'V' && info -> number == 1 && info -> index == 1){
            leftwidth = root -> parent -> left -> width + leftwidth;
            leftheight = bigger(root -> parent -> left -> height, leftheight);
            heighttemp = bigger(root -> height, leftheight);
            widthtemp = leftwidth + root -> width;
        }else if(cutline == 'V' && root -> parent -> cutline == 'V' && info -> number == 1 && info -> index == 0){
            rightwidth = root -> parent -> left -> width + rightwidth;
            rightheight = bigger(root -> parent -> left -> height, rightheight);
            heighttemp = bigger(root -> height, rightheight);
            widthtemp = rightwidth + root -> width;
        }
        if((heighttemp * widthtemp < info -> bestwidth * info -> bestheight) || ((heighttemp * widthtemp == info -> bestwidth * info -> bestheight) && (widthtemp < info -> bestwidth)))
        {
            info -> bestwidth = widthtemp;
            info -> bestheight = heighttemp;
        }
    }
    if(root -> parent != NULL)
        cutline = root -> parent -> cutline;
    if(info -> index == -1)
    {
        info -> number = 0;
        info -> index = 0;
        treeReroot(root -> left, value + 1,  cutline, info, leftwidth, leftheight, rightwidth, rightheight);
        info -> number = 1;
        info -> index = 1;
        treeReroot(root -> right, value + 1,  cutline, info, leftwidth, leftheight, rightwidth, rightheight);
        return;
    }
    info -> number = 0;
    treeReroot(root -> left, value + 1 , cutline , info, leftwidth, leftheight, rightwidth, rightheight);
    info -> number = 1;
    treeReroot(root -> right, value + 1 , cutline , info, leftwidth, leftheight, rightwidth, rightheight);
}