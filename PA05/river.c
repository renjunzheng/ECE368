//
//  river.c
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
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
    Vertex * temp = head;
    
    //successfully build the list, already tested

    
    //now add the two shore
    //build start
    Vertex *start = createVertex(-1, 0, -1);
    start -> yb = column - 1;
    start -> next = head;
    head = start;
    temp = head;
    
    //build end
    while(temp -> next != NULL){
        temp = temp -> next;
    }
    Vertex *end = createVertex(column, 0, index);
    end -> yb = column - 1;
    temp -> next = end;

    int num = index + 2;

    Bridge * bTemp = NULL;
    temp = head;
    for(lc1 = 0; lc1 < num; lc1++){
        bTemp = temp -> node;
        for(lc2 = 0; lc2 < num; lc2 ++){
            Bridge *bridge = createBridge(lc2, lc1, head);
            bTemp -> next = bridge;
            if(bTemp -> next != NULL)
                bTemp = bTemp -> next;
        }
        temp = temp -> next;
    }
    
    //successfully build the adjacency list, already tested
    
    temp = head;
    for(lc1 = 0; lc1 < num; lc1++){
        bTemp = temp -> node;
        bTemp -> turns = 1000;
        temp = temp -> next;
    }
    temp = head;
    temp -> node -> turns = 0;

    //Bellman-Ford
    Vertex *secTemp = head;
    int lc3 = 0;
    temp = head -> next;
    Bridge *briTemp = NULL;
    for(lc1 = 1; lc1 < num - 1; lc1++){
        briTemp = temp -> node -> next;
        for(lc2 = 0; lc2 < num - 2; lc2++){
            secTemp = head;
            for(lc3 = 0; lc3 < briTemp -> index; lc3 ++){
                secTemp = secTemp -> next;
            }
            if(temp -> node -> turns > (briTemp -> turns + secTemp -> node -> turns)){
                temp -> node -> turns = (briTemp -> turns + secTemp -> node -> turns);
            }
            briTemp = briTemp -> next;
        }
        temp = temp -> next;
    }

    temp = head;
    while(temp -> next -> next != NULL){
        temp = temp -> next;
    }
    printf("%d\n", temp -> node -> turns);
    
    
    //free the linked list
    for(lc1 = 0; lc1 < num; lc1++){
        bTemp = head -> node;
        for(lc2 = 0; lc2 < num - 1; lc2 ++){
            Bridge *delete = bTemp -> next;
            bTemp -> next = delete -> next;
            if(delete != NULL)
                free(delete);
        }
        free(bTemp);
        temp = head;
        head = head -> next;
        free(temp);
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
    head -> node = malloc(sizeof(Bridge));
    head -> node -> index = index;
    head -> node -> turns = 0;
    head -> node -> next = NULL;
    return head;
}

Vertex *buildVertex(int row, int column, int poles[row][column], int *index)
{
    int lc1 = 0;
    int lc2 = 0;
    Vertex *head = NULL;
    for(lc2 = 0; lc2 < column; lc2++){
        for(lc1 = 0; lc1 < row; lc1++){
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

Bridge *createBridge(int indexnum, int sourcenum, Vertex *head)
{
    if(indexnum == sourcenum)
        return NULL;
    Bridge *node = malloc(sizeof(Bridge));
    node -> index = indexnum;
    node -> next = NULL;
    Vertex *temp = head;
    Vertex *source = NULL;
    Vertex *bridge = NULL;
    int dify = 0;
    int difx = 0;
    int lc1 = 0;
    for(lc1 = 0; lc1 < sourcenum; lc1 ++){
        temp = temp -> next;
    }
    source = temp;
    temp = head;
    for(lc1 = 0; lc1 < indexnum; lc1 ++){
        temp = temp -> next;
    }
    bridge = temp;
    
    if(source -> x == bridge -> x){
        if(source -> yb < bridge -> yt){
            node -> turns = 2*(bridge -> yt - source -> yb);
        }else if(bridge -> yb < source -> yt){
            node -> turns = 2*(source -> yt - bridge -> yb);
        }
    }else if(source -> x < bridge -> x){
        difx = bridge -> x - source -> x;
        if(source -> yb < bridge -> yt){
            dify = bridge -> yt - source -> yb;
            if(dify >= difx){
                node -> turns = dify*2;
            }else{
                node -> turns = 2*difx - 1;
            }
        }else if(bridge -> yb < source -> yt){
            dify = source -> yt - bridge -> yb;
            if(dify >= difx){
                node -> turns = dify*2;
            }else{
                node -> turns = 2*difx - 1;
            }
        }else if(source -> yb > bridge -> yt){
            node -> turns = 2*difx - 1;
        }else if(bridge -> yb > source -> yt){
            node -> turns = 2*difx - 1;
        }
    }else if(source -> x > bridge -> x){
        difx = source -> x - bridge -> x;
        if(source -> yb < bridge -> yt){
            dify = bridge -> yt - source -> yb;
            if(dify >= difx){
                node -> turns = dify*2;
            }else{
                node -> turns = 2*difx - 1;
            }
        }else if(bridge -> yb < source -> yt){
            dify = source -> yt - bridge -> yb;
            if(dify >= difx){
                node -> turns = dify*2;
            }else{
                node -> turns = 2*difx - 1;
            }
        }else if(source -> yb > bridge -> yt){
            node -> turns = 2*difx - 1;
        }else if(bridge -> yb > source -> yt){
            node -> turns = 2*difx - 1;
        }
    }
    return node;
}