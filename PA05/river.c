//
//  river.c
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
#define EOF (-1)
#include <stdio.h>
#include "river.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
    if(argc != 2){
        printf("input not correct: ""./proj5 input_file""\n");
        return 0;
    }
    
    FILE *fptr = fopen(argv[1],"r");
    if(fptr == NULL){
        printf("fail to open the input file\n");
        return 0;
    }
    
    int column = 0;
    fscanf(fptr,"%d", &column);
    int row = column - 1;
    
    printf("column number %d\n", column);
    //build a 2 dimensional array to determine how many vertex
    int poles[row][column];
    int lc1 = 0; //loop control
    int lc2 = 0;
    char value;
    value = fgetc(fptr);
    for(lc1 = 0; lc1 < row; lc1++){
        for(lc2 = 0; lc2 < column + 1; lc2++){
            //not sure why fscanf cannot operate
            value = fgetc(fptr);
            if(value == 48)
                poles[lc1][lc2] = 0;
            else if(value == 49)
                poles[lc1][lc2] = 1;
            
        }
    }
    //successfully read the file, already tested
    int index = 0;
    Vertex * head = buildVertex(row, column, poles, &index);
    int lc3 = 0;
    for(lc3 = 0; lc3 < index; lc3++){
        printf("%d %d %d\n", head -> x, head -> yt, head -> yb);
        head = head -> next;
    }
    fclose(fptr);
    return 0;
}


Vertex *createVertex(int x, int y, int index)
{
    Vertex *head = malloc(sizeof(Vertex));
    if(head == NULL){
        printf("fail to malloc Vertex\n");
        return NULL;
    }
    head -> index = index;
    head -> x = x;
    head -> yt = y;
    head -> yb = y + 1;
    head -> next = NULL;
    return head;
}

Vertex *buildVertex(int row, int column, int poles[row][column], int *index)
{
    int lc1 = 0;
    int lc2 = 0;
    Vertex *head;
    printf("number of rows %d, number of columns %d\n", row, column);
    for(lc2 = 0; lc2 < column; lc2++){
        for(lc1 = 0; lc1 < row; lc1++){
            printf("row: %d, column: %d \n", lc1, lc2);
            if(lc1 == 0){
                if(poles[lc1][lc2] == 1){
                    if(*index == 0){
                        head = createVertex(lc2, lc1, *index);
                        *index = *index + 1;
                    }else{
                        Vertex *bridge = createVertex(lc2, lc1, *index);
                        *index = *index + 1;
                        Vertex *temp = head;
                        while(temp -> next != NULL){
                            temp = temp -> next;
                        }
                        temp -> next = bridge;
                    }
                }
            }else if(lc1 > 0 && lc1 < row -1){
                if(poles[lc1][lc2] == 1){
                    if(poles[lc1 - 1][lc2] == 1){
                        Vertex *temp = head;
                        while(temp -> next != NULL){
                            temp = temp -> next;
                        }
                        temp -> yb += 1;
                    }else if(poles[lc1 - 1][lc2] == 0){
                        if(*index == 0){
                            head = createVertex(lc2, lc1, *index);
                            *index = *index + 1;
                        }else{
                        Vertex *bridge = createVertex(lc2, lc1, *index);
                            *index = *index + 1;
                        Vertex *temp = head;
                        while(temp -> next != NULL){
                            temp = temp -> next;
                        }
                        temp -> next = bridge;
                        }
                    }
                }
            }else if(lc1 == row - 1){
                if(poles[lc1][lc2] == 1){
                    if(poles[lc1 - 1][lc2] == 1){
                        if(*index == 0){
                            head = createVertex(lc2, lc1, *index);
                            *index = *index + 1;
                        }else{
                        
                        
                        Vertex *temp = head;
                        while(temp -> next != NULL){
                            temp = temp -> next;
                        }
                        temp -> yb += 1;
                        }
                    }else if(poles[lc1 - 1][lc2] == 0){
                        if(*index == 0){
                            head = createVertex(lc2, lc1, *index);
                            *index = *index + 1;
                        }else{
                        
                        Vertex *bridge = createVertex(lc2, lc1, *index);
                            *index = *index + 1;
                            
                        Vertex *temp = head;
                        while(temp -> next != NULL){
                            temp = temp -> next;
                        }
                        temp -> next = bridge;
                        }
                    }
                }
            }

        }
    }
    return head;
}
















