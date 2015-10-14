#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#define memBlock 1024*1024
#define lineMem 1024
#define delims ","

unsigned long readBlock(double *M, FILE* inputFile, unsigned long fileSize, unsigned long rowAmt, unsigned long colAmt, char* buffer){
  unsigned long delimIndex = 0;
  int i=0, j=0;
  double inputDouble;
  char *charInput;
/*  for(i=0;i<rowAmt;++i){
      if(ftell(inputFile) == fileSize){
        break;
      }
      fgets(buffer, lineMem, inputFile);
      ++delimIndex;
      charInput = strtok(buffer, delims);
      for(j = 0; j<colAmt; ++j){
        M[i][j] = atof(charInput);
        charInput = strtok(NULL, delims);
      }
   }
*/
  return 2;
}
/*
double euclidDist(double* p1, double* p2, int dim){
  double accumulator=0;
  int i;
  for(i=0; i<dim; ++i)
    accumulator+=pow(p1[i]-p2[i], 2);
  return sqrt(accumulator);
}

int findClosest(double* p1, double** centers, int dim, int k){
  int closest = 0, i;
  double minDist, testDist;
  minDist = euclidDist(p1, centers[0], dim);
  for(i=1; i<k; ++i){
    testDist = euclidDist(p1, centers[i], dim);
    if( minDist > testDist){
      closest = i;
      minDist = testDist;
    }
  }
  return closest;
}

int centerDiff(double*** centers, double tol, int k, int dim){
  int i;
  double accum=0;
  for(i=0; i<k; ++i){
    accum += euclidDist(centers[0][i], centers[1][i], dim);
  }
  if(accum > tol)
    return 1;
  else
    return 0;
}
*/
int main(int argc, char* argv[]){

  FILE *inputFile; //done
  char *buffer, *tempChar; //done
  unsigned int i=0, j=0, k=atoi(argv[2]), l=0, closePoint=0; //done
  unsigned long colAmt=0, rowAmt=0, delimIndex=0, fileSize=0; //done
  double *M, *centers, *centerWeights; //done

  inputFile = fopen(argv[1], "r");
  buffer = malloc(lineMem);
  fgets(buffer, memBlock, inputFile);
  tempChar = strtok(buffer, delims);
  while(tempChar != NULL){
    ++colAmt;
    tempChar = strtok(NULL, delims);
  }
  rowAmt = memBlock/(sizeof(double)*colAmt);
  fseek(inputFile, 0, SEEK_END);
  fileSize = ftell(inputFile);
  rewind(inputFile);
  M = (double*) malloc(sizeof(double)*rowAmt*colAmt);
  centers = (double*) malloc(sizeof(double)*2*k*colAmt);
  centerWeights = (double*) malloc(sizeof(double)*2*k);


  readBlock(M, inputFile, fileSize, rowAmt, colAmt, buffer);
/*
  for(i=0; i<k; ++i){
    centers[0][i] = malloc(sizeof(double)*colAmt);
    centers[1][i] = malloc(sizeof(double)*colAmt);
    centerWeights[0][i] = 0;
    centerWeights[1][i] = 0;
    memmove(centers[1][i], M[(rand()+100) % rowAmt], sizeof(double)*colAmt);
    memmove(centers[0][i], centers[1][i], sizeof(double)*colAmt);
  }
  j=0;
  do{
    for(i=0; i< colAmt; ++i)
      centerWeights[j%2][i] = 0;
    do{
      for(i=0; i< delimIndex; ++i){
        closePoint = findClosest(M[i], centers[(j+1)%2], colAmt, k);
        for(l=0; l<colAmt; ++l){
           centers[j%2][closePoint][l] = ((centers[j%2][closePoint][l]
              * centerWeights[j%2][closePoint]) + M[i][l]) / (centerWeights[j%2][closePoint] +1);
        }
        ++centerWeights[j%2][closePoint];
      }
      readBlock(); 
    }while(ftell(inputFile) != fileSize);
    rewind(inputFile);
    ++j;
  }while(centerDiff(centers, .0001, k, colAmt));
  
  for(i=0; i<k; ++i){
    printf("\nCenter %i:", i+1);
    for(l=0; l<colAmt; ++l)
      printf(" %lf", centers[j%2][i][l]);
  }
  printf("\n");
*/  return 1;
}
