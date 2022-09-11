#include "spkmeans.h"




int main(int argc, char *argv[]) {
    char *goal, *input_filename;
    SPK_SETUP *returned_spk_values;
    checkInputValidity(argc, argv);
    goal = argv[1];
    input_filename = argv[2];

    returned_spk_values = process(0, goal, input_filename);
    free(returned_spk_values);
    return 0;
}
