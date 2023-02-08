#include "imageProcessing.h"

double PI = 3.14;

/**
 * private arc tangent function 
*/
static double atan(double x) {
    if (x == 0)
        return 0;
    
    if (x >= 1)
        return PI/4;
    
    if (x <= -1)
        return -PI/4;

    // linear value
    return x*PI/4;
}

int guidelineSearch(char imageBuf[IMGWIDTH][IMGWIDTH], int16_t *pos, float *angle){
    // diference between endPosition and startPosition.
    float delta=-1;
    
    // Determine guideline end position
    for(int i=0; i<IMGWIDTH; i++){
        if(imageBuf[GF_ROW][i] == GUIDELINE_COLOR) {
            delta = i;
            break;
        }
        if(i==IMGWIDTH-1) {
            printf("Failed to find guideline end position!\n");
            return -2;
        }
        
    }

    // Determine guideline start position
    for(int i=0; i<IMGWIDTH; i++){
        if(imageBuf[GN_ROW][i] == GUIDELINE_COLOR) {
            delta -= i;
            *pos = ((0.0+i+1)/(IMGWIDTH)*100);
            break;
        }
        if(i==IMGWIDTH-1) {
            printf("Failed to find guideline starting position!\n");
            return -1;
        }
    }

    // checks if the guideline is perpendicular.
    if(delta == 0) {
        *angle = 0;
    }

    *angle = atan(delta/IMGWIDTH);

    return 0;
}

int nearObstSearch(char imageBuf[IMGWIDTH][IMGWIDTH]) {
    for(int row = 0; row<NOB_ROW;row++) {
        for (int col=NOB_COL; col<NOB_WIDTH;col++) {
            if(imageBuf[row][col] == OBSTACLE_COLOR) {
                return 1;
            }
        }
    }
    return 0;
}

int obstCount(char imageBuf[IMGWIDTH][IMGWIDTH]) {
    bool inObject = false; //is in object
    int numObjects = 0;

    for(int row = 0; row<IMGWIDTH;row++) {
        inObject = false;
        for (int col=0; col<IMGWIDTH;col++) {
            if(imageBuf[row][col] == OBSTACLE_COLOR) {
               if(!inObject){
                numObjects++;
                inObject=true;
               }
            }
            if(inObject) {
                inObject = false;
            }
        }
    }
    return numObjects; 
}
