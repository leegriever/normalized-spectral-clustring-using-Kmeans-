#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <float.h>

/* methods */


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
    double ** D_norm;
    /* identity matrix */
    double ** I;
    /* The Normalized Graph Laplacian */
    double ** l_norm;
    /* matrix containing the k eigen vectors of L_norm as columns */
    double ** U;
    /* normalized U */
    double ** T;
    /*matrix of eigenvector as columns*/
    double ** V;
    /* cosen goal within enclosed options */
    
} SPK_MATRIX;

    
typedef struct eigen_info
{
    double EG;
    double * EV;
} EIGEN_INFO;
