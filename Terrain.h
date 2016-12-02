#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <utility>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>

/*
Thien Trandinh
3GC3 Assignment 2
trandit
1420634
November 10th, 2016
*/
using namespace std;

class Terrain
{
    private:
    int terrainSize; //terrain size
    float maxHeight;//max terrain height
    float minHeight; //min terrain height
    float heightMap[300][300]; //height map 2d array for each x,z
    float normVec[300][300][3]; //normal vector for each x,z
    int interations; //iterations used for cicle
    int iterationsFault[6]={50, 100, 300, 400, 600, 800}; //iterations used for fault
    int iterationsParticle[6]={50000,500000,1000000,3000000,7000000,10000000}; //iterations used for particle
    int algorithms; //counter for which algorithm

public:
    Terrain();  //constructor
    int getSize(); //get terrainSize
    float scale(float oldValue); //scales height for greyscale
    void resetGreyScale(); //reset greyscale
    void incSize(); //increase Size
    void decSize(); //decrease Size
    void changeAlgorithm(); //change algorithm
    void circleAlgorithm(int iterations); //circle algorithm
    void faultAlgorithm(int iterations); //fault algorithm
    void particleAlgorithm(int iterations); //particle algorithm
    void buildNormal(); //build normals
    void drawTerrain(int wireFrameStack, bool quad, bool light); //draw terrain
    void reset(); //resets terrain
};

#endif
