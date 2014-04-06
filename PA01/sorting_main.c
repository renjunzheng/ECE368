//
//  main.c
//  sorting_main
//
//  Created by Renjun Zheng on 2/16/14.
//  Copyright (c) 2014 Renjun Zheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long *Load_File(char *Filename, int *Size);
int Save_File(char *Filename, long *Array, int Size);
void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move);
void Shell_Selection_Sort(long *Array, int Size, double *N_Comp, double *N_Move);
int Print_Seq(char *Filename, int Size);

int main(int argc, char * argv[])
{
  if(argc != 5)
    {
      printf("/.proj1 i(s) input.txt seq.txt output.txt\n");
      return EXIT_FAILURE;
    }
    
  char * Inputfile = argv[2];
  char * Seqfile = argv[3];
  char * Outputfile = argv[4];
    
  int Size = 0;
  clock_t loadStart = 0;
  clock_t loadEnd = 0;
  loadStart = clock();
  long *Array = Load_File(Inputfile, &Size);
  loadEnd = clock();
  clock_t saveStart = 0;
  clock_t saveEnd = 0;
  clock_t start = 0;
  clock_t end = 0;
  double N_Comp = 0;
  double N_Move = 0;
  if(strcmp(argv[1] ,"i") == 0)
    {
      start = clock();
      Shell_Insertion_Sort(Array, Size, &N_Comp, &N_Move);
      end = clock();
    }
  else
    {
      start = clock();
      Shell_Selection_Sort(Array, Size, &N_Comp, &N_Move);
      end = clock();
    }
  saveStart = clock();  
  int save = Save_File(Outputfile, Array, Size);
  if(save == -1)
    {
      printf("Fail to save in Output file\n");
      return EXIT_FAILURE;
    }
    
  int seq = Print_Seq(Seqfile, Size);
  if(seq == -1)
    {
      printf("Fail to save in Sequence file\n");
      return EXIT_FAILURE;
    }
    saveEnd = clock();
    printf("Number of comparisons:%le\nNumber of moves: %le\nI/O time: %le\nSorting time: %le\n",N_Comp,N_Move,(double)(loadEnd - loadStart + saveEnd - saveStart)/CLOCKS_PER_SEC,(double)(end - start)/CLOCKS_PER_SEC);

    free(Array);
    return EXIT_SUCCESS;
}

