#define PY_SSIZE_T_CLEAN
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

/* define main methods */
char* Normalized_Spectral_Clustering(char* goal, int n,int dim, char* vectorsFile, int k);
void creat_W(double ** W, double **vectors, int numOfVec, int dim);
void creat_D(double ** D, double **  W, int numOfVec);
void creat_A(double ** L, double ** D, double **  W, double ** I, int numOfVec);
void Jacobi_algorithm(double** A, double** V, int numOfVec);
double J_A_step(double**L,double**V,int numOfVec);
void creat_T(double** T, double* eigenvaluesCopy, double* eigenvalues, int k, int numOfVec);
int find_k (double* eigenvaluesCopy, int numOfVec);
void freeFunc();

/* define auxiliary methods */
void readInput(char *fileName, double **array);
void nullMatrix(int n, double ** p);
double** matrixAlloc(int rowNum, int colNum);
double Distance(double* x, double* y,int dim);
void mult (double ** A, double **  B, double ** result, int size);
void freeMatrix(double** matrix, int rowNum);
void printMatrix(double** A, int numOfRows, int numOfcolumns);
void printAnErrorHasOccured();
void printInvalidInput();
int compare_double(const void *p1, const void *p2);
void writeToFile(int firtLine, char* outFileName, int k, int dim, double** centroids);

/* define variables */
int numOfVec, i1, i2, i3, k;
double sum; 
int check = 0;
double ep=0.00001;
double convergence=1;
int stage = 0;
double **vectors = NULL;
double **W = NULL;
double **D = NULL;
double **V = NULL;
double **A = NULL;
double **A3 = NULL;
double **A2 = NULL;
double **A0 = NULL;
double **A1 = NULL;
double **copyOfV = NULL;
double **P = NULL;
double **P_T = NULL;
double **T = NULL;
double *eigenvaluesCopy = NULL;
double *eigenvalues = NULL;

/*The Normalized Spectral Clustering Algorithm*/
char* Normalized_Spectral_Clustering(char* goal, int n,int dim, char* vectorsFile, int kToCheck) {
    
    numOfVec = n;
    eigenvalues = (double*)malloc(sizeof(double)*n);
    eigenvaluesCopy = (double*)malloc(sizeof(double)*n);
    vectors = matrixAlloc(numOfVec, dim);
    readInput(vectorsFile ,vectors);

    /*creat the Identity matrix */
    V =  matrixAlloc(numOfVec, numOfVec);
    for (i1=0;i1<numOfVec;i1++){
        for (i2=0;i2<numOfVec;i2++){
            if(i1==i2){
                V[i1][i2]=1.0;
            }
            else{
                V[i1][i2]=0.0;
            }
        }
    }

    /*goal="jacobi"*/
    if(strcmp(goal,"jacobi")==0){
        P = matrixAlloc(numOfVec, numOfVec);
        P_T = matrixAlloc(numOfVec, numOfVec);
        A0 = matrixAlloc(numOfVec, numOfVec);
        A1 = matrixAlloc(numOfVec, numOfVec);
        copyOfV = matrixAlloc(numOfVec, numOfVec);
        Jacobi_algorithm(vectors,V,numOfVec);
        for(i1=0;i1<numOfVec;++i1){
            if(i1==numOfVec-1){
                printf("%.4f\n", eigenvalues[i1]);
            }
            else{
                printf("%.4f,", eigenvalues[i1]);
            }
        }
        printMatrix(V,numOfVec,numOfVec);
        stage=-1;
        freeFunc();
        return "";
    }

    /*creating W*/
    W = matrixAlloc(numOfVec, numOfVec);
    creat_W(W,vectors,numOfVec,dim);

    /*goal="wam"*/
    if(strcmp(goal,"wam")==0){
        printMatrix(W,numOfVec,numOfVec);
        freeFunc();
        return "";
    }

    /*creating D*/
    D = matrixAlloc(numOfVec, numOfVec);
    creat_D(D,W,numOfVec);

    /*goal="ddg"*/
    if(strcmp(goal,"ddg")==0){
        printMatrix(D,numOfVec,numOfVec);
        freeFunc();
        return "";
    }

    /*creating 1/D^0.5*/
    for(i1=0;i1<numOfVec;i1++){
        D[i1][i1]=1/sqrt(D[i1][i1]);
    }

    /*creating A*/
    A = matrixAlloc(numOfVec, numOfVec);
    A3 = matrixAlloc(numOfVec, numOfVec);
    A2 = matrixAlloc(numOfVec, numOfVec);
    creat_A(A,D,W,V,numOfVec);

    /*goal="lnorm"*/
    if(strcmp(goal,"lnorm")==0){
        printMatrix(A,numOfVec,numOfVec);
        freeFunc();
        return "";
    }

    /* Jacobi algorithm*/
    P = matrixAlloc(numOfVec, numOfVec);
    P_T = matrixAlloc(numOfVec, numOfVec);
    A0 = matrixAlloc(numOfVec, numOfVec);
    A1 = matrixAlloc(numOfVec, numOfVec);
    copyOfV = matrixAlloc(numOfVec, numOfVec);

    Jacobi_algorithm(A,V,numOfVec);

    /*creating a copy of eigenvalues and sorting it*/
    if (eigenvaluesCopy == NULL) {
        printAnErrorHasOccured();
    }
    for(i1=0;i1<numOfVec;++i1){
        eigenvaluesCopy[i1]=eigenvalues[i1];
    }
    qsort(eigenvaluesCopy, numOfVec, sizeof(double), compare_double);

    /*determination of k*/
    if (kToCheck==0){
        k=find_k(eigenvaluesCopy,numOfVec);
    }
    else{
        k=kToCheck;
    }

    /*creat T*/
    T = matrixAlloc(numOfVec, k);
    creat_T(T, eigenvaluesCopy, eigenvalues, k, numOfVec);
    
    writeToFile(k, "C_outFile.txt", numOfVec, k, T);

    freeFunc();

    return"C_outFile.txt";
}

/* Creat The Weighted Adjacency Matrix = W */
void creat_W(double ** W, double **vectors, int numOfVec, int dim) {
    for (i1=0 ; i1<numOfVec;++i1){
        for (i2=0 ; i2<numOfVec;++i2){
            if (i1!=i2){
                W[i1][i2]=exp((-sqrt(Distance(vectors[i1],vectors[i2],dim))/2));
            }
            else{
                W[i1][i2]=0;
            }
        }
    }
}

/* Creat The Diagonal Degree Matrix = D */
void creat_D(double ** D, double **  W, int numOfVec) {
    for (i1=0 ; i1<numOfVec;i1++){
        sum=0;
        for(i2=0;i2<numOfVec;i2++){
            sum=sum+W[i1][i2];
            if(i1!=i2){
              D[i1][i2]=0;  
            }
        }
        D[i1][i1]=sum;
    }
}

/*creat The Normalized Graph Laplacian = A */
void creat_A(double ** A, double ** D, double **  W, double **  I, int numOfVec) {

    mult(D,W,A3,numOfVec);
    mult(A3,D,A2,numOfVec);

    for (i1=0;i1<numOfVec;i1++){
        for (i2=0;i2<numOfVec;i2++){
            A[i1][i2]=I[i1][i2]-A2[i1][i2];
        }
    }
}

/*Jacobi algorithm*/
void Jacobi_algorithm(double** A, double** V, int numOfVec){
    int i = 0;
    int j1 = 0;
    int j2 = 0;
    convergence=1.0;
    ep=0.00001;
    check=0;
    for(i=0;i<100;++i){
        check=0;
        for (j1=0 ; j1<numOfVec;j1++){
            for (j2=0 ; j2<numOfVec;j2++){
                if(j1!=j2){
                    if(A[j1][j2]!=0.0){
                        check=1;
                        break;
                    }
                }
            }
            if(check==1){
                break;
            }
        }
        if(check==0){
            break;
        }
        convergence=J_A_step(A,V,numOfVec);
        if(convergence<=ep){
            break;
        }
        fflush(stdout);
    }
    /*Putting eigenvalues ​​in the array*/
    if (eigenvalues == NULL) {
        printAnErrorHasOccured();
    }
    for(i1=0;i1<numOfVec;++i1){
        eigenvalues[i1]=A[i1][i1];
    }
}

/*A step of Jacobi algorithm*/
double J_A_step(double**A,double**V,int numOfVec){
    int x_of_max, y_of_max;
    double theta, t, c, s, sum1, sum2, result, max_elem; 
    max_elem=0;

    /*Finding a maximum element*/
    for (i1=0;i1<numOfVec;i1++){
        for (i2=i1+1;i2<numOfVec;i2++){
            if(fabs(max_elem)<fabs(A[i1][i2])){
                max_elem=A[i1][i2];
                x_of_max=i1;
                y_of_max=i2;
            }
        }
    }

    /*Calculation of c and s*/
    theta=(A[y_of_max][y_of_max]-A[x_of_max][x_of_max])/(2.0*A[x_of_max][y_of_max]);
    if (theta<0){
        t=-1.0/(fabs(theta)+sqrt(pow(theta,2)+1.0));
    }
    else{
        t=1.0/(fabs(theta)+sqrt(pow(theta,2)+1.0));
    }
    c =1.0/sqrt(pow(t,2)+1.0);
    s=c*t;

    /*creat P the Rotation Matrix*/
    for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
            if (i1==i2){
                if(i1==x_of_max||i1==y_of_max){
                    P[i1][i2]=c;
                }
                else{
                    P[i1][i2]=1.0;
                }
            }
            else if (i1==x_of_max&&i2==y_of_max){
                P[i1][i2]=s;
            }
            else if (i1==y_of_max&&i2==x_of_max){
                P[i1][i2]=-s;
            }
            else{
                P[i1][i2]=0.0;
            } 
        }
    }

    /*creat P_T the Rotation Matrix transposed*/
    for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
            P_T[i1][i2]=P[i2][i1]; 
        }
    }

    /*creat A1*/
    mult(P_T,A,A0,numOfVec);
    mult(A0,P,A1,numOfVec);

    /*Convergence calculation*/
     for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
            if(i1!=i2){
                sum1+=pow(A[i1][i2],2);
            }
        }
    }
    for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
            if(i1!=i2){
                sum2+=pow(A1[i1][i2],2);
            }
        }
    }

    result=fabs(sum1-sum2);

    /*changing A*/
     for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
            A[i1][i2]=A1[i1][i2];
        }
    } 

    /*creat a copy Of V*/
    for (i1=0;i1<numOfVec;++i1){
        for (i2=0;i2<numOfVec;++i2){
           copyOfV[i1][i2]=V[i1][i2]; 
        }
    }
    mult(copyOfV,P,V,numOfVec);

    return result;
}

/*Determining the number of clusters = k*/
int find_k (double* eigenvaluesCopy, int numOfVec){
    int index =0;
    int max = 0;
    for(i1=0;i1<numOfVec-1;i1++){
        if(fabs(eigenvaluesCopy[i1]-eigenvaluesCopy[i1+1])>max){
            max=fabs(eigenvaluesCopy[i1]-eigenvaluesCopy[i1+1]);
            index=i1;
        }
    }
    return index+1;
}

/*creat the final matrix = T */
void creat_T(double** T, double* eigenvaluesCopy, double* eigenvalues, int k, int numOfVec){
        for(i1=0;i1<k;i1++){
        check=0;
        for(i2=i1;i2>0;i2--){
            if(i1==0){
                break;
            }
            if(eigenvaluesCopy[i2-1]==eigenvaluesCopy[i1]){
                check+=1;
            }
        }
        for(i2=0;i2<numOfVec;i2++){
            if(eigenvaluesCopy[i1]==eigenvalues[i2]){
                if(check!=0){
                    check-=1;}
                else{
                    for(i3=0;i3<numOfVec;i3++){
                        T[i3][i1]=V[i3][i2];
                    }
                }
            }
        }
    }
    
    for(i1=0;i1<numOfVec;i1++){
        sum=0;
        for(i2=0;i2<k;i2++){
            sum+=pow(T[i1][i2],2);
        }
        for(i2=0;i2<k;i2++){
            T[i1][i2]=T[i1][i2]/sqrt(sum);
        }
    }

}

void printAnErrorHasOccured() {
    freeFunc();
    printf("An Error Has Occured\n");
    exit(1);
}

void printInvalidInput() {
    freeFunc();
    printf("Invalid Input!\n");
    exit(1);
}

void readInput(char *fileName, double **array)
{
    double coordinate;
    char comma;
    FILE *fptr;
    int row = 0, col = 0;
    array[0][0] = 0;
    if (!(fptr = fopen(fileName, "r"))) {
        printInvalidInput();  
    }
    while (fscanf(fptr, "%lf%c", &coordinate, &comma) == 2) {
        array[row][(col)++]+=coordinate;
        if (comma == '\n' || comma == '\r') {
            /*fscanf(fptr, "%c", &comma);*/
            row ++;
            col = 0;
        }
        else if (comma != ',') {
            printAnErrorHasOccured(); 
        }
    }
    fclose(fptr);
}

void nullMatrix(int n, double ** p) {
    int i;
    for (i = 0; i < n; i++) {
        p[i] = NULL;
    }
}

double** matrixAlloc(int rowNum, int colNum) {
    double **p = (double **)malloc(rowNum * sizeof(double*));
    if (p == NULL) {
        printAnErrorHasOccured();
    }
    nullMatrix(rowNum, p);
    stage +=1;
    for (i1=0;i1<rowNum;i1++) {
        p[i1] = (double*)malloc(colNum*sizeof(double));
        if (p[i1] == NULL){
            printAnErrorHasOccured();
        }
        for (i2=0; i2<colNum;i2++) {
            p[i1][i2] = 0.0;
        }

    }
    return p;
}

double Distance(double* x, double* y,int dim) {
    double sumOfSquaredDiffrence = 0;
    int i;
    for (i=0; i < dim; ++i){
        sumOfSquaredDiffrence += pow(x[i]-y[i],2);}
    return sumOfSquaredDiffrence;
}


void mult (double ** A, double **  B, double ** result, int size){
    for(i1=0;i1<size;++i1){
        for(i2=0;i2<size;++i2){
            sum=0.0;
            for(i3=0;i3<size;++i3){
                sum+=A[i1][i3]*B[i3][i2];
            }
            result[i1][i2]=sum;
        }
    }
}


void freeMatrix(double** matrix, int rowNum) {
    int i;
    for (i=0; i < rowNum; i++) {
        free(matrix[i]);
    }
}

void printMatrix(double** A, int numOfRows, int numOfcolumns){
        for (i1 = 0; i1 < numOfRows; i1++) {
            for (i2 = 0; i2 < numOfcolumns; i2++) {
                if (i2 != (numOfcolumns - 1)) {
                    printf("%.4f,", A[i1][i2]);
                } 
                else {
                    printf("%.4f\n", A[i1][i2]);
                }
            }
        }
}

int compare_double(const void *p1, const void *p2){
    const double *q1 = p1, *q2 = p2;
    if(*q1>*q2){
        return -1;
    };
    if(*q1<=*q2){
        return 1;
    };
    return 0;
}

void writeToFile(int firtLine, char* outFileName, int k, int dim, double** centroids){
    FILE* outFile = fopen(outFileName, "w");
    if(firtLine!=0){
        fprintf(outFile, "%i\n", firtLine);}
    for (i1 = 0; i1 < k; i1++) {
        for (i2 = 0; i2 < dim; i2++) {
            if (i2 != (dim - 1)) {
                fprintf(outFile, "%.4f,", centroids[i1][i2]);
            } else {
                fprintf(outFile, "%.4f\n", centroids[i1][i2]);
            }
        }
    }

    if (fclose(outFile) == EOF) {
        printAnErrorHasOccured();
    }
}

void freeFunc() {    
    free(eigenvalues);
    free(eigenvaluesCopy);
    if (((stage >= 1) || (stage==-1))&& (vectors != NULL)) {
        freeMatrix(vectors, numOfVec);
    }
    free(vectors);
    if ((stage >= 2) && (W != NULL)) {
        freeMatrix(W, numOfVec);
    }
    free(W);
    if ((stage >= 3) && (D != NULL)) {
        freeMatrix(D, numOfVec);
    }
    free(D);
    if (((stage >= 4)|| (stage==-1)) && (V != NULL)) {
        freeMatrix(V, numOfVec);
    }
    free(V);
    if ((stage >= 5) && (A != NULL)) {
        freeMatrix(A, numOfVec);
    }
    free(A);
    if ((stage >= 6) && (A3 != NULL)) {
        freeMatrix(A3, numOfVec);
    }
    free(A3);
    if ((stage >= 7) && (A2 != NULL)) {
        freeMatrix(A2, numOfVec);
    }
    free(A2);
    if (((stage >= 8)|| (stage==-1)) && (P != NULL)) {
        freeMatrix(P, numOfVec);
    }
    free(P);
    if (((stage >= 9)|| (stage==-1)) && (P_T != NULL)) {
        freeMatrix(P_T, numOfVec);
    }
    free(P_T);
    if (((stage >= 10)|| (stage==-1)) && (A0 != NULL)) {
        freeMatrix(A0, numOfVec);
    }
    free(A0);
    if (((stage >= 11)|| (stage==-1)) && (A1 != NULL)) {
        freeMatrix(A1, numOfVec);
    }
    free(A1);
    if (((stage >= 12)|| (stage==-1)) && (copyOfV != NULL)) {
        freeMatrix(copyOfV, numOfVec);
    }
    free(copyOfV);
    if ((stage >= 13) && (T != NULL)) {
        freeMatrix(T, k);
    }
    free(T);
}

int main(){
    Normalized_Spectral_Clustering("lnorm",100,10,"vectors.txt",0);
    return 1;
}