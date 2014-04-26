//
//  river.c
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//
#define EOF (-1)
#include <stdio.h>

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
    
    fclose(fptr);
    return 0;
}

