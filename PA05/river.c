//
//  river.c
//  river
//
//  Created by Renjun Zheng on 4/18/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

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
    int row = column;
    
    return 0;
}

