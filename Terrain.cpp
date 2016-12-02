
/*
Thien Trandinh
3GC3 Assignment 2
trandit
1420634
November 10th, 2016
*/
#include "Terrain.h"


int iterations; //number of iterations of terrain algorithm
int algorithm = 0; //0=circleAlgorithm, 1=fault, 2=particle, 3= all 3 combined
float maxHeight = 0;
float minHeight = 0;

//constructor
Terrain::Terrain()
{
    terrainSize = 150;   // default size upon terrain construction
    iterations = terrainSize*3;
    // call functions to set up initial terrain
    circleAlgorithm(iterations);
    buildNormal(); //build normals
}

//increase size of grid
void Terrain::incSize(){
    terrainSize+=50;
    reset();
}
//decrease size of grid
void Terrain::decSize(){
    terrainSize-=50;
    reset();
}

//cycle through possible algorithms
void Terrain::changeAlgorithm(){
	if(algorithm!=3){
		algorithm++;
	}else
	{
		algorithm=0;
	}
	reset();
}

// returns current terrainSize
int Terrain::getSize()
{
    return terrainSize;
}

float Terrain::scale(float oldValue){
	float OldRange = (maxHeight - minHeight); 
	return ((oldValue - minHeight) / OldRange);
}

// draws terrain
void Terrain::drawTerrain(int wireFrameStack, bool quad, bool light)
{
    glColor3f(1,1,1);


    //offset for quad strips
    int quadOffset[4][2] = {{0,0}, {0,1}, {1,1}, {1,0}};
    if(quad){
        for(int z =1; z<terrainSize-1; z++){
            for(int x=1; x<terrainSize-1; x++){
                glBegin(GL_QUADS);
                for(int t = 0; t<4; t++){
                    int xWithOffset = x + quadOffset[t][0];
                    int zWithOffset = z + quadOffset[t][1];
                    float yWithOffset = heightMap[xWithOffset][zWithOffset];
                    glNormal3fv(normVec[xWithOffset][zWithOffset]);
                    if(wireFrameStack!=1){
                    float greyScale = scale(heightMap[xWithOffset][zWithOffset]);
                    glColor3f(greyScale, greyScale, greyScale);
                	}
                    glVertex3f(xWithOffset,yWithOffset,zWithOffset);
                }
                glEnd();
            }
        }
    }
    //if drawing with triangle, use GL_TRIANGLES instead
    else{
        for(int z =1; z<terrainSize-1; z++){
            for(int x=1; x<terrainSize-1; x++){
                glBegin(GL_TRIANGLES);
                		// top left
                		glNormal3fv(normVec[x][z+1]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x][z+1]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }
                        glVertex3f(x, heightMap[x][z+1], z+1);

                        glNormal3fv(normVec[x+1][z+1]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x+1][z+1]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }
                        glVertex3f(x+1, heightMap[x+1][z+1], z+1);

                        glNormal3fv(normVec[x][z]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x][z]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }
                        glVertex3f(x, heightMap[x][z], z);

                        // bottom right
                        glNormal3fv(normVec[x+1][z+1]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x+1][z+1]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }

                        glVertex3f(x+1, heightMap[x+1][z+1], z+1);

                        glNormal3fv(normVec[x+1][z]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x+1][z]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }
                        glVertex3f(x+1, heightMap[x+1][z], z);
                        
                        glNormal3fv(normVec[x][z]);
                        if(wireFrameStack!=1){
                        	float greyScale = scale(heightMap[x][z]);
                        	glColor3f(greyScale, greyScale, greyScale);
                        }
                        glVertex3f(x, heightMap[x][z], z);
                        
                        
                        
                        
                glEnd();
            }
        }
    }
}

void Terrain::resetGreyScale(){
	maxHeight=0;
	minHeight=0;
}


//circle terrain algorithm
void Terrain::circleAlgorithm (int iterations){
    for(int i =0; i<iterations; i++){
        int random_x = rand()%terrainSize; //random x point                        
        int random_z = rand()%terrainSize; //random z point                   
        int random_radius = (rand()%(terrainSize/5))+1; //random circle radius              
        float disp = terrainSize/50; //circle height
        for(int x = 0; x<terrainSize; x++){
            for(int z= 0; z<terrainSize; z++){
                int dx = x - random_x; //distance between
                int dz = z - random_z; //distance between
                float distance = sqrtf((dx*dx) + (dz*dz));
                float pd = (distance*2)/random_radius;
                if (fabs(pd) <= 1.0){
                    heightMap[x][z] += (disp/2.0) + (cos(pd*3.14)*(disp/2.0));
                    if(heightMap[x][z]>maxHeight){
                    	maxHeight = heightMap[x][z]; //set new max height
                    }else if(heightMap[x][z]<minHeight){
                    	minHeight = heightMap[x][z]; //set new min height
                    }
                }
            }
        }
    }
}

//fault terrain algorithm
void Terrain::faultAlgorithm(int iterations){
    for(int i =0; i<iterations; i++){
        float v = static_cast <float> (rand()); //random int
        float a = sin(v);                                
        float b = cos(v);                                
        float d = sqrt(terrainSize*terrainSize*2); //get distance  
        float c = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * d - d/2;

        for(int tx = 0; tx<terrainSize; tx++){
            for(int tz=0; tz<terrainSize; tz++){ //raise height of all on this side of line
                if (a*tx + b*tz - c > 0){                  
                    heightMap[tx][tz] += 1;
                     if(heightMap[tx][tz]>maxHeight){
                    	maxHeight = heightMap[tx][tz]; //set new max height
                    }
                }
                else{ //lower height on rest
                    heightMap[tx][tz] -= 1;   
                    if(heightMap[tx][tz]<minHeight){
                    	minHeight = heightMap[tx][tz]; //set new min height
                    }        
                }
            }
        }
    }
}

//particle terrain algorithm
void Terrain::particleAlgorithm(int iterations){
	int x = rand()%terrainSize;                         
    int z = rand()%terrainSize;

    for(int i=0; i<iterations; i++){
    	heightMap[x][z] += 0.1;
    	if(heightMap[x][z]>maxHeight){
            maxHeight = heightMap[x][z]; //set new max height
        }
	    int v = rand() % 5;
		switch(v) {
			case 0:
				if(x<terrainSize){
					x++;
				}else{
					x--;
				}
				break; 
			case 1: 
				if(x>0){
					x--;
				}else{
					x++;
				}
				break; 
			case 2: 
				if(z<terrainSize){
					z++;
				}else{
					z--;
				}
				break; 
			case 3: 
				if(z>0){
					z--;
				}else{
					z++;
				}
				break;
		}      
	}      
}

//build normals
void Terrain::buildNormal(){

    for(int x =0; x<terrainSize; x++){
        for(int z=0; z<terrainSize; z++){

            float vector1[3]; //down vector
            float vector2[3]; //left vector
            float vector3[3]; //up vector
            float vector4[3]; //right vector

            float crossProduct[3]; //normal of top right quad
            float crossProduct2[3]; //normal of bottom right quad
            float crossProduct3[3]; //normal of bottom left quad
            float crossProduct4[3]; //normal of top left quad


            vector4[0] = 1.0;
            vector4[1] = heightMap[x+1][z] - heightMap[x][z];
            vector4[2] = 0.0;

            vector3[0] = 0.0;
            vector3[1] = heightMap[x][z+1] - heightMap[x][z];
            vector3[2] = 1.0;

            vector2[0] = -1.0;
            vector2[1] = heightMap[x-1][z] - heightMap[x][z];
            vector2[2] = 0.0;
            
            vector1[0] = 0.0;
            vector1[1] = heightMap[x][z-1] - heightMap[x][z];
            vector1[2] = -1.0;
            



            //calculate normal of top right quad
            crossProduct[0] = vector1[1]*vector2[2] - vector1[2]*vector2[1];
            crossProduct[1] = vector1[2]*vector2[0] - vector1[0]*vector2[2];
            crossProduct[2] = vector1[0]*vector2[1] - vector1[1]*vector2[0];
            float unit = sqrt(crossProduct[0]*crossProduct[0] + crossProduct[1]*crossProduct[1] + crossProduct[2]*crossProduct[2]);
            crossProduct[0] = crossProduct[0]/unit;
            crossProduct[1] = crossProduct[1]/unit;
            crossProduct[2] = crossProduct[2]/unit;
            //calculate normal of top left quad
            crossProduct2[0] = vector2[1]*vector3[2] - vector2[2]*vector3[1];
            crossProduct2[1] = vector2[2]*vector3[0] - vector2[0]*vector3[2];
            crossProduct2[2] = vector2[0]*vector3[1] - vector2[1]*vector3[0];
            float unit2 = sqrt(crossProduct2[0]*crossProduct2[0] + crossProduct2[1]*crossProduct2[1] + crossProduct2[2]*crossProduct2[2]);
            crossProduct2[0] = crossProduct2[0]/unit2;
            crossProduct2[1] = crossProduct2[1]/unit2;
            crossProduct2[2] = crossProduct2[2]/unit2;

            //calculate normal of bottom right quad
            crossProduct3[0] = vector3[1]*vector4[2] - vector3[2]*vector4[1];
            crossProduct3[1] = vector3[2]*vector4[0] - vector3[0]*vector4[2];
            crossProduct3[2] = vector3[0]*vector4[1] - vector3[1]*vector4[0];
            float unit3 = sqrt(crossProduct3[0]*crossProduct3[0] + crossProduct3[1]*crossProduct3[1] + crossProduct3[2]*crossProduct3[2]);
            crossProduct3[0] = crossProduct3[0]/unit3;
            crossProduct3[1] = crossProduct3[1]/unit3;
            crossProduct3[2] = crossProduct3[2]/unit3;

            //calculate normal of bottom left quad
            crossProduct4[0] = vector4[1]*vector1[2] - vector4[2]*vector1[1];
            crossProduct4[1] = vector4[2]*vector1[0] - vector4[0]*vector1[2];
            crossProduct4[2] = vector4[0]*vector1[1] - vector4[1]*vector1[0];
            float unit4 = sqrt(crossProduct4[0]*crossProduct4[0] + crossProduct4[1]*crossProduct4[1] + crossProduct4[2]*crossProduct4[2]);
            crossProduct4[0] = crossProduct4[0]/unit4;
            crossProduct4[1] = crossProduct4[1]/unit4;
            crossProduct4[2] = crossProduct4[2]/unit4;

            //get average
            float tempAverage[3];
            tempAverage[0] = (crossProduct[0]+crossProduct2[0]+crossProduct3[0]+crossProduct4[0]);
            tempAverage[1] = (crossProduct[1]+crossProduct2[1]+crossProduct3[1]+crossProduct4[1]);
            tempAverage[2] = (crossProduct[2]+crossProduct2[2]+crossProduct3[2]+crossProduct4[2]);

            //calculate unit length
            float unit5 = sqrt(tempAverage[0]*tempAverage[0] + tempAverage[1]*tempAverage[1] + tempAverage[2]*tempAverage[2]);
            //set normal vector of each face using top right vector
            normVec[x][z][0] = tempAverage[0]/unit5;
            normVec[x][z][1] = tempAverage[1]/unit5;
            normVec[x][z][2] = tempAverage[2]/unit5;
        }
    }
}

//resets normal and terrain. Then remakes it with current values for terrain and size
void Terrain::reset()
{
	resetGreyScale();
    // reset height map and normal vectors
    for (int x=0; x<terrainSize; x++)
    {
        for (int z=0; z<terrainSize; z++)
        {
            heightMap[x][z] = 0;
            normVec[x][z][0] = 0;
            normVec[x][z][1] = 0;
            normVec[x][z][2] = 0;
        }
    }
    //if circle terrain
    if(algorithm==0){
    	circleAlgorithm(iterations);
    }else if(algorithm==1){ //if fault terrain
    	faultAlgorithm(iterationsFault[(terrainSize/50-1)]);
    }else if(algorithm==2){ //if particle terrain
    	particleAlgorithm(iterationsParticle[(terrainSize/50-1)]);
    }else{ //if all terrain
    	circleAlgorithm(iterations);
    	faultAlgorithm(iterationsFault[(terrainSize/50-1)]);
		particleAlgorithm(iterationsParticle[(terrainSize/50-1)]);
    }
    buildNormal();//reconstruct normals
}

