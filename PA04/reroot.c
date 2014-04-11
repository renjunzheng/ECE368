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
    info -> leftwidth = root ->left -> width;
    info -> leftheight = root -> left -> height;
    info -> bestwidth = root -> width;
    info -> bestheight = root -> height;
    info -> num = -1;
    info -> index = -1;
    info -> cutline = '-';
    info -> rightwidth = root -> right -> width;
    info -> rightheight = root -> right -> height;
    treeReroot(root, info);
    printf("\n\n\n\nbest%le\n\n%le\n\n\n", info -> bestwidth, info -> bestheight);
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

void treeReroot(Tree *root, Info *info)
{
    if(root == NULL)return;
    double widthtemp = 0;
    double heighttemp = 0;
    if(info -> num == -1){
        info -> num = 1;
        treeReroot(root -> left, info);
        info -> num = 0;
        treeReroot(root -> right, info);
    }else{
        if(info -> index == -1){
            info -> index = 1;
            info -> cutline = root -> parent -> cutline;
            treeReroot(root -> left, info);
            info -> index = 0;
            info -> cutline = root -> parent -> cutline;
            treeReroot(root -> right, info);
        }
        if(info -> num == 1 && info -> index == 1){
            if(root -> parent -> cutline == 'H' && info -> cutline == 'H'){
                info -> rightwidth = bigger(root -> parent -> right -> width ,info -> rightwidth);
                info -> rightheight += root -> parent -> right -> height;
                widthtemp = bigger(info -> rightwidth, root -> width);
                heighttemp = root -> height + info -> rightheight;
                
                
                //confirm
            }else if(root -> parent -> cutline == 'H' && info -> cutline == 'V'){
                info -> rightwidth += root -> parent -> right -> width;
                info -> rightheight = bigger(root -> parent -> right -> width, info -> rightwidth);
                heighttemp = info -> rightheight + root -> height;
                widthtemp = bigger(info -> rightwidth, root -> width);
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline == 'H'){
                info -> rightwidth = bigger(root -> parent -> right -> width, info -> rightwidth);
                info -> rightheight += root -> parent -> right -> height;
                heighttemp = bigger(root -> height, info -> rightheight);
                widthtemp = info -> rightwidth + root -> width;
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline == 'V'){
                info -> rightheight = bigger(root -> parent -> right -> height, info -> rightheight);
                info -> rightwidth += root -> parent -> right -> width;
                widthtemp = root -> width + info -> rightwidth;
                heighttemp = bigger(root -> height, info -> rightheight);
                
                //confirm
            }
            if((widthtemp * heighttemp < info -> bestheight * info -> bestwidth) || ((widthtemp * heighttemp == info -> bestheight * info -> bestwidth)&&(widthtemp < info -> bestwidth))){
                info -> bestwidth = widthtemp;
                info -> bestheight = heighttemp;
            }
        }else if(info -> num == 1 && info -> index == 0){
            if(root -> parent -> cutline == 'H' && info -> cutline  == 'H'){
                info -> leftwidth = bigger(root -> parent -> left -> width, info -> leftwidth);
                info -> leftheight += root -> parent -> left -> height;
                widthtemp = bigger(root -> width, info -> leftwidth);
                heighttemp = info -> leftheight + root -> height;
                
                
               //confirm
            }else if(root -> parent -> cutline == 'H' && info -> cutline  == 'V'){
                info -> leftheight = bigger(root -> parent -> left -> height, info -> leftheight);
                info -> leftwidth += root -> parent -> left -> width;
                widthtemp = bigger(root -> width, info -> leftwidth);
                heighttemp = info -> leftheight + root -> height;
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline  == 'H'){
                info -> leftwidth = bigger(root -> parent -> left -> width, info -> leftwidth);
                info -> leftheight += root -> parent -> left -> height;
                widthtemp = root -> width + info -> leftwidth;
                heighttemp = bigger(root -> height, info -> leftheight);
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline == 'V'){
                info -> leftheight = bigger(root -> parent -> left -> height, info -> leftheight);
                info -> leftwidth += root -> parent -> left -> width;
                widthtemp = root -> width + info -> leftwidth;
                heighttemp = bigger(root -> height, info -> leftheight);
                
                //confirm
            }
            if((widthtemp * heighttemp < info -> bestheight * info -> bestwidth) || ((widthtemp * heighttemp == info -> bestheight * info -> bestwidth)&&(widthtemp < info -> bestwidth))){
                info -> bestwidth = widthtemp;
                info -> bestheight = heighttemp;
            }
        }else if(info -> num == 0 && info -> index == 1){
            if(root -> parent -> cutline == 'H' && info -> cutline  == 'H'){
                info -> rightwidth = bigger(root -> parent -> right -> width ,info -> rightwidth);
                info -> rightheight += root -> parent -> right -> height;
                widthtemp = bigger(root -> width, info -> rightwidth);
                heighttemp = info -> rightheight + root -> height;

                //confirm
            }else if(root -> parent -> cutline == 'H' && info -> cutline  == 'V'){
                info -> rightheight = bigger(root -> parent -> right -> height, info -> rightheight);
                info -> rightwidth += root -> parent -> left -> width;
                widthtemp = bigger(root -> width, info -> rightwidth);
                heighttemp = info -> rightheight + root -> height;
                
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline  == 'H'){
                info -> rightwidth = bigger(root -> parent -> right -> width ,info -> rightwidth);
                info -> rightheight += root -> parent -> right -> height;
                widthtemp = root -> width + info -> rightwidth;
                heighttemp = bigger(root -> height, info -> rightheight);
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline  == 'V'){
                info -> rightheight = bigger(root -> parent -> right -> height, info -> rightheight);
                info -> rightwidth += root -> parent -> left -> width;
                widthtemp = root -> width + info -> rightwidth;
                heighttemp = bigger(root -> height, info -> rightheight);
                
                //confirm
            }
            if((widthtemp * heighttemp < info -> bestheight * info -> bestwidth) || ((widthtemp * heighttemp == info -> bestheight * info -> bestwidth)&&(widthtemp < info -> bestwidth))){
                info -> bestwidth = widthtemp;
                info -> bestheight = heighttemp;
            }
        }else if(info -> num == 0 && info -> index == 0){
            if(root -> parent -> cutline == 'H' && info -> cutline  == 'H'){
                info -> leftwidth = bigger(root -> parent -> left -> width, info -> leftwidth);
                info -> leftheight += root -> parent -> left -> height;
                heighttemp = root -> height + info -> leftheight;
                widthtemp = bigger(root -> width, info -> leftwidth);
                
                //comfirm
            }else if(root -> parent -> cutline == 'H' && info -> cutline  == 'V'){
                info -> leftheight = bigger(root -> parent -> left -> height, info -> leftheight);
                info -> leftwidth += root -> parent -> left -> width;
                heighttemp = root -> height + info -> leftheight;
                widthtemp = bigger(root -> width, info -> leftwidth);
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline  == 'H'){
                info -> leftwidth = bigger(root -> parent -> left -> width, info -> leftwidth);
                info -> leftheight += root -> parent -> left -> height;
                widthtemp = root -> width + info -> leftwidth;
                heighttemp = bigger(root -> height, info -> leftheight);
                
                //confirm
            }else if(root -> parent -> cutline == 'V' && info -> cutline  == 'V'){
                info -> leftheight = bigger(root -> parent -> left -> height, info -> leftheight);
                info -> leftwidth += root -> parent -> left -> width;
                widthtemp = root -> width + info -> leftwidth;
                heighttemp = bigger(root -> height, info -> leftheight);

                //confirm
            }
            if((widthtemp * heighttemp < info -> bestheight * info -> bestwidth) || ((widthtemp * heighttemp == info -> bestheight * info -> bestwidth)&&(widthtemp < info -> bestwidth))){
                info -> bestwidth = widthtemp;
                info -> bestheight = heighttemp;
            }
        }
        info -> cutline = root -> parent -> cutline;
        info -> num = info -> index;
        info -> index = 1;
        treeReroot(root -> left, info);
        info -> index = 0;
        treeReroot(root -> right, info);
    }
}