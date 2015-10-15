#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#define memBlock 1024*1024
#define lineMem 1024
#define delims ","

unsigned long readBlock(double *M, FILE* inputFile, unsigned long fileSize, unsigned long rowAmt, unsigned long colAmt, char* buffer){
  unsigned long delimIndex = 0;
  unsigned long i=0, j=0;
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
        M[(i*colAmt)+j] = atof(charInput);
        charInput = strtok(NULL, delims);
      }
   }
  return delimIndex;
}

double euclidDist(double* p1, double* p2, unsigned long dim){
  double accumulator=0;
  unsigned long i;
  for(i=0; i<dim; ++i)
    accumulator+=pow(p1[i]-p2[i], 2);
  return sqrt(accumulator);
}

int findClosest(double* p1, double* centers, unsigned long dim, unsigned long k){
  unsigned long closest = 0, i;
  double minDist=0, testDist=0;
  minDist = euclidDist(p1, centers, dim);
  for(i=1; i<k; ++i){
    testDist = euclidDist(p1, centers+(i*dim), dim);
    if( minDist > testDist){
      closest = i;
      minDist = testDist;
    }
  }
  return closest;
}

int centerDiff(double* centers1, double* centers2, double tol, unsigned long k, unsigned long dim){
  long i;
  double accum=0;
  for(i=0; i<k; ++i){
    accum += euclidDist(centers1+(i*dim), centers2+(i*dim), dim);
  }
  if(accum > tol)
    return 1;
  else
    return 0;
}

int main(int argc, char* argv[]){
//Variable Declorations
  FILE *inputFile; //done
  char *buffer, *tempChar; //done
  unsigned long i=0, j=0, k=atoi(argv[2]), l=0, closePoint=0, n=0; //done
  unsigned long colAmt=0, rowAmt=0, delimIndex=0, fileSize=0; //done
  double *M, *centers1, *centers2, *centersTemp, *centerWeights1, *centerWeights2, *centerWeightsTemp; //done

//Variable allocation and initialization
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
  centers1 = (double*) malloc(sizeof(double)*k*colAmt);
  centers2 = (double*) malloc(sizeof(double)*k*colAmt);
  centerWeights1 = (double*) malloc(sizeof(double)*k);
  centerWeights2 = (double*) malloc(sizeof(double)*k);
  srand(time(NULL));

  delimIndex = readBlock(M, inputFile, fileSize, rowAmt, colAmt, buffer);
  for(i=0; i<k; ++i){
    centerWeights1[i] = 0;
    centerWeights2[i] = 0;
    memmove(centers1+(i*colAmt), M+((rand())%delimIndex)*colAmt, sizeof(double)*colAmt);
    memmove(centers2+(i*colAmt), centers1+(i*colAmt), sizeof(double)*colAmt);
  }

  for(n=0; n<10; ++n){
    j=0;
    do{
      for(i=0; i< k; ++i)
        centerWeights2[i] = 0;
      do{
        for(i=0; i< delimIndex; ++i){
          closePoint = findClosest(M+(i*colAmt), centers1, colAmt, k);
          for(l=0; l<colAmt; ++l){
             centers2[closePoint*colAmt+l] = ((centers2[closePoint*colAmt+l]
                * centerWeights2[closePoint]) + M[i*colAmt + l]) / (centerWeights2[closePoint] +1);
          }
          ++centerWeights2[closePoint];
        }
        delimIndex = readBlock(M, inputFile, fileSize, rowAmt, colAmt, buffer); 
      }while(ftell(inputFile) != fileSize);
      rewind(inputFile);
      ++j;
      centersTemp = centers1;
      centers1 = centers2;
      centers2 = centersTemp;
      centerWeightsTemp = centerWeights1;
      centerWeights1 = centerWeights2;
      centerWeights2 = centerWeightsTemp;
    }while(centerDiff(centers1, centers2, .0001, k, colAmt));

  }
  for(i=0; i<k; ++i){
    printf("\nCenter %lu:", i+1);
    for(l=0; l<colAmt; ++l)
      printf(" %lf", centers1[i*colAmt + l]);
  }
  printf("\n");
  fclose(inputFile); 

/*Print Center Weights
  for(i=0;i<k;++i)
    printf(" %f", centerWeights1[i]);
  printf("\n");
  for(i=0;i<k;++i)
    printf(" %f", centerWeights1[i]);
  printf("\n");
*/
/*Print Centers
  for(i=0; i<k; ++i){
    for(j=0; j<colAmt;++j)
      printf(" %f", centers1[i*colAmt+j]);
    printf("\n");
  }
  for(i=0; i<k; ++i){
    for(j=0; j<colAmt;++j)
      printf(" %f", centers2[i*colAmt+j]);
    printf("\n");
  }
*/

/*Print Current Matrix
  for(i=0;i<rowAmt;++i){
    for(j=0;j<colAmt;++j)
      printf(" %f", M[i*colAmt+j]);
    printf("\n");
  }
*/
  free(buffer);
  free(M);
  free(centers1);
  free(centers2);
  free(centerWeights1);
  free(centerWeights2);

  return 1;
}
