//
//  sorting_main.c
//  sorting
//
//  Created by Renjun Zheng on 2/25/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct _node{
    long value;
    struct _node *next;
}Node;

typedef struct _list{
    Node *node;
    struct _list *next;
}List;

Node *Load_File(char * Filename);
int Save_File(char * Filename, Node * list);
Node *Shell_Sort(Node *list);

int main(int argc, char ** argv){
  if(argc != 3){
    printf("./proj2 input.txt output.txt. Need more!\n");
  }
  clock_t sortstart = 0;
  clock_t sortend = 0;
  clock_t loadstart = 0;
  clock_t loadend = 0;
  clock_t savestart = 0;
  clock_t saveend = 0;
  char * Input = argv[1];
  char * Output = argv[2];
  loadstart = clock();
  Node * head = Load_File(Input);
  loadend = clock();
  sortstart = clock();
  head = Shell_Sort(head);
  sortend = clock();
  savestart = clock();
  Save_File(Output, head);
  saveend = clock();
  printf("I/O time: %le\n",(double)(loadend -loadstart + saveend -savestart)/CLOCKS_PER_SEC);
  printf("Sorting time: %le\n",(double)(sortend - sortstart)/CLOCKS_PER_SEC);
  return 0;
}
