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

#include <stdlib.h>
#include <stdio.h>
void preOrderPrint(Tree * root);
Tree *treeBuild(char *Filename);
Tree *treeCreate(double width, double height);
Stack *stackPush(Stack *top, Tree *node);
Stack *stackPop(Stack *top);
void postOrderPrint(Tree *root);
void treeWidthHeight(Tree *root);

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
  preOrderPrint(root);
  postOrderPrint(root);
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
      head = stackPop(head);
      vnode -> left = head -> node;
      head = stackPop(head);
      head = stackPush(head, vnode);
      //even though it's unlikely the file store V or H before node, but it's better to check
    }else if(cutline == 'H'){
      Tree *hnode = malloc(sizeof(Tree));
      hnode -> cutline = 'H';
      hnode -> right = head -> node;
      head = stackPop(head);
      hnode -> left = head -> node;
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
