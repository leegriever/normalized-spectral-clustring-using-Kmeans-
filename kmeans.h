#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> 
#include <ctype.h>

/* methods */
static char* execute2();
double Distance(double* x, double* y, int dim);
void updateCentroid(double** centroids, double** clusters, int i, int dim, int n, double**vectors);
void assignVectorToCluster(double** vectors,double** centroids,double** clusters, int vectorIndex, int dim, int n, int k);
void freeMatrix(double** matrix, int rowNum);
double** matrixAlloc(int rowNum, int colNum);
void printInvalidInput();
void printAnErrorHasOccured();
static char* fit_c(int k, int n,int dim, int max_iter, double eps, char* vectorsFile, char* centroids);
void readInput(char *fileName, double **array);
void writeToFile(char* outFileName, int k, int dim, double** centroids);
void freeFunc();
void nullMatrix(int n, double ** p);
void restartclusters(double ** p, int k, int n);

/* variables */
double kDouble, max_iter_double;
int dim, g, row, col, num_of_unchanged_centorids, j, r, l, max_iter,t, numOfVec, stage = 0, k;

FILE * outFile;

char* vectorsFile, centroidsFile;
double **vectors = NULL;
double **centroids = NULL;
double** clusters = NULL;
double *old_centroid = NULL;
char* outputFileName = "outputFileName";