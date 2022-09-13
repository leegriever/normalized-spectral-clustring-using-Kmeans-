#include "spkmeans.h"

/* methods */

void initMat(double *** p, int rows, int cols);
void initMatrices();
void initSpkInfo();
void initInputInfo(int k, char *goal, char *filename);
void initEigenList();
void initiateGoal(INPUT_INFO *input_info, char *goal);
void createIdentityMat();
void inputValidity(int k, int n, int d, char *goal);
void printInvalidInput(int stage);
void printAnErrorHasOccured(int stage);
int calcNumOfVec(char *filename);
int calcDim(char *filename);
void readInput(char *fileName, double **m);
void create_W();
double Distance(double* x, double* y,int dim);
void freeMatrix(double** matrix, int rowNum);
void freefunc(int stage);
void freeE_lst();
void freeMatrices();
void printMat(double** A, int numOfRows, int numOfcolumns);

SPK_INFO *spk_info;
INPUT_INFO *input_info;
SPK_MATRIX *matrices;
EIGEN_INFO ** E_lst;

/* in case user is caliing spkmeans.c */

int main() {
    char *goal, *filename;
    /* if (argc != 3)
         printInvalidInput(0);
     goal = argv[1];
     filename = argv[2]; */
    goal = "wam";
    filename = "T_output";
    printf("1");
    spk_info = execute1(0, goal, filename);
    free(spk_info);
    return 0;
}

/* conduct NSC algorithm and division to goals */
SPK_INFO* execute1(int k, char *goal, char *filename){
    /* init and allocate returned object to python */
    initSpkInfo();
    spk_info->spk_mat_filename = filename;
    printf("execute1 in spkmeans.c %d", k);
    spk_info->k = 2;
    return spk_info;
    printf("2");
    /* init and allocate input's baic info */
    initInputInfo(k, goal, filename);
    printf("3");
    /* check validity for both c and python callings */
    inputValidity(input_info->k, input_info->n, input_info->d, goal);
    initMatrices();
    printf("4");
    initEigenList();

    /* readMat, check about reading from file diffrence for j and others */
    /* goal is jacobi */
    
    if (input_info->curr_goal == jacobi){
        /* whhhhy is it l_norm */
        /* complete jacobi stuff */
        readInput(input_info->input_filename, matrices->l_norm);
    }
    else{
        readInput(input_info->input_filename, matrices->X);
        /* step 1 in algorithm: buils weighted adjacency matrix*/ 
        create_W();
        if (input_info-> curr_goal == wam){
            printMat(matrices->W, input_info->n, input_info->n);
            freefunc(3);
        }
    }
    
    /*

    else if (spk){
        if option == 'dd-emo'
            do shira fuction
        elif option = 'dd-emo2'
            do shira2 function
    }

    */
   return spk_info;

}
void printMat(double** A, int numOfRows, int numOfcolumns){
    int i, j;
        for (i = 0; i < numOfRows; i++) {
            for (j = 0; j < numOfcolumns; j++) {
                if (j != (numOfcolumns - 1)) {
                    printf("%.4f,", A[i][j]);
                } 
                else {
                    printf("%.4f\n", A[i][j]);
                }
            }
        }
}


void initMat(double *** p, int rows, int cols) {
    int i;
    double ** m;
    *(p) = (double**) malloc(rows * sizeof(double*));
    m = *(p);
    if (m == NULL) {
        printAnErrorHasOccured(2);
    } else {
        for (i = 0; i < rows; i++) {
            m[i] = (double*) malloc(cols * sizeof(double));
            if (m[i] == NULL) {
                printAnErrorHasOccured(2);
            }
        }
    }
}
void initMatrices(){
    matrices = (SPK_MATRIX*) malloc(sizeof(SPK_MATRIX));
    initMat(&(matrices->X), input_info->n, input_info->d);
    initMat(&(matrices->W), input_info->n, input_info->n);
    initMat(&(matrices->D), input_info->n, input_info->n);
    initMat(&(matrices->D_norm), input_info->n, input_info->n);
    initMat(&(matrices->I), input_info->n, input_info->n);
    createIdentityMat();
    initMat(&(matrices->l_norm), input_info->n, input_info->n);
    initMat(&(matrices->V), input_info->n, input_info->n);
}
void initSpkInfo(){
    spk_info = (SPK_INFO*) malloc(sizeof(SPK_INFO));
    spk_info->k = 0;
    spk_info->spk_mat_filename = NULL;
}
void initInputInfo(int k, char *goal, char *filename){
    input_info = (INPUT_INFO*) malloc(sizeof(INPUT_INFO));
    input_info->k = k;
    input_info->n = calcNumOfVec(filename);
    input_info->input_filename = filename;
    initiateGoal(input_info, goal);
    if (input_info->curr_goal == jacobi)
        input_info->d = input_info->n;
    else
        input_info->d = calcDim(filename);
    
}
void initEigenList(){
    int i;
    int size; 
    E_lst = (EIGEN_INFO**) malloc(sizeof(EIGEN_INFO)*input_info->n);
    if (E_lst == NULL)
        printAnErrorHasOccured(3);
    for (i = 0; i < input_info->n; i++) {
        E_lst[i] = (EIGEN_INFO*) malloc(sizeof(EIGEN_INFO));
        if (E_lst == NULL)
            printAnErrorHasOccured(3);
        size = sizeof(double) * input_info->n;
		E_lst[i]->EV = (double *) malloc(size);
    }
}

void initiateGoal(INPUT_INFO *input_info, char *goal){
    if (strcmp(goal, "spk") == 0) {
        input_info->curr_goal = spk;
    } else if (strcmp(goal, "wam") == 0){
        input_info->curr_goal = wam;
    } else if (strcmp(goal, "ddg") == 0) {
        input_info->curr_goal = ddg;
    } else if (strcmp(goal, "lnorm") == 0) {
        input_info->curr_goal = lnorm;
    } else if (strcmp(goal, "jacobi") == 0) {
        input_info->curr_goal = jacobi;
    } else {
        printInvalidInput(1);
    }
}


/* complete!!!!!*/
void inputValidity(int k, int n, int d, char *goal) {
    if (d <= 0 || n <= 0 || k < 0 || k > n)
        printInvalidInput(1);
    if (((strcmp(goal, "wam") != 0) && (strcmp(goal, "ddg") != 0) &&
    (strcmp(goal, "lnorm") != 0) && (strcmp(goal, "jacobi") != 0))) {
        printInvalidInput(1);
    }
    
}
/* note: correct free of both error functions */
void printInvalidInput(int stage) {
    freefunc(stage);
    printf("Invalid Input!\n");
    exit(1);
}

void printAnErrorHasOccured(int stage) {
    freefunc(stage);
    printf("An Error Has Occurred\n");
    exit(1);
}

int calcNumOfVec(char *filename){
    char ch, ifeof;
    FILE *f = fopen(filename, "r");
    int numOfVec = 0;
    do {
        ch = fgetc(f);
        if(ch == '\n')
            numOfVec++;
    } while (ch != EOF);
    ifeof = fclose(f);
    if (ifeof == EOF) {
        printAnErrorHasOccured(0);
    }
    return numOfVec;
}

int calcDim(char *filename) {
    char ch, ifeof;
    FILE* f = fopen(filename, "r");
    int commasCount = 0;
    do {
        ch = fgetc(f);
        if(ch == ',')
            commasCount++;
    } while (ch != '\n');
    ifeof = fclose(f);
    if (ifeof == EOF) {
        printAnErrorHasOccured(0);
    }
    return (commasCount + 1);
}

void readInput(char *fileName, double **m)
{
    double coordinate;
    char comma;
    FILE *fptr;
    int row = 0, col = 0;
    m[0][0] = 0;
    if (!(fptr = fopen(fileName, "r"))) {
        printInvalidInput(3);  
    }
    while (fscanf(fptr, "%lf%c", &coordinate, &comma) == 2) {
        m[row][(col)++]+=coordinate;
        if (comma == '\n' || comma == '\r') {
            /*fscanf(fptr, "%c", &comma);*/
            row ++;
            col = 0;
        }
        else if (comma != ',') {
            printAnErrorHasOccured(3); 
        }
    }
    fclose(fptr);
}

/* all matrices creation funcs */

void createIdentityMat() {
    int i, j;
    for(i = 0; i < input_info->n; i++){
        for(j = 0; j < input_info->n; j++){
            if (i != j)
                matrices->I[i][j] = 0;
            else
                matrices->I[i][j] = 1;
        }
    }
}
/* Create The Weighted Adjacency Matrix = W */
void create_W() {
    int i, j;
    for (i = 0 ; i < input_info->n ; ++i){
        for (j = 0 ; j < input_info->n ; ++j){
            if (i != j) {
                matrices->W[i][j] = exp((-sqrt(Distance(matrices->X[i],matrices->X[j],input_info->d))/2));
            }
            else{
                matrices->W[i][j] = 0;
            }
        }
    }
}



/* all free related funcs */

void freefunc(int stage){
    /* only info_input and spk_info are aloocated */ 
    if (stage == 1){
        free(input_info);
        free(spk_info);
    }
    /* info_input, spk_info and matrices are allocated */ 
    if (stage == 2){
        free(input_info);
        free(spk_info);
        freeMatrices();
    }
    if (stage == 3){
        free(input_info);
        free(spk_info);
        freeMatrices();
        freeE_lst();
    }
}
void freeE_lst(){
    if (E_lst != NULL){
        int i;
        for (i = 0; i < input_info->n; i++) {
            if(E_lst[i]->EV != NULL)
                free(E_lst[i]->EV);
            free(E_lst);
        }
    }
    free(E_lst);
}
void freeMatrices(){
    if (matrices->X != NULL)
        freeMatrix(matrices->X, input_info->n);    
    free (matrices->X);
    if (matrices->W != NULL)
        freeMatrix(matrices->W, input_info->n);    
    free(matrices->W);
    if (matrices->D != NULL)
        freeMatrix(matrices->D, input_info->n);   
    free(matrices->D);
    if (matrices->D_norm != NULL)
        freeMatrix(matrices->D_norm, input_info->n);   
    free(matrices->D_norm);
    if (matrices->I != NULL)
        freeMatrix(matrices->I, input_info->n); 
    free(matrices->I);
    if(matrices->l_norm != NULL)
        freeMatrix(matrices->l_norm, input_info->n);   
    free(matrices->l_norm);
    if (matrices->V != NULL)
        freeMatrix(matrices->V, input_info->n);  
    free(matrices->V);
}

void freeMatrix(double** matrix, int rowNum) {
    int i;
    for (i = 0; i < rowNum; i++) {
        free(matrix[i]);
    }
}

/* utility methods */
double Distance(double* x, double* y,int dim) {
    double sumOfSquaredDiffrence = 0;
    int i;
    for (i=0; i < dim; ++i){
        sumOfSquaredDiffrence += pow(x[i]-y[i],2);}
    return sumOfSquaredDiffrence;
}
