/**
 * @file imageDefinition.h
 * @author João Martins (joaofmartins@ua.pt); Raquel Milheiro (raq.milh@ua.pt)
 * @brief Defines the meaninful values for the image pixels.
 * @version 0.1
 * @date 2022-12-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __IMAGE_DEF_H__
#define __IMAGE_DEF_H__ 

#define BACKGROUND_COLOR 0x00   /* Color of the background */
#define GUIDELINE_COLOR 0xFF    /* Guideline color */
#define OBSTACLE_COLOR 0x80     /* Obstacle color */

#define IMG_TX_START 0x40     /* The start of the image transmission */
#define IMG_TX_DONE 0xC0     /* image has been completely sent */

#endif