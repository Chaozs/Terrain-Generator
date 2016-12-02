/*
Thien Trandinh
3GC3 Assignment 2
trandit
1420634
November 10th, 2016
*/
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <math.h>
#include "Terrain.h"

float eye[] = {0,0,0};
float lookAt[] {0,0,0};

float lightPos1[] = {-70, 38, -70, 1}; //initial light0 position
float lightPos2[] = {70, 38, 70, 1}; //initial light1 positon

float xAxis = 0; //x rotation of object
float yAxis = 0; //y rotation of object

float moveAcross=0; //for transitioning object right and left for camera
float moveForwardBackWard=0; //for transitioning object forward and backward for camera
float yPos=0; //for moving object up and down

int fillType = 0; //0=fill, 1=wireframe, 2=both
bool quad = true; //if true, draw with quads. if false, draw with triangles
bool flatShading = false; //if flat shading is on or not
bool lighting = true; //if lighting is on

Terrain terrain;

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
            //if 'q' pressed -> quit program
        case 'q':
            exit (0);
            break;
            //w Cycle Full shading, WireFrame, or Both
        case '1':
            if(fillType<2){
                fillType ++;
            }
            else{
                fillType=0;
            }
            break;
            //toggle flat shading
        case '2':
            if(flatShading){
                flatShading = false;
                glShadeModel(GL_FLAT);
            }
            else{
                flatShading = true;
                glShadeModel(GL_SMOOTH);
            }
            break;
            //toggle lighting on or off
        case '3':
            if (lighting){
                lighting = false;
                glDisable(GL_LIGHTING);
            }
            else{
                lighting = true;
                glEnable(GL_LIGHTING);
            }
            break;
            // increase size of terrain up to max of 300x300
        case '5':
            if(terrain.getSize()<300){
                terrain.incSize();
            }
            break;
            // decrease size of terrain down to minimal of 50x50
        case '4':
            if(terrain.getSize()>50){
                terrain.decSize();
            }
            break;
            // move camera up
        case '6':
            yPos--;
            break;
            //move camera down
        case '7':
            yPos++;
            break;
            //cycle between different algorithms. 
        case '8':
            terrain.changeAlgorithm();
            break;
        case 't':
            //set mesh to triangle strips
            quad=false;
            break;
            //set mesh to quad strips
        case 'y':
            quad=true;
            break;
            //move light0 left
        case 'a':;
            lightPos1[0]-=8;
            break;
            //move light0 right
        case 'd':
            lightPos1[0]+=8;
            break;
            //move light0 up
        case 'w':
            lightPos1[1]+=8;
            break;
            //move light0 down
        case 's':
            lightPos1[1]-=8;
            break;
            //move light1 left
        case 'j':
            lightPos2[0]-=8;
            break;
            //move light1 right
        case 'l':
            lightPos2[0]+=8;
            break;
            //move light1 up
        case 'i':
            lightPos2[1]+=8;
            break;
            //move light1 down
        case 'k':
            lightPos2[1]-=8;
            break;
            //move camera right
        case 'b':
            moveAcross+=1;
            break;
            //move camera left
        case 'm':
            moveAcross-=1;
            break;
            //move camera forward
        case 'h':
            moveForwardBackWard-=1;
            break;
            //move camera backwards
        case 'n':
            moveForwardBackWard+=1;
            break;
            //reset terrain
        case 'r':
            terrain.reset();
            break;
    }
    glutPostRedisplay();
}

void special(int key, int x, int y)
{
    //arrow key rotates camera
    switch(key)
    {
        case GLUT_KEY_LEFT:
            yAxis-=0.3;
            break;
        case GLUT_KEY_RIGHT:
            yAxis+=0.3;
            break;
        case GLUT_KEY_UP:
        if(xAxis<=6){
            xAxis += 0.3;
        }
            break;
        case GLUT_KEY_DOWN:
        if(xAxis>=-6){
            xAxis -= 0.3;
        }
            break;
    }
    glutPostRedisplay();
}

void createMenu(){
    cout <<"Model Settings:"  << endl;
    cout <<"1   -   Toggle Full shading, WireFrame, or Both"  << endl;
    cout <<"2   -   Toggle Flat Shading"  << endl;
    cout <<"3   -   Toggle lighting on and off"  << endl;
    cout <<"4   -   Decrease size of Terrain by 50(min 50)"  << endl;
    cout <<"5   -   Increase size of Terrain by 50(max 300)"  << endl;
    cout <<"8   -   EXTRA FEATURE - Cycle Algorithm - Circle, Fault, Particle, combination"  << endl;
    cout <<"t   -   Turn on triangle strips" <<endl;
    cout <<"y   -   Turn on quad strips" <<endl;
    cout << endl;

    cout <<"Light Control:"  << endl;    
    cout <<"a   -   Move light0 left" << endl;
    cout <<"d   -   Move light0 right" << endl;
    cout <<"w   -   Move light0 up" << endl;
    cout <<"s   -   Move light0 down" << endl;
    cout <<"j   -   Move light1 left" << endl;
    cout <<"l   -   Move light1 right" << endl;
    cout <<"i   -   Move light1 up" << endl;
    cout <<"k   -   Move light1 down" << endl;
    cout << endl;

    cout <<"EXTRA FEATURE ADVANCED CAMERA CONTROL"  << endl;
    cout <<"ArrowKey Left   -   Rotate camera left" << endl;
    cout <<"ArrowKey Right  -   Rotate camera right" << endl;
    cout <<"ArrowKey Up     -   Rotate camera forward" << endl;
    cout <<"ArrowKey Down   -   Rotate camera backwards" << endl;
    cout <<"b   -   Move camera left" << endl;
    cout <<"m   -   Move camera right" << endl;
    cout <<"h   -   Move camera forward" << endl;
    cout <<"n   -   Move camera backwards" << endl;
    cout <<"6   -   Move Camera Up"  << endl;
    cout <<"7   -   Move Camera Down"  << endl;
}
//restes camera everytime terrain grid size changes
void resetCamera(){
    lookAt[0] = terrain.getSize();
    lookAt[1] = -terrain.getSize()*0.35;
    lookAt[2] = terrain.getSize();

    eye[0] = -terrain.getSize()*0.5;
    eye[1] = terrain.getSize();
    eye[2] = -terrain.getSize()*0.5;
}

//initialize
void init(void)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glClearColor(0.3, 0.3, 0.3, 0);
    glColor3f(1, 1, 1);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);    

    
    gluPerspective(45, 1, 1, 1000);

    //enable backface culling
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
}


//display method to be recalled upon any changes
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    resetCamera();
    
    //set light colours
    float diff0[4] = {1.0, 0.1,0.1, 1}; 
    float diff1[4] = {0.6,0.3,0.1,1};

    float amb0[4] = {0.8,0.3, 0.1, 1};
    float amb1[4] = {0.8,0.5, 0.1, 1};

    float spec0[4] = {0.5,0.3, 0.8, 1};
    float spec1[4] = {0.5,0.3, 0.8, 1};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0); 
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);

    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);

    glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

    //set light to initial positions
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos2);

    //set camera initial location
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], 0,1,0);

    //calculate scale
    float scaleDegree = terrain.getSize()/50;

    //apply transformations
    glPushMatrix();
    //glScaled(0.16/scaleDegree,0.16/scaleDegree,0.16/scaleDegree); //for scaling terrain size down
    glTranslatef(terrain.getSize()/2,1,terrain.getSize()/2); //translate to center of terrain for rotation
    glRotatef(10*xAxis,1,0,0); //set x rotate transform
    glRotatef(10*yAxis,0,1,0); //set y rotate transform
    glTranslatef(moveAcross+moveForwardBackWard, yPos, -moveAcross+moveForwardBackWard); //set moving object ("camera") transform
    glTranslatef(-terrain.getSize()/2,1,-terrain.getSize()/2); //reset the center of terrain translate for rotation

    
    if(fillType==0){ //if full shading
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        terrain.drawTerrain(fillType, quad, lighting);
    }else if(fillType==1){ //if wireframe
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        terrain.drawTerrain(fillType, quad, lighting);
    }else{ //if both
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        terrain.drawTerrain(fillType-1, quad, lighting);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        terrain.drawTerrain(fillType, quad, lighting);
        
    }
    glPopMatrix();
    glutSwapBuffers();
}

//main method
int main(int argc, char** argv)
{
    glutInit(&argc, argv);      //starts up GLUT

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("2GC3 Assignment 2: Terrain Model: Thien Trandinh: 1420634");   //creates the window

    createMenu();
    glutDisplayFunc(display);   //registers "display" as the display callback function
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);

    glEnable(GL_DEPTH_TEST);
    init();

    glutMainLoop();             //starts the event loop

    return(0);                  //return may not be necessary on all compilers
}
