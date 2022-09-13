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
    try:
        k = int(args[1])
    except:
        printInvalidInput()
    goal = args[2]
    input_filename = args[3]
    
    k, output_filename = spkmm.execute_program(k, goal, input_filename)    
    print(k)
    if goal == "spk":
        executeStage6(k, output_filename)
        
    # delete spk file returnes from c
    os.remove(output_filename)

def executeStage6(k, filename):
    #TODO: check if vecors is what we want
    vectors = pd.read_csv(filename, header = None)
    vectors = vectors.to_numpy()
    n = vectors.shape[0]
    dim = len(vectors[0])
    centroids, centroidIndices = initializeCentroids(k, n, vectors)

    # writing vectors, centroids to files for c code to read from 
    np.savetxt("vectorsFile", X = vectors.tolist(), fmt = '%.4f', delimiter=',', newline='\n')
    np.savetxt("centroidsFile", X = centroids, fmt = '%.4f', delimiter=',', newline='\n')

    # executing c part of kmeans
    kmeans_output_filename = spkmm.execute_kmeans(k, n, dim, "vectorsFile", "centroidsFile")

    # print spk 
    # 1 row print
    print(",".join(str(vIndex) for vIndex in centroidIndices))
    # 2 row print
    # reads file accepted from c 
    finalCentroids = pd.read_csv(filename, header = None).to_numpy().tolist()
    for i in range(k):
        print(",".join(str(coordinate) for coordinate in finalCentroids[i]))
    # print empty row
    print("")

    # delte files
    os.remove("vectorsFile")
    os.remove("centroidsFile")

def initializeCentroids(k, n, vectors):
    np.random.seed(0)
    centroidsIndices = []
    centroids = []
    addToCentroids(n, centroidsIndices, centroids, vectors, None)
    i = 1
    while i < k:
        minDistances = []
        prob = []
        sumOfD = 0
        # loop over all vectors
        for l in range(n):
            Dl = sys.float_info.max
            # for each vector, find min distance by checking with all possible centroids
            for j in range(i):
                Dl  = min(Dl, Distance(vectors[l], centroids[j]))
            minDistances.append(Dl)
            sumOfD += Dl 
        # cala p for each vector  
        for l in range(n):
            p = minDistances[l]/sumOfD
            prob.append(p)
        i +=1
        addToCentroids(n, centroidsIndices, centroids, vectors, prob)
    return centroids, centroidsIndices

def addToCentroids(n, centroidsIndices, centroids, vectors, prob):
    randIndex = np.random.choice(n, p = prob)
    centroidsIndices.append(int(randIndex))
    centroids.append(list(vectors[randIndex]))

def Distance(x,y):
    sumOfSquaredDiffrence = 0
    for i in range(len(x)):
        sumOfSquaredDiffrence += math.pow(float(x[i])-float(y[i]),2)
    return sumOfSquaredDiffrence

def printInvalidInput():
    print("Invalid Input!")
    sys.exit(1)

def printanErrorHasOccurred():
    print("An Error Has Occurred")
    # TODO: remove files created using:
    sys.exit(1)



    

if __name__ == "__main__":
    main()