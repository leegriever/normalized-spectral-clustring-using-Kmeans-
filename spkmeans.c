#include "spkmeans.h"

/* in case user is caliing spkmeans.c */
int main(int argc, char *argv[]) {
    char *goal, *filename;
    SPK_INFO *spk_info;
    if (argc != 3)
        printInvalidInput();
    
    goal = argv[1];
    filename = argv[2];
    spk_info = execute(0, goal, filename);
    free(spk_info);
    return 0;
}

/* conduct NSC algorithm and division to goals */
SPK_INFO* execute(int k, char *goal, char *filename){
    input_validity(k, goal);
    SPK_INFO *spk_info;
    INPUT_INFO *input_info;
    SPK_MATRIX *matrices;
    /* init and allocate returned object to python */
    initSpkInfo(spk_info, k);
    /* init and allocate input's baic info */
    initInputInfo(input_info, k, goal, filename);
    /* goal is jacobi */
    if (input_info->curr_goal == jacobi){
        readMat()
    }

}

void initSpkInfo(SPK_INFO *spk_info, int k){
    spk_info = (SPK_INFO*) malloc(sizeof(SPK_INFO));
    spk_info->k = 0;
    spk_info->spk_mat_filename = NULL;
}
void initInputInfo(INPUT_INFO *input_info, int k, char *goal, char *filename){
    input_info->k = k;
    input_info->n = calcNumOfVec(filename);
    input_info->input_filename = filename;
    initiateGoal(input_info, goal);
    if (input_info->curr_goal == jacobi)
        input_info->d = input_info->n;
    else
        input_info->d = calcDim(filename);
    if (input_info->d <= 0 || input_info->n <= 0)
        printInvalidInput();
}
void initMatrices(input_info){

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
        invalidInput();
    }
}

void readMat(char *fileName, double **array)
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




/* complete!!!!!*/
void inputValidity(int k, char *goal) {
   if (((strcmp(goal, "wam") != 0) && (strcmp(goal, "ddg") != 0) &&
    (strcmp(goal, "lnorm") != 0) && (strcmp(goal, "jacobi") != 0))) {
        printInvalidInput();
    }
    
}
/* note: correct free of both error functions */
void printInvalidInput() {
    freeFunc();
    printf("Invalid Input!\n");
    exit(1);
}

void printAnErrorHasOccured() {
    freeFunc();
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