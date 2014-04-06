//
//  packing.h
//  packing
//
//  Created by Renjun Zheng on 3/25/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
#ifndef hw03_h
#define hw03_h

typedef struct node{
  int thisnode;
  int parnode;
  int lcnode;
  int rcnode;
  char cutline;
  double width;
  double height;
  double xcoord;
  double ycoord;
}Node;
void Print(Node **tree, int root, int number_of_blocks);
void Preorder_print(Node **tree, int root);
void Inorder_print(Node **tree, int root);
void Postorder_print(Node **tree, int root);
void WidthHeight(Node **tree, int root);
void Coordinate(Node **tree, int root, int index);
void Node_destroy(Node **tree, int number_of_nodes);

#endif
