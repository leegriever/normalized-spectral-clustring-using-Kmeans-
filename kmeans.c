#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h> 
#include <ctype.h>

char* execute2();

char* execute2(){
    printf("3- arrived from python to c for kmeans");
    return "hi!";
}