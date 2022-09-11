#include "spkmeans.h"

/* in case user is caliing spkmeans.c */
int main(int argc, char *argv[]) {
    char *goal, *filename;
    SPK_INFO *spk_info;
    inputValidity(argc, argv);
    goal = argv[1];
    filename = argv[2];

    spk_info = process(0, goal, filename);
    free(spk_info);
    return 0;
}

/* conduct NSC algorithm and division to goals */
SPK_INFO* normalizedSpectralClustering(int k, char *goal, char *filename){
    SPK_INFO *spk_info;
    INPUT_INFO *input_info;
    SPK_MATRIX *matrices;
    spk_info->k = 0;
    spk_info->spk_mat_filename = NULL;

}

void initInputInfo(INPUT_INFO *input_info, int k, char *goal, char *filename){
    input_info->k = k;
    input_info->n = calcNumOfVec;
    input_info->d = calcDim;
    input_info->input_filename = filename;
    input_info->curr_goal = initiateGoal(input_info, goal);
}
void initMatrices(input_info){

}

void initiateGoal(INPUT_INFO *input_info, char *goal){
    
}


/* complete!!!!!*/
void inputValidity(int argc, char *argv[]) {
   char *file_name, *goal;
   /* must have exactly 3 arguments */
   if (argc != 3){
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