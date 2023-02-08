/**
 * @file imageProcessing.h
 * @author João Martins (joaofmartins@ua.pt)
 * @brief Image processing algorithms for SOTR 22 final project.
 * @version 0.1
 * @date 2022-11-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __IMAGEPROCESSING_H__
#define __IMAGEPROCESSING_H__

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Defines */
/* From provided code sample */

#define IMGWIDTH 128 /* Square image. Side size, in pixels*/

#define BACKGROUND_COLOR 0x00   /* Color of the background */
#define GUIDELINE_COLOR 0xFF    /* Guideline color */
#define OBSTACLE_COLOR 0x80     /* Obstacle color */

#define GN_ROW 0            /* Row to look for the guide line - close */
#define GF_ROW (IMGWIDTH-1) /* Row to look for the guide line - far */

#define NOB_ROW (IMGWIDTH/2)-1              /* Row to look for near obstacles */
#define NOB_COL 0.25*IMGWIDTH               /* Col to look for near obstacles */
#define NOB_WIDTH (0.75*IMGWIDTH)-NOB_COL   /* WIDTH of the sensor area */


/* Function Signatures */
/* Based on provided sample code */

/**
 * @brief Given an IMGWIDTHxIMGWIDTH image, finds the starting point and its angle.
 * 
 * @param imageBuf The current scene.
 * @param pos Pointer to store where the guideline starts in GN, percentage.
 * @param angle Pointer to store the angle between the guideline and the perpendicular axis, in radians.
 * @return int 0 if success. -1 if failed to find guideline start, -2 if failed to find guideline end.
 */
int guidelineSearch(uint8_t imageBuf[IMGWIDTH][IMGWIDTH], int16_t *pos, float *angle);

/**
 * @brief Detects if objects are present in the CSA of the current scene.
 * 
 * @param imageBuf The current scene.
 * @return int 1 if objects were detected, 0 if no object was detected.
 */
int nearObstSearch(uint8_t imageBuf[IMGWIDTH][IMGWIDTH]);

/**
 * @brief Counts the number of objects in the scene.
 * Is assumed that an object only uses one row.
 * 
 * @param imageBuf The current scene.
 * @return int The total number of objects in scene.
 */
int obstCount(uint8_t imageBuf[IMGWIDTH][IMGWIDTH]);

#endif /* __IMAGEPROCESSING_H__ */