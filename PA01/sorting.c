#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long *Load_File(char *Filename, int *Size)
{
  //create a file pointer
  FILE * fptr;
    
  //use fopen to read the file and store in a file pointer
  fptr = fopen(Filename,"r");
    
  //check if this file is empty
  if(fptr == NULL)
    return NULL;
    
  //store the size
  fscanf(fptr, "%d", Size);
    
  //malloc a long intger array to store the array
  long * arr = malloc(sizeof(long) * (*Size));
    
  //create a index for the array and value to store the number read in the file
  int i = 0;
  long value = 0;
    
  //use fscanf to read value and store in array
  while(fscanf(fptr, "%ld", &value) == 1){
    arr[i] = value;
    i++;
  }
 
  fclose(fptr);
    
  return arr;
}

int Save_File(char *Filename, long *Array, int Size)
{
  FILE * fptr;
  int i;
  fptr = fopen(Filename,"w");
  if(fptr == NULL)
    return -1;
    
  fprintf(fptr,"%d\n", Size);
  for(i = 0; i < Size; i++)
    fprintf(fptr,"%ld\n",Array[i]);
    
  fclose(fptr);
  return Size;
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double * N_Move)
{  
  int tSize = Size;
    
  //calculate the height of the triangle
  int level = 0;
  while(tSize > 0)
    {
      tSize = tSize/3;
      level++;
    }
    
  //calculate the numbers of the subarray k
  int k = 1;
  int i = 0; //index of the for loop
  int l = 0; //index of the second for loop
  int m = 0; //index of the third for loop
  int o = 0; //index of the fourth for loop
  int temp = 0; //temporary value uesed to swap value
  int p = 0; //index of the while loop
  //implement the shell sort by insertion sort
  for(i = level; i > 0; i--)
    {
      //i = 3,2,1
      //find the order of 3 at that level
      for(l = 0; l < i - 1; l++)
	  k = k*3;
      for(m = 0; m < i; m++)
        {//insertion sort for each sub-array
	  for(o = k; o < Size; o++)
            {
	      temp = Array[o];
	      p = o;
	      while((p >= k) && (Array[p - k] > temp))
                {
		  Array[p] = Array[p - k];
		  p = p - k;
		  *N_Move = *N_Move + 2;
		  *N_Comp = *N_Comp + 1;
                }
	      if(p >= k)
		{
		  *N_Comp = *N_Comp + 1;
		}
	      Array[p] = temp;
	      *N_Move = *N_Move + 1;
            }
            k = k/3*2;//find the number before it in that line
        }
      k = 1;//reset the value of k that it can start from the bottom right next time
    }
}

void Shell_Selection_Sort(long *Array, int Size, double *N_Comp, double *N_Move)
{
  int i = 0;//index of the for loop
  int j = 0;//index of the second for loop
  int temp = 0;
  int k = 1;
  int l = 0; //index of the second for loop
  int m = 0; //index of the third for loop
  int o = 0; //index of the fourth for loop

  int tSize = Size;
    
  //calculate the height of the triangle
  int level = 0;
  while(tSize > 0)
    {
      tSize = tSize/3;
      level++;
    }
  for(i = level; i > 0; i--)
    {
      //find the order of 3 at that level
      for(l = 0; l < i - 1; l++)
        {
	  k = k*3;
        }
      for(m = 0; m < i; m++)
        {//selection sort for each sub-array
	  for(o = 0; o < Size; o++)//use 0 instead of k
            {
	      int minindex = o;
	      j = o;
	      while(j < Size)
                {
		  if(Array[j]< Array[minindex])
                    {
		      minindex = j;
                    }
		  *N_Comp = *N_Comp + 1;
		  j = j + k;
                }
	      if(minindex != i)
                {
		  temp = Array[minindex];
		  Array[minindex] = Array[o];
		  Array[o] = temp;
		  *N_Move = *N_Move + 3;
                }
            }
            k = k/3*2;//find the number before it in that line
        }
        k = 1;//reset the value of k that it can start from the bottom right next time
    }
}

int Print_Seq(char *Filename, int Size)
{
  int i = 0;
  int m = 0;
  int l = 0;
  int k = 1;
  int n = 0;
  int level = 0;
  int tSize = Size;
  while(tSize > 0)
    {
      tSize = tSize/3;
      level++;
    }
  int num = 0;
  //find the total number of sequence
  num = (1 + level)*(level - 1 + 1)/2;
  int *array = malloc(sizeof(int) * num);
  for(i = 1; i < level + 1; i++)
    {
      k = 1;
      for(l = 0; l < i - 1; l++)
	k = k*2;//find the smallest number in that line
      array[n] = k;
      for(m = 0; m < i - 1; m++)
        {
	  n++;
	  k = k/2*3;//find the number after it in that line
	  array[n] = k;
        }
      n++;
    }

  FILE * fptr;
  fptr = fopen(Filename, "w");
  if(fptr == NULL)
    return -1;
    
  fprintf(fptr,"%d\n", num);
  for(i = 0; i < num; i++)
    {
      fprintf(fptr,"%d\n",array[i]);
    }
    
  fclose(fptr);
  free(array);
  return 0;
}
