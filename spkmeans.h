#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* methods */
SPK_INFO* normalizedSpectralClustering(int k, char *goal, char *filename);
void initInputInfo(INPUT_INFO *input_info, int k, char *goal, char *filename);
void input_validity(int argc, char *argv[]);
void printInvalidInput();
void printAnErrorHasOccured();

/* structs */
typedef struct spk_info
{
    int k;
    char * spk_mat_filename;

} SPK_INFO;

typedef struct input_info
{
    int k, n, d;
    char * input_filename;
    enum goal {
        wam, ddg, lnorm, jacobi, spk
    } curr_goal;
} INPUT_INFO;

typedef struct spk_matrix
{
    /* matrix og given points */
    double ** X;
    /* weighted adjecancy matrix */
    double ** W;
    /* miagonal degree matrix */
    double ** D;
    /* D**(-0.5) */
    double ** norm_D;
    /* identity matrix */
    double ** identity_matrix;
    /* The Normalized Graph Laplacian */
    double ** L_norm;
    /* matrix containing the k eigen vectors of L_norm as columns */
    double ** U;
    /* a structure of EIGEN items containing eigen value and corresponding eigen vector each */
    EIGEN ** eigenList;
    /* normalized U */
    double ** T;
    /*matrix of eigenvector as columns*/
    double ** V;
    /* cosen goal within enclosed options */
    enum goal {
        wam, ddg, lnorm, jacobi, spk
    } chosen_goal;
} SPK_MATRIX;