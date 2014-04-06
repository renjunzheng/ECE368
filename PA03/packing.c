//
//  packing.c
//  packing
//
//  Created by Renjun Zheng on 3/23/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "packing.h"

int main(int argc, char * argv[])
{
  //check argument number
  if(argc != 3){
    printf("./proj3 input_file output_file\n");
  }
    
  //initialize the input file
  FILE * fptr = NULL;
  fptr = fopen(argv[1], "r");
  if(fptr == NULL){
    printf("fail to initialize input file\n");
    return -1;
  }
  int number_of_blocks = 0;
  int number_of_nodes = 0;
  fscanf(fptr, "%d", &number_of_blocks);
  fscanf(fptr, "%d", &number_of_nodes);

  Node **tree = malloc(sizeof(Node) * number_of_nodes);
  if(tree == NULL)
    printf("fail to malloc the whole table\n");
    
  int i = 0;
  for(i = 0; i < number_of_nodes; i++){
    tree[i] = malloc(sizeof(Node));
    if(tree[i] == NULL){
      printf("fail to malloc the each node\n");
      break;
    }
    fscanf(fptr, "%d", &tree[i] -> thisnode);;
    fscanf(fptr, "%d", &tree[i] -> parnode);
    fscanf(fptr, "%d", &tree[i] -> lcnode);
    fscanf(fptr, "%d", &tree[i] -> rcnode);
    fscanf(fptr, " %c", &tree[i] -> cutline);;
    fscanf(fptr, "%lf", &tree[i] -> width);
    fscanf(fptr, "%lf", &tree[i] -> height);
    tree[i] -> xcoord = 0;
    tree[i] -> ycoord = 0;
  }
  fclose(fptr);
  //find the root node
  int root = 0;
  for(i = 0; i < number_of_nodes; i++){
    if(tree[i] -> parnode == -1)
      root = i;
  }
  
  clock_t start = 0;
  clock_t end = 0;
  start = clock();
  //screen dump
  Print(tree, root, number_of_blocks);
  end = clock();
  printf("\nElapsed Time:  %le\n",(double)(end - start)/CLOCKS_PER_SEC);

  //save the output file
  FILE * fptr2 = NULL;
  fptr2 = fopen(argv[2], "w");
  if(fptr2 == NULL){
    printf("fail to initialize output file\n");
    return -1;
  }
  fprintf(fptr2, "%d\n", number_of_blocks);
  for(i = 0; i < number_of_blocks; i++)
    {
      fprintf(fptr2, "%d %le %le %le %le\n", i + 1, tree[i] -> width, tree[i] -> height, tree[i] -> xcoord, tree[i] -> ycoord);
    }
  fclose(fptr2);

  Node_destroy(tree, number_of_nodes);
  return 0;
}

void Print(Node **tree, int root, int number_of_blocks)
{
  //preorder print
  printf("Preorder:  ");
  Preorder_print(tree, root + 1);
  printf("\n\n");
    
  //inorder print
  printf("Inorder:  ");
  Inorder_print(tree, root + 1);
  printf("\n\n");

  //postoder print
  printf("Postorder:  ");
  Postorder_print(tree,root + 1);
  printf("\n\n");

  //calculate the width and height
  WidthHeight(tree, root + 1);
  printf("Width:  %le\nHeight:  %le\n\n", tree[root] -> width, tree[root] -> height);

  //calculate the coordinate
  Coordinate(tree, root + 1, -1);
  printf("X-coordinate:  %le\nY-coordinate:  %le\n",tree[number_of_blocks - 1] -> xcoord, tree[number_of_blocks - 1] -> ycoord);
  return;
}

void Coordinate(Node **tree, int root, int index)
{
  if(root < 0)
    return;
  //left
  if(index == 1){
    if(tree[tree[root - 1] -> parnode - 1] -> cutline == 'H'){
      tree[root - 1] -> xcoord = tree[tree[root - 1] -> parnode - 1] -> xcoord;
      tree[root - 1] -> ycoord = tree[tree[root - 1] -> parnode - 1] -> ycoord + tree[tree[tree[root - 1] -> parnode - 1] -> rcnode - 1] -> height;
    }else if(tree[tree[root - 1] -> parnode - 1] -> cutline == 'V'){
      tree[root - 1] -> xcoord = tree[tree[root - 1] -> parnode - 1] -> xcoord;
      tree[root - 1] -> ycoord = tree[tree[root - 1] -> parnode - 1] -> ycoord;
    }
  }else if(index == 0){//right
    if(tree[tree[root - 1] -> parnode - 1] -> cutline == 'H'){
      tree[root - 1] -> xcoord = tree[tree[root - 1] -> parnode - 1] -> xcoord;
      tree[root - 1] -> ycoord = tree[tree[root - 1] -> parnode - 1] -> ycoord;
    }else if(tree[tree[root - 1] -> parnode - 1] -> cutline == 'V'){
      tree[root - 1] -> ycoord = tree[tree[root - 1] -> parnode - 1] -> ycoord;
      tree[root - 1] -> xcoord = tree[tree[root - 1] -> parnode - 1] -> xcoord + tree[tree[tree[root - 1] -> parnode - 1] -> lcnode - 1] -> width;
    }
  }
  Coordinate(tree, tree[root - 1] -> lcnode,1);
  Coordinate(tree, tree[root - 1] -> rcnode,0);
  return;
}

void Preorder_print(Node **tree, int root)
{
  if(root < 0)
    return;
  printf("%d ",tree[root - 1] -> thisnode);
  Preorder_print(tree,tree[root - 1] -> lcnode);
  Preorder_print(tree,tree[root - 1] -> rcnode);
  return;
}

void Inorder_print(Node **tree, int root)
{
  if(root < 0)
    return;
  Inorder_print(tree,tree[root - 1] -> lcnode);
  printf("%d ",tree[root - 1] -> thisnode);
  Inorder_print(tree,tree[root - 1] -> rcnode);
  return;
}

void Postorder_print(Node **tree, int root)
{
  if(root < 0)
    return;
  Postorder_print(tree,tree[root - 1] -> lcnode);
  Postorder_print(tree,tree[root - 1] -> rcnode);
  printf("%d ",tree[root - 1] -> thisnode);
  return;
}

void WidthHeight(Node **tree, int root)
{
  if(root < 0)
    return;
  WidthHeight(tree, tree[root - 1] -> lcnode);
  WidthHeight(tree,tree[root - 1] -> rcnode);
  if(tree[root - 1] -> cutline == 'H'){
    //width
    if((tree[tree[root - 1] -> lcnode - 1] -> width) > (tree[tree[root - 1] -> rcnode - 1] -> width))
      tree[root - 1] -> width = tree[tree[root - 1] -> lcnode - 1] -> width;
    else
      tree[root - 1] -> width = tree[tree[root - 1] -> rcnode - 1] -> width;
    //height
    tree[root - 1] -> height = tree[tree[root - 1] -> lcnode - 1] -> height + tree[tree[root - 1] -> rcnode - 1] -> height;
  }else if(tree[root - 1] -> cutline == 'V'){
    if((tree[tree[root - 1] -> lcnode - 1] -> height) > (tree[tree[root - 1] -> rcnode - 1] -> height))
      tree[root - 1] -> height = tree[tree[root -1 ] -> lcnode - 1] -> height;
    else
      tree[root - 1] -> height = tree[tree[root - 1] -> rcnode - 1] -> height;
    tree[root - 1] -> width = tree[tree[root - 1] -> lcnode - 1] -> width + tree[tree[root - 1] -> rcnode - 1] -> width;    
  }
  return;
}

void Node_destroy(Node **tree, int number_of_nodes)
{
  int i = 0;
  for(i = 0; i < number_of_nodes; i++)
    {
      free(tree[i]); 
    }
  free(tree);
  return;
}
