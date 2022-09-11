from fileinput import filename
import os
import sys
import math
from tkinter import N
import numpy as np
import pandas as pd
import spkmeansmodule as spkmm 

MAX_ITER = 300
EPSILON = 0.0

def main():
    np.random.seed(0)
    args = sys.argv
    if len(args) != 4:
        printInvalidInput()
    goal = args[2]
    input_filename = args[3]
    
    k, output_filename = spkmm.execute_program(k, goal, input_filename)
    
    if goal == "spk":
        executeStage6(k, output_filename)
        vectors = pd.read_csv(filename, header = None)
def executeStage6(k, filename):
    if filename == None: 
        printanErrorHasOccurred()


def printInvalidInput():
    print("Invalid Input!")
    sys.exit(1)

def printanErrorHasOccurred():
    print("An Error Has Occurred")
    # TODO: remove files created using:
    # os.remove("")
    sys.exit(1)



    

if __name__ == "__main__":
    main()