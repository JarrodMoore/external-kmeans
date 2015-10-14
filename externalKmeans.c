#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#define memBlock 1024*1024
#define lineMem 1024
#define delims ","
double **M;
unsigned long delimIndex;
unsigned long colAmt;
unsigned long rowAmt;
unsigned long fileSize;
FILE *inputFile;
char* buffer;



void readBlock(){
  delimIndex = 0;
  int i, j;
  double inputDouble;
  char *charInput;
  for(i=0;i<rowAmt;++i){
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
}
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

int main(int argc, char* argv[]){
  char *tempChar;
  buffer = malloc(lineMem);
  delimIndex = 0;
  inputFile = fopen(argv[1], "r");
  int i, j, k=atoi(argv[2]), l, closePoint;
  unsigned long firstIndex=0;
  double **centers[2];
  double *centerWeights[2];
//  centerWeights = (double**) malloc(sizeof(double*)*2);
//  centers = (double***) malloc(sizeof(double**)*2);
  centers[0] = (double**) malloc(sizeof(double*)*k);
  centers[1] = (double**) malloc(sizeof(double*)*k);
  fgets(buffer, memBlock, inputFile);
  tempChar = strtok(buffer, delims);
  while(tempChar != NULL){
    ++colAmt;
    tempChar = strtok(NULL, delims);
  }
  fseek(inputFile, 0, SEEK_END);
  fileSize = ftell(inputFile);
  rewind(inputFile);
  rowAmt = memBlock/(sizeof(double)*colAmt);
  M = malloc(sizeof(double**)*rowAmt);
  for(i=0; i< rowAmt; ++i)
    M[i] = malloc(sizeof(double)*colAmt);
  readBlock();
  centerWeights[0] = malloc(sizeof(double)*colAmt);
  centerWeights[1] = malloc(sizeof(double)*colAmt);
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
  return 1;
}
