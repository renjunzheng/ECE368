//
//  sorting.c
//
//  Created by Renjun Zheng on 2/25/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct _node{
  long value;
  struct _node *next;
}Node;

typedef struct _list{
  Node *node;
  struct _list *next;
}List;

Node * Load_File(char *Filename)
{
  FILE * fptr = fopen(Filename,"r");
  if(fptr == NULL)
    return NULL;
    
  long value = 0;
  //create a header
  Node *head = NULL;
    
  //store all the value into a linked list
  while(fscanf(fptr, "%ld", &value) == 1)
    {
      Node *nd = malloc(sizeof(Node));
      nd -> value = value;
      nd -> next = head;
      head = nd;
    }
   
  fclose(fptr);
  return head;
}

int Save_File(char *Filename, Node *list)
{
  int num = 0;
  if(list != NULL)
    {
      FILE * fptr = fopen(Filename, "w");
      if(fptr == NULL)
	return -1;
      while(list != NULL)
        {
	  fprintf(fptr, "%ld\n", list -> value);
	  list = list -> next;
	  num ++;
        }
      fclose(fptr);
    }
    
  return num;
}

Node *Shell_Sort(Node *list)
{
  int size = 0;
  int level = 0;
  Node * head = list;
  while(list != NULL){
    size ++;
    list = list -> next;
  }
  int tsize = size;
  while(size > 0){
    size = size/3;
    level++;
  }
  int k = 1;
  int i = 0;
  int l = 0;
  int z = 0;
  int n = 0;
  //implement the shell sort by insertion sort
  for(i = level; i > 0; i--){
    for(l = 0; l < i - 1; l++)
      k = k*3;
    for(n = 0; n < i ; n++){//insertion sort for each sub-array
      //build the linked list and link the list head with the node
      List * lshead = malloc(sizeof(List));
      lshead -> node = head;
      lshead -> next = NULL;
      head = head -> next;
      lshead -> node -> next = NULL;
      for(z = 0; z < k - 1; z++){
	List * ls = malloc(sizeof(List));
	ls -> next = lshead;
	lshead = ls;
	lshead -> node = head;
	head = head -> next;
	lshead -> node -> next = NULL; 
      }
      List * listhead = lshead;
 
      lshead = listhead;
      //build the sublist
      while((lshead != NULL) && (head != NULL)){
	Node * tempN = head; 
	head = head -> next;
	tempN -> next = NULL;
	//already get the node need to insert then compare value and insert
	Node * thead = lshead -> node;
	Node * theadbefore = lshead -> node;
	//find where to insert
	while((thead != NULL) && (tempN -> value > thead -> value)){
	  theadbefore = thead;
	  thead = thead -> next;
	}
	//for different situation, insert the node
	if(thead == NULL){
	  theadbefore -> next = tempN;
	  tempN -> next = thead;
	}
	if((thead != NULL) && (theadbefore != lshead -> node)){
	  theadbefore -> next = tempN;
	  tempN -> next = thead;
	}
	if((thead != NULL) && (theadbefore == lshead -> node) && (thead != lshead -> node)){
	  theadbefore -> next = tempN;
	  tempN -> next = thead;
	}
	if((thead != NULL) && (thead == lshead -> node)){
	  lshead -> node = tempN;
	  tempN -> next = thead;
	}
	//after the node insert, go to next list node
	lshead = lshead -> next;
	if(lshead == NULL){
	  lshead = listhead;
	}
      }
      lshead = listhead;
      //finished sort on that k sublists then merge into a big list
      Node * newhead = lshead -> node;
      head = lshead -> node;
      lshead -> node = lshead -> node -> next;
      lshead = lshead -> next;
      int a = 0;
      for(a = 0; a < tsize - 1; a++){
	if((lshead != NULL) && (lshead -> node != NULL)){
	    newhead -> next = lshead -> node;
	    lshead -> node = lshead -> node -> next;
	    lshead = lshead -> next;
	    newhead = newhead -> next;
	  }
	if(lshead == NULL){
	  lshead = listhead;
	}
      }
      
      lshead = listhead;
 
      k = k/3*2;//find the number before it in that line

      free(listhead);
 
    }
    k = 1;//reset the value of k that it can start from the bottom right next time

  }
  return head;
}













